#pragma once

#include <string>
#include <memory>
#include <random>
class ItemDefManager;
class World;
class BlockTypeManager;

class FileSystem{
public:
	FileSystem();

	std::string fullpath(const std::string& resourcename) const;
private:
	std::string dataDir;
};

class GameGlobals{
public:
	static void init();
	static std::unique_ptr<ItemDefManager> itemDefs;
	static FileSystem fileSystem;
	static std::mt19937 rnd;
	static std::unique_ptr<BlockTypeManager> blockDefs;
};
