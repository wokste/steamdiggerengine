#pragma once

#include <string>
#include <memory>
#include <random>
class ItemDefManager;
class World;

class FileSystem{
public:
	FileSystem();

	std::string fullpath(const std::string& resourcename) const;
private:
	std::string dataDir;
};

class Game{
public:
	Game();
	~Game();
	ItemDefManager* itemDefs;
	FileSystem fileSystem;
	std::mt19937 rnd;
};
