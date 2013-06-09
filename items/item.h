#pragma once
#include <vector>
#include <memory>

class Entity;
class Screen;
class Effect;
class ConfigNode;

class ItemType{
public:
	ItemType(ConfigNode& config);
	~ItemType();
	bool use(Entity& owner, const Screen& screen);

	int maxStack;
	bool consumable;
	double useTime;
	std::vector<std::unique_ptr<Effect>> onUseEffects;
};
