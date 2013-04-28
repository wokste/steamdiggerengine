#pragma once

#include <string>
class ItemDefManager;

class GameSettings{
public:
	GameSettings();
	~GameSettings();
	ItemDefManager* itemDefs;
	std::string findResource(const std::string& resourcename);
private:
	std::string dataDir;
	std::string findDataDir();

};
