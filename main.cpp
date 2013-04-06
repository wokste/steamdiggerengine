#include <iostream>
#include <time.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "screen.h"
#include "entities/player.h"
#include "world.h"
#include "hud.h"
#include "utils/assert.h"

std::string dataDirectory;

int main(){
	// create the window
	dataDirectory = "../../data/"; // TODO: fix this line
	dataDirectory = "C:/Users/wokste/projects/steamdigger/data/";
	//dataDirectory += "/.steamdigger/core/";
	std::cout << dataDirectory;

	sf::ContextSettings settings;
	settings.depthBits = 0;
	settings.stencilBits = 0;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 1;
	settings.minorVersion = 1;

	sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	Screen screen(&window);

	srand(time(nullptr));

	// load resources, initialize the OpenGL states, ...
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH);
	 glEnable (GL_DEPTH_TEST);
	//glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	world = new World();

	Player* player = dynamic_cast<Player*>(world->spawn("player.json",Vector2d(20,10)));
	ASSERT(player != nullptr, "Main", "player = NULL");
	//world->spawn("ghost.json",Vector2d(1200,200));
	HUD * hud = new HUD();

	bool running = true;
	while (running){
		if (player != nullptr)
			player->checkInput(10,screen);
		world->logic(10);

		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::EventType::Closed){
				running = false;
			} else if (event.type == sf::Event::EventType::Resized){
				screen.resize(Vector2::uToI(window.getSize()));
			} else if (event.type == sf::Event::EventType::MouseWheelMoved) {
				player->onMouseWheel(event.mouseWheel.delta);
			}
		}
		glClearColor(0.2f, 0.0f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		screen.startScene();
		glPushMatrix();
		screen.centerOn(player);
		world->render();
		glPopMatrix();

		if (player != nullptr)
			hud->draw(player);
		// end the current frame (internally swaps the front and back buffers)
		window.display();
	}

	// release resources...
	delete world;
	delete hud;
	return 0;
}

int floorInt(double d){
	return (int) (d - 0.5);
}
