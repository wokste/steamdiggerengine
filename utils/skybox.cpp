#include "skybox.h"
#include <cmath>
#include <SFML/OpenGL.hpp>
#include "../map/lightingengine.h"

SkyboxStateData::SkyboxStateData(double newStateTime, double newTransitionTime, sf::Color newStateColor, int newNextState) :
	stateTime(newStateTime),
	transitionTime(newTransitionTime),
	stateColor(newStateColor),
	nextState(newNextState)
{}

Skybox::Skybox(){
	states.push_back(SkyboxStateData(20.0,5.0,sf::Color(200,200,150),1));
	states.push_back(SkyboxStateData(20.0,5.0,sf::Color(20,20,50),0));
	currentState = 0;
	timeToNextState = states[currentState].stateTime;
}

void Skybox::logic(double time){
	timeToNextState -= time;
	while (timeToNextState < 0){
		currentState = states[currentState].nextState;
		timeToNextState += states[currentState].stateTime;
	}
	if (timeToNextState > states[currentState].transitionTime){
		currentLight = states[currentState].stateColor;
	} else {
		sf::Color& color1 = states[currentState].stateColor;
		sf::Color& color2 = states[states[currentState].nextState].stateColor;
		double fraction = timeToNextState/states[currentState].transitionTime;
		currentLight = color1 * fraction + color2 * (1 - fraction);
	}
	glClearColor(currentLight.r / 255.0, currentLight.g / 255.0, currentLight.b / 255.0, 1.0);
	/*

	*/
}

void Skybox::render(){

}
