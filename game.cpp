#include "game.h"
#include "items/itemdefmanager.h"
#include <iostream>
#include <cstdlib>

std::unique_ptr<ItemDefManager> GameGlobals::itemDefs;
FileSystem GameGlobals::fileSystem;
std::mt19937 GameGlobals::rnd;

void GameGlobals::init(){
	itemDefs.reset(new ItemDefManager(fileSystem.fullpath("tools.json")));
}

FileSystem::FileSystem(){
	dataDir = "data/";
}

std::string FileSystem::fullpath(const std::string& resourcename) const{
	std::cout << dataDir << " + " << resourcename << " \n";
	return dataDir + resourcename;
}
