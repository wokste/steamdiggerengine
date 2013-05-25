#include "skybox.h"
#include <cmath>
#include <SFML/OpenGL.hpp>
#include "../map/lightingengine.h"

SkyboxStateData::SkyboxStateData(double newStateTime, double newTransitionTime, sf::Color newSkyColor, sf::Color newLightColor, int newNextState) :
	stateTime(newStateTime),
	transitionTime(newTransitionTime),
	skyColor(newSkyColor),
	lightColor(newLightColor),
	nextState(newNextState)
{}

Skybox::Skybox(){
	states.push_back(SkyboxStateData(20.0,5.0,sf::Color(50,100,200),sf::Color(200,200,150),1));
	states.push_back(SkyboxStateData(20.0,5.0,sf::Color(0,0,0), sf::Color(20,20,50),0));
	currentState = 0;
	timeToNextState = states[currentState].stateTime;
}

void Skybox::logic(double time){
	timeToNextState -= time;
	while (timeToNextState < 0){
		currentState = states[currentState].nextState;
		timeToNextState += states[currentState].stateTime;
	}
	sf::Color skyColor;

	if (timeToNextState > states[currentState].transitionTime){
		lightColor = states[currentState].lightColor;
		skyColor = states[currentState].skyColor;
	} else {
		double fraction = timeToNextState/states[currentState].transitionTime;
		lightColor = states[currentState].lightColor * fraction + states[states[currentState].nextState].lightColor * (1 - fraction);
		skyColor = states[currentState].skyColor * fraction + states[states[currentState].nextState].skyColor * (1 - fraction);
	}
	glClearColor(skyColor.r / 255.0, skyColor.g / 255.0, skyColor.b / 255.0, 1.0);
	/*

	*/
}

void Skybox::render(){

}
