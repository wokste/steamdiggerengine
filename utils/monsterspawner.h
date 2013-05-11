#pragma once
#include "../cooldown.h"
#include "../entities/flyingmonster.h"
#include <memory>

class World;
class Player;

class MonsterSpawner{
public:
	MonsterSpawner(Game& game);
	void logic(World* world, int timeMs);

private:
	Cooldown cooldown;
	std::unique_ptr<FlyingMonsterStats> prototype;

	bool trySpawn(World* world, Player* player);
};
