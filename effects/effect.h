#pragma once
#include <memory>

class Entity;
class Screen;
class ConfigNode;

class Effect{
public:
	virtual bool use(Entity& owner, const Screen& screen) = 0;
	static std::unique_ptr<Effect> loadEffect(ConfigNode& node);
};