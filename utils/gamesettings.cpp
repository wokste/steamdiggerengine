#include "gamesettings.h"
#include "../items/itemdefmanager.h"
#include <iostream>
#include <cstdlib>

GameSettings::GameSettings(){
	dataDir = findDataDir();
	itemDefs = new ItemDefManager(findResource("items.json"));
}

GameSettings::~GameSettings(){
	delete itemDefs;
}

// Idea: http://en.kioskea.net/contents/systemes/variables-environnement.php3
std::string GameSettings::findDataDir(){
#if defined _WIN32 or defined _WIN64
	char* userprofile = getenv("USERPROFILE");
	return (userprofile ? std::string(userprofile) : "~/") + "/.steamdigger/data/";
#elif defined _LINUX or defined __APPLE__ or defined __unix
	char* home = getenv("HOME");
	return (home ? std::string(home) : "~") + "/.steamdigger/data/";
#else
	#error This operating system is currently not supported
#endif
}

std::string GameSettings::findResource(const std::string& resourcename){
	std::cout << dataDir << " + " << resourcename << " \n";
	return dataDir + resourcename;
}
