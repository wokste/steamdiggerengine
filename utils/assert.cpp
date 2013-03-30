#include "assert.h"

void ASSERT(bool expression, std::string resource, std::string message){
	if (!expression){
		std::cout << "in " << resource << ": " << message << "\n";
		//std::cin.getline();
	}
}
