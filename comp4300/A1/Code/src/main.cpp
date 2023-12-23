/*
* TODO: Code cleanup
* - put more behavior in base shape class using a pointer to a sf::Shape in Shape abstract class
* - draw text using less hacky way you can use character.getHeight or something like that
* - clean up gui drawing code and shape initalization
*/

#include <iostream>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui-SFML.h"

class Shape
{
public:
	Shape(int charSize, const sf::Color& textColor, const std::string& name, const sf::Font& font, const sf::Color& color, const sf::Vector2f& pos, const sf::Vector2f velocity, float speed, sf::RenderWindow& window)
		: m_name(name, font, charSize)
		, m_color(color)
		, m_pos(pos)
		, m_velocity(velocity)
		, m_speed(speed)
		, m_window(window)
		, isVisible(true)
		, scale(1.0f)

	{
		m_name.setFillColor(textColor);
		colorArray[0] = m_color.r / 255.0f;
		colorArray[1] = m_color.g / 255.0f;
		colorArray[2] = m_color.b / 255.0f;
		nameString = m_name.getString();
	}

	const std::string getName() const
	{
		return m_name.getString();
	}

	sf::Vector2f& exposeVelocity()
	{
		return m_velocity;
	}

	virtual void update() = 0;
	virtual void draw() = 0;

protected:
	sf::Text m_name;
	sf::Vector2f m_pos;
	sf::Vector2f m_velocity;
	sf::Color m_color;
	float m_speed;
	sf::RenderWindow& m_window;

public:
	bool isVisible;
	float scale;
	float colorArray[3];
	std::string nameString;
};

class RectangleShape : public Shape
{
public:
	RectangleShape(int charSize, const sf::Color& textColor, const std::string& name, const sf::Font& font, const sf::Color& color, const sf::Vector2f& pos, const sf::Vector2f velocity, float speed, sf::RenderWindow& window, sf::Vector2f size)
		: Shape(charSize, textColor, name, font, color, pos, velocity, speed, window)
		, m_size(size)
	{
		m_shape.setSize(size);
		m_shape.setPosition(pos);
		m_shape.setFillColor(color);
	}

	void update() override
	{
		// update shape

		if ((m_shape.getGlobalBounds().left < 0.0f) || (m_shape.getGlobalBounds().left + m_shape.getLocalBounds().width * scale > m_window.getSize().x))
		{
			m_velocity.x *= -1.0f;
		}

		if ((m_shape.getGlobalBounds().top < 0.0f) || (m_shape.getGlobalBounds().top + m_shape.getLocalBounds().height * scale > m_window.getSize().y))
		{
			m_velocity.y *= -1;
		}

		m_pos.x += m_velocity.x * m_speed;
		m_pos.y += m_velocity.y * m_speed;
		m_shape.setPosition(m_pos);
		m_shape.setScale(scale, scale);

		// update text

		sf::Vector2f topLeft(m_pos.x, m_pos.y - 6.0f);	// minus 6 because dumb text handeling in sfml
		float namePosX = (topLeft.x + m_shape.getSize().x * scale / 2.0f) - (m_name.getLocalBounds().width / 2.0f);
		float namePosY = (topLeft.y + m_shape.getSize().y * scale / 2.0f) - (m_name.getLocalBounds().height / 2.0f);
		m_name.setPosition(namePosX, namePosY);
		m_name.setString(nameString);

		// update color

		m_shape.setFillColor(sf::Color(colorArray[0] * 255.0f, colorArray[1] * 255.0f, colorArray[2] * 255.0f));
	}

	void draw() override
	{
		if (isVisible)
		{
			m_window.draw(m_shape);
			m_window.draw(m_name);
		}
	}

private:
	sf::Vector2f m_size;
	sf::RectangleShape m_shape;
};

class CircleShape : public Shape
{
public:
	CircleShape(int charSize, const sf::Color& textColor, const std::string& name, const sf::Font& font, const sf::Color& color, const sf::Vector2f& pos, const sf::Vector2f velocity, float speed, sf::RenderWindow& window, float radius)
		: Shape(charSize, textColor, name, font, color, pos, velocity, speed, window)
		, m_radius(radius)
	{
		m_shape.setRadius(radius);
		m_shape.setPosition(pos);
		m_shape.setFillColor(color);
	}

	void update() override
	{
		// update shape

		if ((m_shape.getGlobalBounds().left < 0.0f) || (m_shape.getGlobalBounds().left + m_shape.getLocalBounds().width > m_window.getSize().x))
		{
			m_velocity.x *= -1.0f;
		}

		if ((m_shape.getGlobalBounds().top < 0.0f) || (m_shape.getGlobalBounds().top + m_shape.getLocalBounds().height > m_window.getSize().y))
		{
			m_velocity.y *= -1;
		}

		m_pos.x += m_velocity.x * m_speed;
		m_pos.y += m_velocity.y * m_speed;
		m_shape.setPosition(m_pos);
		m_shape.setRadius(m_radius * scale);

		// update text

		sf::Vector2f topLeft(m_pos.x, m_pos.y - 6.0f);	// minus 6 because dumb text handeling in sfml
		float namePosX = (topLeft.x + m_shape.getRadius()) - (m_name.getLocalBounds().width / 2.0f);
		float namePosY = (topLeft.y + m_shape.getRadius()) - (m_name.getLocalBounds().height / 2.0f);
		m_name.setPosition(namePosX, namePosY);
		m_name.setString(nameString);

		// update color

		m_shape.setFillColor(sf::Color(colorArray[0] * 255.0f, colorArray[1] * 255.0f, colorArray[2] * 255.0f));
	}

	void draw() override
	{
		if (isVisible)
		{
			m_window.draw(m_shape);
			m_window.draw(m_name);
		}
	}

private:
	float m_radius;
	sf::CircleShape m_shape;
};

struct Data
{
	int wWidth, wHeight;
	sf::RenderWindow window;
	sf::Font font;
	int textSize = 30;
	sf::Color textColor;
	std::vector<Shape*> shapes;

	Data() = delete;
	Data(const std::string& configFile)
	{
		std::ifstream fin(configFile);
		if (!fin)
		{
			std::cerr << "Could not load config file!" << std::endl;
			exit(-1);
		}

		std::string token;
		while (fin >> token)
		{
			if (token == "Window")
			{
				fin >> wWidth;
				fin >> wHeight;

				window.create(sf::VideoMode(wWidth, wHeight), "A1", sf::Style::Close | sf::Style::Titlebar);
				window.setFramerateLimit(60);
			}
			else if (token == "Font")
			{
				std::string fontFile;
				int r, g, b;

				fin >> fontFile;
				fin >> textSize;
				fin >> r;
				fin >> g;
				fin >> b;

				textColor.r = r;
				textColor.g = g;
				textColor.b = b;
				textColor = sf::Color(r, g, b);

				if (!font.loadFromFile(fontFile))
				{
					std::cerr << "Could not load font file!" << std::endl;
					exit(-1);
				}
			}
			else if (token == "Rectangle")
			{
				std::string name;
				sf::Vector2f pos;
				sf::Vector2f speed;
				sf::Vector2f size;
				int r, g, b;

				fin >> name;
				fin >> pos.x;
				fin >> pos.y;
				fin >> speed.x;
				fin >> speed.y;
				fin >> r;
				fin >> g;
				fin >> b;
				fin >> size.x;
				fin >> size.y;

				shapes.push_back(new RectangleShape(textSize, textColor, name, font, sf::Color(r, g, b), pos, speed, 1.0f, window, size));
			}
			else if (token == "Circle")
			{
				std::string name;
				sf::Vector2f pos;
				sf::Vector2f speed;
				float radius;
				int r, g, b;

				fin >> name;
				fin >> pos.x;
				fin >> pos.y;
				fin >> speed.x;
				fin >> speed.y;
				fin >> r;
				fin >> g;
				fin >> b;
				fin >> radius;

				shapes.push_back(new CircleShape(textSize, textColor, name, font, sf::Color(r, g, b), pos, speed, 1.0f, window, radius));
			}
		}
	}
};

int main()
{
	Data world("config.txt");

	// Initialize IMGUI
	ImGui::SFML::Init(world.window);
	sf::Clock guiClock;
	ImGui::GetStyle().ScaleAllSizes(1.0f);

	while (world.window.isOpen())
	{
		sf::Event event;
		while (world.window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
			{
				world.window.close();
			}
		}

		for (auto& s : world.shapes)
		{
			s->update();
			s->draw();
		}

		ImGui::SFML::Update(world.window, guiClock.restart());
		
		ImGui::Begin("Controls");
		static int item_current_idx = 0;
		const std::string& combo_preview_value = world.shapes[item_current_idx]->getName();
		if (ImGui::BeginCombo("Select Shape", combo_preview_value.c_str()))
		{
			for (int n = 0; n < world.shapes.size(); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(world.shapes[n]->getName().c_str(), is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::Checkbox("visible", &world.shapes[item_current_idx]->isVisible);
		ImGui::SliderFloat("scale", &world.shapes[item_current_idx]->scale, 0.0f, 4.0f);
		ImGui::SliderFloat("x velocity", &world.shapes[item_current_idx]->exposeVelocity().x, -8.0f, 8.0f);
		ImGui::SliderFloat("y velocity", &world.shapes[item_current_idx]->exposeVelocity().y, -8.0f, 8.0f);
		ImGui::ColorEdit3("color", world.shapes[item_current_idx]->colorArray);
		ImGui::InputText("name", &world.shapes[item_current_idx]->nameString);
		ImGui::End();

		ImGui::SFML::Render(world.window);
		world.window.display();
		world.window.clear();
	}

	// free memory from shapes
	for (Shape* s : world.shapes)
	{
		delete s;
	}
	world.shapes.clear();

	return 0;
}