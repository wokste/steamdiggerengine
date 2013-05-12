#include "monsterspawner.h"

#include "../entities/monster.h"
#include "../entities/player.h"
#include "../entities/flyingmonster.h"
#include "../world.h"
#include "../game.h"
#include <cmath>
#include <algorithm>

MonsterSpawner::MonsterSpawner(Game& game) : basicSpawnConfig(game)
{
}

FlyingMonsterStats* SpawnConfig::getMonsterType(){
	return prototype.get();
}

SpawnConfig::SpawnConfig(Game& game){
	ConfigNode::load(game.fileSystem.fullpath("ghost.json"),[&] (ConfigNode& config){
		auto stats = new FlyingMonsterStats();
		stats->load(game, config);
		prototype.reset(stats);
	});

	newWaveChance = 0.4;
	delayWaves = 15;
	delaySpawns = 1;
	maxMonsters = 5;
}

void MonsterSpawner::logic(World* world, double time){
	cooldown -= time;
	std::poisson_distribution<> poisson(1);

	if (cooldown.done()){
		// TODO: Have a different spawn config for day and night
		SpawnConfig& spawnConfig = basicSpawnConfig;
		for(auto& player: world->players){
			int numMonsters = std::count_if(world->monsters.begin(), world->monsters.end(),[&](std::unique_ptr<Monster>& monster) {
				return monster->target == player.get();
			});
			if (numMonsters >= spawnConfig.maxMonsters)
				continue;

			for (int attempt = 0; attempt < 50; ++attempt ){
				if (trySpawn(world, player.get())){
					numMonsters++;
					break;
				}
			}
		}
		if (std::bernoulli_distribution(spawnConfig.newWaveChance)(world->game->rnd))
			cooldown.set(poisson(world->game->rnd) * spawnConfig.delayWaves);
		else
			cooldown.set(poisson(world->game->rnd) * spawnConfig.delaySpawns);
	}
}

bool MonsterSpawner::trySpawn(World* world, Player* player){
	double distance = 12 + (double)rand() / RAND_MAX;
	double direction = (double)rand() / RAND_MAX * 6.283184;
	Vector2d spawnPos = player->pos + Vector2d(std::sin(direction) * distance, std::cos(direction) * distance);

	// TODO: randomly chosen biome dependant mobs
	FlyingMonsterStats* typeToSpawn = basicSpawnConfig.getMonsterType();
	Monster* spawned = typeToSpawn->spawn(world, spawnPos);

	if (spawned)
		spawned->target = player;

	return (spawned != nullptr);
}
