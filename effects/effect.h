#pragma once
#include "../utils/vector2.h"
#include <memory>

class Entity;
class Screen;
class ConfigNode;

class Effect{
public:
	Effect(){};
	~Effect(){};
	virtual bool use(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer) = 0;
	static std::unique_ptr<Effect> loadEffect(ConfigNode& node);
};
