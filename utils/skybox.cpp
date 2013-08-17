/*
The MIT License (MIT)

Copyright (c) 2013, Steven Wokke

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
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
	states.push_back(SkyboxStateData(300.0,60.0,sf::Color(50,100,200),sf::Color(200,200,150),1));
	states.push_back(SkyboxStateData(300.0,60.0,sf::Color(0,0,20), sf::Color(20,20,50),0));
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
