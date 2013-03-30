#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>

class Entity;
class EntityList;

struct EntityHandle{
	EntityHandle();
	EntityHandle(unsigned int nr, unsigned int index);
	unsigned int m_nr;
	unsigned int m_index;
	Entity* get(EntityList& list);
};
