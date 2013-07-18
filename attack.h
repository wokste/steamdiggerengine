#pragma once
#include "utils/confignode.h"

struct Attack{
	int damage;
	int push;
	int type;

	void load(const ConfigNode& config);

	Attack();
};
