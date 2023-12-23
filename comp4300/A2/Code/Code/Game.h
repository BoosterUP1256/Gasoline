#pragma once

#include "Entity.h"
#include "EntityManager.h"

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig  { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConig  { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };
struct WindowConfig { int W, H, FL; bool FS; };
struct FontConfig { std::string F; int S, R, G, B; };

class Game
{
	sf::RenderWindow	m_window;			// the window we will draw to
	EntityManager		m_entities;			// vector of entities to maintain
	sf::Font			m_font;				// the font we will use to draw
	sf::Text			m_text;				// the score text to be drawn to the screen
	PlayerConfig		m_playerConfig;
	EnemyConfig			m_enemyConfig;
	BulletConig			m_bulletConfig;
	WindowConfig        m_windowConfig;
	FontConfig          m_fontConfig;
	sf::Clock			m_deltaClock;
	int					m_score = 0;
	int					m_currentFrame = 0;
	int					m_lastEnemySpawnTime = 0;
	bool                m_paused = false;
	bool				m_running = true;
	bool				m_movementOn = true;
	bool				m_lifespanOn = true;
	bool				m_collisionOn = true;
	bool				m_spawningOn = true;
	bool				m_guiOn = true;
	bool				m_renderingOn = true;

	std::shared_ptr<Entity> m_player;

	void init(const std::string& path);
	void setPaused(bool paused);
	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> e);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
	void showEntityTable(const std::string& name, const EntityVec& entityVec);
	int randInt(int min, int max) const;
	float randFloat(float min, float max) const;
	bool areColliding(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2);

	void sMovement();
	void sLifespan();
	void sCollision();
	void sEnemySpawner();
	void sGui();
	void sRender();
	void sUserInput();

public:

	Game(const std::string& config);

	void run();
};