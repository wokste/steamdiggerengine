#include "entitylist.h"
#include "entity.h"
#include "flyingmonster.h"
#include "player.h"
#include "projectile.h"
#include "../utils/assert.h"
#include <tuple>
#include <iostream>

extern std::string dataDirectory;

EntityList::EntityList(){
	m_entities.resize(256);
}

EntityList::~EntityList(){
	for (auto e : m_entities){
		if (e.exists()){
			delete e.m_pEntry;
		}
	}
	for (auto s : m_stats){
		delete s.second;
	}
}

EntityHandle EntityList::findMax(std::function<double (const Entity&)> func){
	double curMax = 0;
	EntityHandle curHandle(0,0);

	auto numEntities = m_entities.size();
	for (unsigned int i = 0 ; i < numEntities; ++i){
		if (m_entities[i].exists()){
			double eMax = func(*m_entities[i].m_pEntry);
			if (eMax > curMax){
				curMax = eMax;
				curHandle.m_index = i;
				curHandle.m_nr = m_entities[i].m_nr;
			}
		}
	}
	return curHandle;
}

EntityHandle EntityList::addEntity(Entity * pE){
	static int autoIncrementNumber = 1;

	auto numEntities = m_entities.size();
	for (unsigned int i = 0 ; i < numEntities; ++i){
		if (!m_entities[i].exists()){
			m_entities[i].m_pEntry = pE;
			m_entities[i].m_nr = ++autoIncrementNumber;
			return EntityHandle(i, m_entities[i].m_nr);
		}
	}

	ListItem listItem;
	listItem.m_nr = ++autoIncrementNumber;
	listItem.m_pEntry = pE;
	m_entities.push_back(listItem);
	return EntityHandle(numEntities, m_entities[numEntities].m_nr);
}

EntityStats * EntityList::load(std::string fileName){
	auto it = m_stats.find(fileName);
	if (it != m_stats.end())
		return it->second;

	EntityStats * stat = nullptr;

	std::cout << "loading " << fileName << "\n";

	ConfigNode config = config.load(fileName);

	std::string className = config.getString("class");

#define OPTION(str,class) if (className == str) {stat = new class ();}
	//OPTION("entity",EntityStats)
	OPTION("player",PlayerStats)
	OPTION("projectile",ProjectileStats)
	//OPTION("monster",MonsterStats)
	OPTION("flyingmonster",FlyingMonsterStats)
#undef OPTION

	ASSERT(stat, fileName, "Unknown class " + className);
	if (stat != nullptr)
		stat->load(config);

	//input >> value;

	m_stats[fileName] = stat;
	return stat;
}

void EntityList::logic(int timeMs){
	//std::vector<Entity *> toDelete;

	// == Basic Logic ==
	auto numEntities = m_entities.size();
	for (unsigned int i = 0 ; i < numEntities; ++i){
		if (m_entities[i].exists()){
			m_entities[i].m_pEntry->logic(timeMs);
			if (m_entities[i].m_pEntry->bDeleteMe){
				delete m_entities[i].m_pEntry;
				m_entities[i].m_pEntry = nullptr;
				m_entities[i].m_nr = 0;
			}
		}
	}

	// == Check Collisions ==
	for (unsigned int i = 0 ; i < numEntities; ++i){
		if (m_entities[i].exists()){
			for (unsigned int j = 0 ; j < numEntities; ++j){
				if (m_entities[j].exists() && m_entities[i].m_pEntry->checkCollision(*m_entities[j].m_pEntry)){
					m_entities[i].m_pEntry->onCollision(*m_entities[j].m_pEntry);
					m_entities[j].m_pEntry->onCollision(*m_entities[i].m_pEntry);
				}
			}
		}
	}
}

void EntityList::render(){
	for (auto entity : m_entities){
		if (entity.exists()){
			entity.m_pEntry->render();
		}
	}
}

