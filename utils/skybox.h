#pragma once
#include <SFML/Graphics/Color.hpp>
#include "../cooldown.h"
#include <vector>

namespace sf{
	class Color;
};

struct SkyboxStateData{
	SkyboxStateData(double stateTime, double transitionTime, sf::Color skyColor, sf::Color lightColor, int nextState);
	double stateTime;
	double transitionTime;
	sf::Color lightColor;
	sf::Color skyColor;
	int nextState;
};

class Skybox{
public:
	Skybox();
	void logic(double time);
	void render();
	inline sf::Color getLightColor() const{return lightColor;}
private:
	std::vector<SkyboxStateData> states;
	int currentState;
	double timeToNextState;

	sf::Color lightColor;
};
