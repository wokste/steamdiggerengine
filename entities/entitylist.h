#pragma once
#include "entity.h"
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <memory>
#include "../utils/vector2.h"

class Entity;
class EntityStats;
class EntityList;
class EntityHandle;

struct ListItem{
	ListItem() {entity = nullptr;}
	ListItem(unsigned int nr, Entity* newEntity);
	~ListItem() {}

	Entity* entity;
	unsigned int nr;

	inline bool exists(){
		return nr != 0;
	}
};

class EntityList{
	friend EntityHandle;

	// EntityList Stuff
private:
	std::vector<ListItem> entities;
	void foreach(std::function<void (int id, ListItem&)> func);

public:
	EntityList();
	~EntityList();
	EntityHandle addEntity(Entity * pE);
	EntityHandle findMax(std::function<double (const Entity&)> func);
	bool areaHasEntity(Vector2i px1, Vector2i px2);

	void logic(int timeMs);
	void render();
};
