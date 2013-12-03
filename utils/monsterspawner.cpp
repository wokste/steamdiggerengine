/*
The MIT License (MIT)

Copyright (c) 2013, Steven Wokke

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "src/utils/monsterspawner.h"

#include "src/entities/monster.h"
#include "src/entities/player.h"
#include "src/world.h"
#include "src/game.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Monster* SpawnConfig::getMonsterType(){
	std::discrete_distribution<> monsterOdds(probs.begin(), probs.end());
	unsigned int index = monsterOdds(GameGlobals::rnd);
	if (index >= prototypes.size())
		return nullptr;

	return prototypes[index].get();
}

SpawnConfig::SpawnConfig(){
	pugi::xml_document doc;
	auto result = doc.load_file(GameGlobals::fileSystem.fullpath("monsters.xml").c_str());
	if (result){
		auto spawnConfigNode = doc.first_child();
		newWaveChance = spawnConfigNode.attribute("long-delay-odd").as_double();
		delayWaves = spawnConfigNode.attribute("long-delay").as_double();
		delaySpawns = spawnConfigNode.attribute("short-delay").as_double(5);
		maxMonsters = spawnConfigNode.attribute("max-monsters").as_int(5);

		// TODO: Add more properties
		for(auto monsterNode : spawnConfigNode){
			auto stats = new Monster();
			stats->load(monsterNode);
			prototypes.push_back(std::move(std::unique_ptr<Monster>(stats)));
			probs.push_back(monsterNode.attribute("odds-weight").as_double());
		}
	} else {
		std::cerr << result.description();
	}
}

MonsterSpawner::MonsterSpawner() : basicSpawnConfig()
{
	std::poisson_distribution<> poisson(1);
	SpawnConfig& spawnConfig = basicSpawnConfig;
	cooldown.set(poisson(GameGlobals::rnd) * spawnConfig.delayWaves);
}

void MonsterSpawner::logic(World& world, double time){
	cooldown -= time;

	if (cooldown.done()){
		// TODO: Have a different spawn config for day and night
		SpawnConfig& spawnConfig = basicSpawnConfig;
		std::vector<Creature*> spawnAround;

		// Step 1: Find the creatures it should spawn around.
		for (auto test : world.creatures()){
			if (test->isPlayer){
				// This is a creature on the player team and monsters should be spawned around it
				int numMonsters = 0;
				for (auto other : world.creatures()){
					if (other->aggressiveTo(test) && Vector2::lengthTo(other->pos, test->pos) < despawnRadius){
						numMonsters++;
						// TODO: Optimize
					}
				}
				if (numMonsters < spawnConfig.maxMonsters)
					spawnAround.push_back(test);
			} else {
				// Testing whether this creature should be deleted
				bool playerNear = false;
				for (auto other : world.creatures()){
					if (other->aggressiveTo(test) && Vector2::lengthTo(other->pos, test->pos) < despawnRadius){
						playerNear = true;
						break;
					}
				}
				if (!playerNear){
					world.entities->remove(test);
				}
			}
		}

		// Step 2: Spawn around those creatures
		for(auto player: spawnAround){
			for (int attempt = 0; attempt < spawnAttempts; ++attempt ){
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

bool MonsterSpawner::validSpawnPos(Vector2d& desiredPosition, World& world, Creature& monster){
	Vector2d offset(0,4);

	if (!monster.validPos(world, desiredPosition) || monster.validPos(world, desiredPosition + offset))
		return false;

	return true;
}

bool MonsterSpawner::trySpawn(World& world, Creature* player){
	Vector2d spawnPos = Vector2::randomize(player->pos, 28, 32);

	// TODO: randomly chosen biome dependant mobs
	Monster* typeToSpawn = basicSpawnConfig.getMonsterType();
	if (!typeToSpawn || !validSpawnPos(spawnPos, world, *typeToSpawn))
		return false;



	Monster* spawned = world.spawn(*typeToSpawn, spawnPos);

	if (spawned)
		spawned->target = player;

	return (spawned != nullptr);
}
