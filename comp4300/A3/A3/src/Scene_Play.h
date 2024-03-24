#pragma once

class Scene_Play : public Scene
{
public:
	Scene_Play(GameEngine* gameEngine, const std::string& levelPath);

private:
	void init(const std::string& levelPath);
};