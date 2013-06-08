#pragma once
#include <vector>
#include <memory>

class Entity;
class Screen;
class Effect;
class ConfigNode;

class ItemType{
public:
	ItemType(ConfigNode& array);
	bool use(Entity& owner, const Screen& screen);

	bool stackable;
	bool consumable;
	double useTime;
	std::vector<std::unique_ptr<Effect>> effects;
};
