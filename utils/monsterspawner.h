#pragma once
#include "../cooldown.h"
#include <memory>
#include <vector>

class Game;
class World;
class Player;
class Monster;
class MonsterStats;

class SpawnConfig{
public:
	SpawnConfig(Game& game);
	MonsterStats* getMonsterType(Game& game);
	double newWaveChance;
	int delayWaves;
	int delaySpawns;
	int maxMonsters;
private:
	std::vector<std::unique_ptr<MonsterStats>> prototypes;
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
