#pragma once
#include "utils/confignode.h"

struct Attack{
	int damage;
	int push;
	int type;

	void load(ConfigNode& config);

	Attack();
	Attack(const Attack& that) = delete;
};
