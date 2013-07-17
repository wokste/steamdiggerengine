#pragma once
#include <vector>
#include <memory>
#include "../effects/effect.h"

class Entity;
class Screen;
class ConfigNode;

class ItemType{
public:
	ItemType(ConfigNode& config);
	ItemType(int frameID);
	~ItemType();
	bool use(Entity& owner, const Screen& screen);

	int maxStack;
	bool consumable;
	double useTime;
	EffectSlot onUse;
};
