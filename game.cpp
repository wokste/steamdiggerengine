#include "game.h"
#include "items/itemdefmanager.h"
#include "map/blocktype.h"
#include <iostream>
#include <cstdlib>

std::unique_ptr<ItemDefManager> GameGlobals::itemDefs;
std::unique_ptr<BlockTypeManager> GameGlobals::blockDefs;
FileSystem GameGlobals::fileSystem;
std::mt19937 GameGlobals::rnd;

void GameGlobals::init(){
	itemDefs.reset(new ItemDefManager(fileSystem.fullpath("tools.json")));
	blockDefs.reset(new BlockTypeManager(fileSystem.fullpath("blocks.json")));
}

FileSystem::FileSystem(){
	dataDir = "C:/Users/wokste/projects/steamdigger/data/";
}

std::string FileSystem::fullpath(const std::string& resourcename) const{
	std::cout << dataDir << " + " << resourcename << " \n";
	return dataDir + resourcename;
}
