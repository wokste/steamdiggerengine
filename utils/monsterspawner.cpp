#include "monsterspawner.h"

#include "../entities/monster.h"
#include "../entities/player.h"
#include "../world.h"
#include "../game.h"
#include <cmath>
#include <algorithm>

MonsterStats* SpawnConfig::getMonsterType(){
	std::discrete_distribution<> monsterOdds({40, 10});
	unsigned int index = monsterOdds(GameGlobals::rnd);
	if (index >= prototypes.size())
		return nullptr;

	return prototypes[index].get();
}

SpawnConfig::SpawnConfig(){
	ConfigNode::load(GameGlobals::fileSystem.fullpath("monsters.json"),[&] (ConfigNode& configFile){
		configFile.forEachNode([&] (ConfigNode& monsterConfig) {
			auto stats = new MonsterStats();
			stats->load(monsterConfig);
			prototypes.push_back(std::move(std::unique_ptr<MonsterStats>(stats)));
		});
	});

	newWaveChance = 0.4;
	delayWaves = 15;
	delaySpawns = 1;
	maxMonsters = 5;
}

MonsterSpawner::MonsterSpawner() : basicSpawnConfig()
{
}

void MonsterSpawner::logic(World* world, double time){
	cooldown -= time;
	if (cooldown.done()){
		std::poisson_distribution<> poisson(1);
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
		if (std::bernoulli_distribution(spawnConfig.newWaveChance)(GameGlobals::rnd))
			cooldown.set(poisson(GameGlobals::rnd) * spawnConfig.delayWaves);
		else
			cooldown.set(poisson(GameGlobals::rnd) * spawnConfig.delaySpawns);
	}
}

bool MonsterSpawner::trySpawn(World* world, Player* player){
	std::uniform_real_distribution<> distanceRandomizer(28,32);
	std::uniform_real_distribution<> directionRandomizer(0,6.283184);
	double distance = distanceRandomizer(GameGlobals::rnd);
	double direction = directionRandomizer(GameGlobals::rnd);
	Vector2d spawnPos = player->pos + Vector2d(std::sin(direction) * distance, std::cos(direction) * distance);

	// TODO: randomly chosen biome dependant mobs
	MonsterStats* typeToSpawn = basicSpawnConfig.getMonsterType();
	if (typeToSpawn == nullptr) return false;
	Monster* spawned = typeToSpawn->spawn(world, spawnPos);

	if (spawned)
		spawned->target = player;

	return (spawned != nullptr);
}
