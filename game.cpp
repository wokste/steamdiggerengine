#include "game.h"
#include "items/itemdefmanager.h"
#include <iostream>
#include <cstdlib>

Game::Game(){
	itemDefs.reset(new ItemDefManager(*this, fileSystem.fullpath("items.json")));
}

Game::~Game(){
}

FileSystem::FileSystem(){
	dataDir = "data/";
}

std::string FileSystem::fullpath(const std::string& resourcename) const{
	std::cout << dataDir << " + " << resourcename << " \n";
	return dataDir + resourcename;
}
