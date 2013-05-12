#pragma once
#include "../cooldown.h"
#include "../entities/flyingmonster.h"
#include <memory>

class World;
class Player;

class SpawnConfig{
public:
	SpawnConfig(Game& game);
	FlyingMonsterStats* getMonsterType();
	double newWaveChance;
	int msDelayWaves;
	int msDelaySpawns;
	int maxMonsters;
private:
	std::unique_ptr<FlyingMonsterStats> prototype;
};

class MonsterSpawner{
public:
	MonsterSpawner(Game& game);
	void logic(World* world, int timeMs);

private:
	Cooldown cooldown;
	SpawnConfig basicSpawnConfig;
	bool trySpawn(World* world, Player* player);
};
