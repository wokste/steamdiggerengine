#include "monsterspawner.h"

#include "../entities/monster.h"
#include "../entities/movementtype.h"
#include "../entities/player.h"
#include "../world.h"
#include "../game.h"
#include <cmath>
#include <algorithm>

Monster* SpawnConfig::getMonsterType(){
	std::discrete_distribution<> monsterOdds({40, 10});
	unsigned int index = monsterOdds(GameGlobals::rnd);
	if (index >= prototypes.size())
		return nullptr;

	return prototypes[index].get();
}

SpawnConfig::SpawnConfig(){
	ConfigNode::load(GameGlobals::fileSystem.fullpath("monsters.json"),[&] (ConfigNode& configFile){
		configFile.forEachNode([&] (ConfigNode& monsterConfig) {
			auto stats = new Monster();
			stats->load(monsterConfig);
			prototypes.push_back(std::move(std::unique_ptr<Monster>(stats)));
		});
	});

	newWaveChance = 0.4;
	delayWaves = 15;
	delaySpawns = 1;
	maxMonsters = 5;
}

MonsterSpawner::MonsterSpawner() : basicSpawnConfig()
{
	std::poisson_distribution<> poisson(1);
	SpawnConfig& spawnConfig = basicSpawnConfig;
	cooldown.set(poisson(GameGlobals::rnd) * spawnConfig.delayWaves);
}

void MonsterSpawner::logic(World* world, double time){
	cooldown -= time;

	if (cooldown.done()){
		// TODO: Have a different spawn config for day and night
		SpawnConfig& spawnConfig = basicSpawnConfig;
		std::vector<Creature*> spawnAround;

		// Step 1: Find the creatures it should spawn around.
		for(auto test: world->creatures){
			if (test->isPlayer){
				// This is a creature on the player team and monsters should be spawned around it
				int numMonsters = std::count_if(world->creatures.begin(), world->creatures.end(),[&](Creature* other) {
					return (other->aggressiveTo(*test) && Vector2::length(other->pos - test->pos) < despawnRadius);
				});
				if (numMonsters >= spawnConfig.maxMonsters)
					continue;
				spawnAround.push_back(test);
			} else {
				// Testing whether this creature should be deleted
				if(std::all_of(world->creatures.begin(), world->creatures.end(),[&](Creature* other) {
					return (!other->aggressiveTo(*test) || Vector2::length(other->pos - test->pos) > despawnRadius);
				})){
					world->removeEntity(test);
				}
			}
		}

		// Step 2: Spawn around those creatures
		for(auto player: spawnAround){
			for (int attempt = 0; attempt < 50; ++attempt ){
				if (trySpawn(world, player)){
					break;
				}
			}
		}

		// Step 3: Set cooldown
		std::poisson_distribution<> poisson(1);
		if (std::bernoulli_distribution(spawnConfig.newWaveChance)(GameGlobals::rnd))
			cooldown.set(poisson(GameGlobals::rnd) * spawnConfig.delayWaves);
		else
			cooldown.set(poisson(GameGlobals::rnd) * spawnConfig.delaySpawns);
	}
}

bool MonsterSpawner::trySpawn(World* world, Creature* player){
	std::uniform_real_distribution<> distanceRandomizer(28,32);
	std::uniform_real_distribution<> directionRandomizer(0,6.283184);
	double distance = distanceRandomizer(GameGlobals::rnd);
	double direction = directionRandomizer(GameGlobals::rnd);
	Vector2d spawnPos = player->pos + Vector2d(std::sin(direction) * distance, std::cos(direction) * distance);

	// TODO: randomly chosen biome dependant mobs
	Monster* typeToSpawn = basicSpawnConfig.getMonsterType();
	if (typeToSpawn == nullptr)
		return false;
	Monster* spawned = world->spawn(*typeToSpawn, spawnPos);

	if (spawned)
		spawned->target = player;

	return (spawned != nullptr);
}
