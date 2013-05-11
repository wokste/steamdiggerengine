#include "monsterspawner.h"

#include "../entities/monster.h"
#include "../entities/player.h"
#include "../entities/flyingmonster.h"
#include "../world.h"
#include "../game.h"
#include <cmath>
#include <algorithm>

MonsterSpawner::MonsterSpawner(Game& game){
	ConfigNode::load(game.fileSystem.fullpath("ghost.json"),[&] (ConfigNode& config){
		auto stats = new FlyingMonsterStats();
		stats->load(game, config);
		prototype.reset(stats);
	});
}
void MonsterSpawner::logic(World* world, int timeMs){
	cooldown -= timeMs;
	if (cooldown.done()){
		for(auto& player: world->players){
			int numMonsters = std::count_if(world->monsters.begin(), world->monsters.end(),[&](std::unique_ptr<Monster>& monster) {
				return monster->target == player.get();
			});

			if (numMonsters > 5)
				continue;

			for (int i = 0; i < 50; ++i ){
				if (trySpawn(world, player.get())){
					break;
				}
			}
		}
		cooldown.set(2500);
	}
}

bool MonsterSpawner::trySpawn(World* world, Player* player){
	FlyingMonsterStats* typeToSpawn = prototype.get();

	double distance = 12 + (double)rand() / RAND_MAX;
	double direction = (double)rand() / RAND_MAX * 6.283184;
	Vector2d spawnPos = player->pos + Vector2d(std::sin(direction) * distance, std::cos(direction) * distance);
	Monster* spawned = world->spawn(typeToSpawn, spawnPos);

	if (spawned)
		spawned->target = player;

	return (spawned != nullptr);
}
