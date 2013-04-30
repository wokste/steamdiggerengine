#include "game.h"
#include "items/itemdefmanager.h"
#include <iostream>
#include <cstdlib>

Game::Game(){
	itemDefs = new ItemDefManager(*this, fileSystem.fullpath("items.json"));
}

Game::~Game(){
	delete itemDefs;
}

FileSystem::FileSystem(){
#if defined _WIN32 or defined _WIN64
	char* userprofile = getenv("USERPROFILE");
	dataDir = (userprofile ? std::string(userprofile) : "~/") + "/.steamdigger/data/";
#elif defined _LINUX or defined __APPLE__ or defined __unix
	char* home = getenv("HOME");
	dataDir = (home ? std::string(home) : "~") + "/.steamdigger/data/";
#else
	#error This operating system is currently not supported
#endif
}

std::string FileSystem::fullpath(const std::string& resourcename) const{
	std::cout << dataDir << " + " << resourcename << " \n";
	return dataDir + resourcename;
}
