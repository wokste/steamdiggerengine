#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>

class Entity;
class EntityList;

struct EntityHandle{
	EntityHandle();
	EntityHandle(unsigned int newNr, unsigned int newIndex);
	unsigned int nr;
	unsigned int index;
	Entity* get(EntityList& list);
};
