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
#pragma once
#include "../cooldown.h"
#include <memory>
#include <vector>

class World;
class Creature;
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
	std::vector<double> probs;
};

class MonsterSpawner{
public:
	MonsterSpawner();
	void logic(World* world, double time);

private:
	Cooldown cooldown;
	SpawnConfig basicSpawnConfig;
	bool trySpawn(World* world, Creature* player);
	static constexpr int despawnRadius = 48;
	static constexpr int spawnRadiusMin = 28;
	static constexpr int spawnRadiusMax = 32;
	static constexpr int spawnAttempts = 256;
};
