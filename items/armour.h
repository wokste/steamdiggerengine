#pragma once
#include "itemdef.h"
#include <string>

class Armour : public ItemDef{
public:
	Armour();
	virtual void load(ConfigNode& config);
};
