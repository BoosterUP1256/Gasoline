#include "Game.h"

#include <iostream>
#include <fstream>
#include <random>
#include <numbers>

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	// TODO: read in config file here
	//       use the premade PlayerConfig, EnemyConfig, BulletConfig variables
	std::ifstream fin(path);
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
			fin >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.FL >> m_windowConfig.FS;
		}

		if (token == "Font")
		{
			fin >> m_fontConfig.F;
			if (!m_font.loadFromFile(m_fontConfig.F)) 
			{
				std::cerr << "Failed to load font!\n";
				exit(-1);
			}
			fin >> m_fontConfig.S >> m_fontConfig.R >> m_fontConfig.G >> m_fontConfig.B;
		}

		if (token == "Player")
		{
			fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB;
			fin >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
		}

		if (token == "Enemy")
		{
			fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR;
			fin >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
		}

		if (token == "Bullet")
		{
			fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR;
			fin >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
		}
	}
	
	// set up default window parameters
	if (m_windowConfig.FS)
	{
		m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), "A2", sf::Style::Close | sf::Style::Titlebar | sf::Style::Fullscreen);
	}
	else
	{
		m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), "A2", sf::Style::Close | sf::Style::Titlebar);
	}

	// set up text for score drawing
	m_text.setFont(m_font);
	m_text.setCharacterSize(m_fontConfig.S);
	m_text.setFillColor(sf::Color(m_fontConfig.R, m_fontConfig.G, m_fontConfig.B));
	m_text.setPosition(0.0f, 0.0f);

	m_window.setFramerateLimit(m_windowConfig.FL);

	ImGui::SFML::Init(m_window);

	spawnPlayer();

	// =============== TEST SPAWNING =========================================================================
	//auto entity = m_entities.addEntity("enemy");
	//entity->cTransform = std::make_shared<CTransform>(Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f), 0.0f);
	//entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color::Yellow, sf::Color(255, 0, 0), 4.0f);
	//entity->cInput = std::make_shared<CInput>();

	//spawnEnemy();
	//========================================================================================================
}

void Game::run()
{
	// TODO: add pause functionality in here
	//		 some systems should function while paused (rendering)
	//       some systems shouldn't (movement / input)
	while (m_running)
	{
		if (m_paused)
		{
			ImGui::SFML::Update(m_window, m_deltaClock.restart());
			if (m_guiOn) { sGui(); };
			sUserInput();
			if (m_renderingOn)
			{
				sRender();
			}
			else // so abort is not triggered by ImGui
			{
				ImGui::SFML::Render(m_window);
			}
		}
		else
		{
			// update the entity manager
			m_entities.update();

			// required update call to imgui
			ImGui::SFML::Update(m_window, m_deltaClock.restart());

			if (m_spawningOn) { sEnemySpawner(); }
			if (m_movementOn) { sMovement(); }
			if (m_lifespanOn) { sLifespan(); }	// Why did Dave never call this?
			if (m_collisionOn) { sCollision(); };
			sUserInput();
			if (m_guiOn) { sGui(); };

			if (m_renderingOn)
			{
				sRender();
			}
			else // so abort is not triggered by ImGui
			{
				ImGui::SFML::Render(m_window);
			}

			// increment the current frame
			// may need to be moved when pause implemented
			m_currentFrame++;
		}
	}
}

void Game::setPaused(bool paused)
{
	// TODO
	m_paused = paused;
}

// respawn the player in the middle of the screen
void Game::spawnPlayer()
{
	// TODO: Finish adding all properties of the player with the correct values from the config
	
	// We create every entity by calling EntityManager.addEntity(tag)
	// This returns a std::shared_ptr<Entity>, so we can use 'auto' to save typing
	auto entity = m_entities.addEntity("player");

	// Give this entity a transform so it spawns at (200,200) with velocity (1,1) and angle 0
	entity->cTransform = std::make_shared<CTransform>(Vec2(m_windowConfig.W/2, m_windowConfig.H/2), Vec2(0.0f, 0.0f), 0.0f);

	// The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

	// Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();

	// Since we want this Entity to be our player, set our Game's player variable to be this Entity
	// This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it
	m_player = entity;
}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
	// TODO: make sure the enemy is spawned properly with the m_enemyConfig variables
	//		 the enemy must be spawned completely within the bounds of the window
	//
	//int xPos = randInt()

	auto entity = m_entities.addEntity("enemy");
	entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);

	int points = randInt(3, 8);
	int fr = randInt(0, 255);
	int fg = randInt(0, 255);
	int fb = randInt(0, 255);
	entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, points, sf::Color(fr, fg, fb), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);

	int posx = randInt(m_enemyConfig.SR, m_windowConfig.W - m_enemyConfig.SR);
	int posy = randInt(m_enemyConfig.SR, m_windowConfig.H - m_enemyConfig.SR);
	int speed = randInt(m_enemyConfig.SMIN, m_enemyConfig.SMAX);
	float xdir = randFloat(-1.0f, 1.0f);
	float ydir = randFloat(-1.0f, 1.0f);
	entity->cTransform = std::make_shared<CTransform>(Vec2(posx, posy), Vec2(xdir * speed, ydir * speed), speed);

	entity->cScore = std::make_shared<CScore>(points * 100);

	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

// spawns the small enemies when a big one (input entity e) explodes
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
	// TODO: spawn small enemies at the location of the input enemy e

	// when we create the smaller enemy, we have to read the values of the original enemy
	// - spawn a number of small enemies equal to the vertices of the original enemy
	// - set each small enemy to the same color as the original, half the size
	// - small enemies are worth double points of the original enemy

	float spreadInterval = 360.0f / e->cShape->circle.getPointCount();
	spreadInterval = spreadInterval * (std::numbers::pi / 180.0f);

	for (int i = 0; i < e->cShape->circle.getPointCount(); i++)
	{
		auto small = m_entities.addEntity("small");
		small->cScore = std::make_shared<CScore>(e->cShape->circle.getPointCount() * 100 * 2);
		small->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
		small->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR / 2);
		small->cShape = std::make_shared<CShape>(m_enemyConfig.SR / 2, e->cShape->circle.getPointCount(), e->cShape->circle.getFillColor(), e->cShape->circle.getOutlineColor(), m_enemyConfig.OT / 2);

		// Calculate new velocity
		Vec2 velocity(cosf(i * spreadInterval), sinf(i * spreadInterval));
		small->cTransform = std::make_shared<CTransform>(e->cTransform->pos, velocity, e->cTransform->angle);
	}
}

// spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	// TODO: implement the spawning of a bullet which travels toward target
	//		 - bullet speed is given as a scalar speed
	//       - you must set the velocity by using formula in notes

	float distance = entity->cTransform->pos.dist(target);
	Vec2 difference = target - entity->cTransform->pos;
	float angle = atan2f(difference.y, difference.x);
	Vec2 velocity(m_bulletConfig.S * cosf(angle), m_bulletConfig.S * sinf(angle));

	auto bullet = m_entities.addEntity("bullet");
	bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
	bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
	bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
	bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, velocity, angle);	// NOTE: Still not entirly sure why I need to put the angle in here
	//auto bullet = m_entities.addEntity("bullet");
	//bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
	//bullet->cTransform = std::make_shared<CTransform>(target, Vec2(0, 0), 0);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO: implement your own special weapon
}

void Game::sMovement()
{
	// TODO: implement all entity movement in this function
	//       you should read the m_player->cInput component to determine if the player is moving

	// Sample movement speed update
	//m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	//m_player->cTransform->pos.y += m_player->cTransform->velocity.y;

	for (auto& entity : m_entities.getEntities())
	{
		Vec2 pos = entity->cTransform->pos;
		float radius = entity->cShape->circle.getRadius();

		if (entity->tag() == "enemy")
		{
			if ((pos.x - radius < 0.0f) || (pos.x + radius > m_windowConfig.W))
			{
				entity->cTransform->velocity.x *= -1.0f;
			}

			if ((pos.y - radius < 0.0f) || (pos.y + radius > m_windowConfig.H))
			{
				entity->cTransform->velocity.y *= -1.0f;
			}
		}

		if (entity->cInput != nullptr)
		{
			if (entity->cInput->up && (pos.y - radius > 0.0f))
			{ 
				entity->cTransform->velocity.y = -1.0f; 
			}
			else if (entity->cInput->down && (pos.y + radius < m_windowConfig.H))
			{ 
				entity->cTransform->velocity.y = 1.0f; 
			}
			else
			{
				entity->cTransform->velocity.y = 0.0f;
			}

			if (entity->cInput->left && (pos.x - radius > 0.0f))
			{
				entity->cTransform->velocity.x = -1.0f;
			}
			else if (entity->cInput->right && (pos.x + radius < m_windowConfig.W))
			{
				entity->cTransform->velocity.x = 1.0f;
			}
			else
			{
				entity->cTransform->velocity.x = 0.0f;
			}

			// normalize the vector
			entity->cTransform->velocity.normalize();
			entity->cTransform->velocity *= m_playerConfig.S;
		}

		entity->cTransform->pos.x += entity->cTransform->velocity.x;
		entity->cTransform->pos.y += entity->cTransform->velocity.y;
	}
}

void Game::sLifespan()
{
	// TODO: implement all lifespan functionality
	//
	// for all entities
	//	   if entity has no lifespan component, skip it
	//     if entity has > 0 remaining lifespan, subtract 1
	//     if it has lifespan and is alive
	//         scale its alpha channel properly
	//     if it has lifespan and its time is up
	//         destroy the entity

	for (auto& entity : m_entities.getEntities())
	{
		if (entity->cLifespan != nullptr)
		{
			if (entity->cLifespan->remaining > 0)
			{
				entity->cLifespan->remaining--;
				// TODO: scale alpha channel properly
				sf::Color currentColorF = entity->cShape->circle.getFillColor();
				sf::Color currentColorO = entity->cShape->circle.getOutlineColor();
				float bruh = (static_cast<float>(entity->cLifespan->remaining) / static_cast<float>(entity->cLifespan->total)) * 255.0f;
				sf::Color newColorF(currentColorF.r, currentColorF.g, currentColorF.b, bruh);
				sf::Color newColorO(currentColorO.r, currentColorO.g, currentColorO.b, bruh);
				entity->cShape->circle.setFillColor(newColorF);
				entity->cShape->circle.setOutlineColor(newColorO);
			}
			else
			{
				entity->destroy();
			}
		}
	}
}

void Game::sCollision()
{
	// TODO: implement all proper collision between entities
	//       be sure to use the collision radius, NOT the shape radius
	for (auto bullet : m_entities.getEntities("bullet"))
	{
		for (auto enemy : m_entities.getEntities("enemy"))
		{
			if (areColliding(bullet, enemy))
			{
				m_score += enemy->cScore->score;
				bullet->destroy();
				spawnSmallEnemies(enemy);
				enemy->destroy();
			}
		}

		for (auto small : m_entities.getEntities("small"))
		{
			if (areColliding(bullet, small))
			{
				m_score += small->cScore->score;
				bullet->destroy();
				small->destroy();
			}
		}
	}

	for (auto enemy : m_entities.getEntities("enemy"))
	{
		if (areColliding(m_player, enemy))
		{
			m_player->destroy();
			spawnPlayer();
		}
	}

	for (auto small : m_entities.getEntities("small"))
	{
		if (areColliding(m_player, small))
		{
			m_player->destroy();
			spawnPlayer();
		}
	}
}

void Game::sEnemySpawner()
{
	// TODO: code which implements enemy spawning should go here
	if (m_currentFrame - m_lastEnemySpawnTime >= m_enemyConfig.SI)
	{
		spawnEnemy();
	}
}

void Game::showEntityTable(const std::string& name, const EntityVec& entityVec)
{
	ImGui::BeginTable(name.c_str(), 4, ImGuiTableFlags_SizingStretchProp);
	for (int row = 0; row < entityVec.size(); row++)
	{
		ImGui::TableNextRow();
		for (int column = 0; column < 4; column++)
		{
			ImGui::TableSetColumnIndex(column);
			switch (column)
			{
			case 0:
			{
				const sf::Color& color = entityVec[row]->cShape->circle.getFillColor();
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f));
				std::string label = "D##" + std::to_string(row);
				if (ImGui::Button(label.c_str()))
				{
					entityVec[row]->destroy();
				}
				ImGui::PopStyleColor();
				break;
			}
			case 1:
				ImGui::Text(std::to_string(entityVec[row]->id()).c_str());
				break;
			case 2:
				ImGui::Text(entityVec[row]->tag().c_str());
				break;
			case 3:
			{
				int x = static_cast<int>(entityVec[row]->cTransform->pos.x);
				int y = static_cast<int>(entityVec[row]->cTransform->pos.y);
				std::string coordinates = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
				ImGui::Text(coordinates.c_str());
			}
			}
		}
	}
	ImGui::EndTable();
}

int Game::randInt(int min, int max) const
{
	std::random_device rd;
	std::uniform_int_distribution<int> dist(min, max);

	return dist(rd);
}

float Game::randFloat(float min, float max) const
{
	std::random_device rd;
	std::uniform_real_distribution<float> dist(min, max);

	return dist(rd);
}

bool Game::areColliding(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2)
{
	const Vec2& e1Pos = e1->cTransform->pos;
	const Vec2& e2Pos = e2->cTransform->pos;
	float e1Radius = e1->cCollision->radius;
	float e2Radius = e2->cCollision->radius;
	Vec2 diff = e2Pos - e1Pos;
	float distSq = diff.x * diff.x + diff.y * diff.y;

	return distSq < (e1Radius + e2Radius) * (e1Radius + e2Radius);
}

void Game::sGui()
{
	ImGui::Begin("Geometry Wars");

	if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Systems"))
		{
			ImGui::Checkbox("Movement", &m_movementOn);
			ImGui::Checkbox("Lifespan", &m_lifespanOn);
			ImGui::Checkbox("Collision", &m_collisionOn);
			ImGui::Checkbox("Spawning", &m_spawningOn);
			ImGui::Indent();
			ImGui::SliderInt("Spawn Interval", &m_enemyConfig.SI, 1, 500);
			if (ImGui::Button("Manual Spawn"))
			{
				spawnEnemy();
			}
			ImGui::Unindent();
			ImGui::Checkbox("GUI", &m_guiOn);
			ImGui::Checkbox("Rendering", &m_renderingOn);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Entity Manager"))
		{
			if (ImGui::CollapsingHeader("Entities by Tag"))
			{
				ImGui::Indent();
				if (ImGui::CollapsingHeader("bullet"))
				{
					showEntityTable("bulletTable", m_entities.getEntities("bullet"));
				}

				if (ImGui::CollapsingHeader("enemy"))
				{
					showEntityTable("enemyTable", m_entities.getEntities("enemy"));
				}

				if (ImGui::CollapsingHeader("player"))
				{
					showEntityTable("player", m_entities.getEntities("player"));
				}

				if (ImGui::CollapsingHeader("small"))
				{
					showEntityTable("smallTable", m_entities.getEntities("small"));
				}
				ImGui::Unindent();
			}
			if (ImGui::CollapsingHeader("All Entities"))
			{
				showEntityTable("allTable", m_entities.getEntities());
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}

void Game::sRender()
{
	//// TODO: change the code below to draw ALL of the entities
	////       sample drawing of the player Entity that we have created
	//m_window.clear();

	//// set the position of the shape based on the entity's transform->pos
	//m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

	//// set the rotation of the shape based on the entity's transform->angle
	//m_player->cTransform->angle += 1.0f;
	//m_player->cShape->circle.setRotation(m_player->cTransform->angle);

	//// draw the entity's sf::CricleShape
	//m_window.draw(m_player->cShape->circle);

	m_window.clear();

	for (auto& entity : m_entities.getEntities())
	{
		entity->cTransform->angle += 1.0f;
		entity->cShape->circle.setRotation(entity->cTransform->angle);
		entity->cShape->circle.setPosition(entity->cTransform->pos.x, entity->cTransform->pos.y);
		m_window.draw(entity->cShape->circle);
	}

	// draw score text
	std::string score = std::to_string(m_score);
	m_text.setString("Score: " + score);
	m_window.draw(m_text);

	// draw the ui last
	ImGui::SFML::Render(m_window);

	m_window.display();
}

void Game::sUserInput()
{
	// TODO: handle user input here
	//       note that you should only be setting the player's input component variables here
	//       you should not implement the player's movement logic here
	//       the movement system will read the variables you set in this function

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		// pass the event to imgui to be parsed
		ImGui::SFML::ProcessEvent(m_window, event);

		// this event triggers when the window is closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		// this event is triggered when a key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				// TODO: set player's input component "up" to true
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = true;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::Escape:
				m_running = false;
				break;
			case sf::Keyboard::P:
				setPaused(!m_paused);
			default: break;
			}
		}

		// this event is triggered when a key is released
		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				// TODO: set player's input component "up" to false
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = false;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = false;
				break;
			default: break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			// this line ignores mouse events if ImGui is the thing being clicked
			if (ImGui::GetIO().WantCaptureMouse) { continue; }

			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//::cout << "Left Mouse Button Clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")\n";
				// call spawnBullet here
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
			}
		}

		if (event.mouseButton.button == sf::Mouse::Right)
		{
			std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")\n";
			// call spawnSpecialWeapon here
		}
	}
}