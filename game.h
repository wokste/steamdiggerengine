#pragma once

#include <string>
#include <memory>
class ItemDefManager;
class World;

class FileSystem{
public:
	FileSystem();

	std::string fullpath(const std::string& resourcename);
private:
	std::string dataDir;
};

class Game{
public:
	Game();
	~Game();
	ItemDefManager* itemDefs;
	//World* world;
	FileSystem fileSystem;
};
