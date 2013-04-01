#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>
#include "../utils/vector2.h"

class Entity;
class EntityStats;
class EntityList;
class EntityHandle;

struct ListItem{
	Entity*	  m_pEntry;
	unsigned int m_nr;

	inline bool exists(){
		return m_nr != 0;
	}
};

class EntityList{
	friend EntityHandle;

	// Resource manager stuff
	std::map<std::string,EntityStats *> m_stats;
public:
	EntityStats * load(std::string type);

	// EntityList Stuff
private:
	std::vector<ListItem> m_entities;

public:
	EntityList();
	virtual ~EntityList();
	EntityHandle addEntity(Entity * pE);
	EntityHandle findMax(std::function<double (const Entity&)> func);
	bool areaHasEntity(Vector2i px1, Vector2i px2);

	void logic(int timeMs);
	void render();
};
