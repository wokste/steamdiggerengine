#pragma once
#include "../cooldown.h"
#include <memory>
#include <vector>

class World;
class Player;
class Monster;

class SpawnConfig{
public:
	SpawnConfig();
	Monster* getMonsterType();
	double newWaveChance;
	int delayWaves;
	int delaySpawns;
	int maxMonsters;
private:
	std::vector<std::unique_ptr<Monster>> prototypes;
};

class MonsterSpawner{
public:
	MonsterSpawner();
	void logic(World* world, double time);

private:
	Cooldown cooldown;
	SpawnConfig basicSpawnConfig;
	bool trySpawn(World* world, Player* player);
};
