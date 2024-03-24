#include "Scene_Play.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"

#include <iostream>

Scene_Play::Scene_Play(GameEngine* gameEngine, const std::string& levelPath)
	: Scene(gameEngine)
	, m_levelPath(levelPath)
{
	init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
	registerAction(sf::Keyboard::P,		 "PAUSE");
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::T,	     "TOGGLE_TEXTURE");		// toggle drawing (T)extures
	registerAction(sf::Keyboard::C,      "TOGGLE_COLLISION");   // toggle drawing (C)ollision Boxes
	registerAction(sf::Keyboard::G,      "TOGGLE_GRID");        // toggle drawing (G)rid 

	// TODO: Register all other gameplay Actions

	m_gridText.setCharacterSize(12);
	m_gridText.setFont(m_game->assets().getFont("Tech"));

	loadLevel(levelPath);
}
