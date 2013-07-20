#pragma once
#include "entity.h"

class DroppedItem : public Entity{
public:
	DroppedItem(int itemId, int count = 1);
	virtual ~DroppedItem();
	virtual void logic(double time);

	double TTL; // Time to live in seconds
	int itemId;
	int count;
};
