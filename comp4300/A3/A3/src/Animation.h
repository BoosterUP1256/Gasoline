#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "Vec2.h"

class Animation
{
public:
	Animation() = default;
	Animation(const std::string& name, const sf::Texture& t);
	Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed);

	void update();
	const Vec2& getSize() const;
	const std::string& getName() const;
	sf::Sprite& getSprite();
	bool hasEnded() const;

private:
	std::string m_name;
	sf::Sprite m_sprite;
	size_t		m_frameCount;
	size_t      m_currentFrame;
	size_t      m_speed;
	Vec2        m_size;
};