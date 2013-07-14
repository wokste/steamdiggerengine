#pragma once
#include "../utils/vector2.h"
#include <memory>

class Entity;
class Screen;
class ConfigNode;
class Game;

class Effect{
public:
	Effect(){};
	~Effect(){};
	virtual bool run(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer) = 0;
	static std::unique_ptr<Effect> loadEffect(ConfigNode& node, const Game& game);
};

class EffectSlot{
public:
	EffectSlot();
	~EffectSlot();
	void load(ConfigNode& node, const Game& game);
	bool run(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer);
	void add(std::unique_ptr<Effect> effect);
private:
	std::vector<std::unique_ptr<Effect>> effects;
};
