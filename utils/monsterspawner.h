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
	int delayWaves;
	int delaySpawns;
	int maxMonsters;
private:
	std::unique_ptr<FlyingMonsterStats> prototype;
};

class MonsterSpawner{
public:
	MonsterSpawner(Game& game);
	void logic(World* world, double time);

private:
	Cooldown cooldown;
	SpawnConfig basicSpawnConfig;
	bool trySpawn(World* world, Player* player);
};
