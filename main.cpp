#include <iostream>
#include <time.h>
#include <memory>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "screen.h"
#include "entities/player.h"
#include "world.h"
#include "utils/hud.h"
#include "utils/assert.h"
#include "game.h"
#include "utils/confignode.h"

std::unique_ptr<Screen> screen;
std::unique_ptr<World> world;
std::unique_ptr<HUD> hud;
Player* player;

void doWindowEvents(){
	sf::Event event;
	while (screen->window->pollEvent(event)){
		if (event.type == sf::Event::EventType::Closed){
			exit(0); // TODO: make this nicer.
		} else if (event.type == sf::Event::EventType::Resized){
			screen->resize(Vector2::uToI(screen->window->getSize()));
		} else if (event.type == sf::Event::EventType::MouseWheelMoved) {
			player->onMouseWheel(event.mouseWheel.delta);
		} else if (event.type == sf::Event::EventType::KeyPressed){
			if (event.key.code == sf::Keyboard::Key::Space)
				player->tryJump();
			else if (event.key.code >= sf::Keyboard::Key::Num1 && event.key.code <= sf::Keyboard::Key::Num9)
				player->selectItem((int)(event.key.code) - (int)(sf::Keyboard::Key::Num1));
		}
	}

	Vector2i mousePos = sf::Mouse::getPosition(*screen->window);
	Vector2i windowSize = Vector2::uToI(screen->window->getSize());
	if (mousePos.x >= 0 && mousePos.x < windowSize.x && mousePos.y >= 0 && mousePos.y < windowSize.y){
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
			bool handled = hud->onMousePressed(*screen.get(), *player, event.mouseButton.button, mousePos);
			if (!handled){
				player->useItem(*screen.get());
			}
		}
	}
}

void initGL(){
	glEnable(GL_TEXTURE_2D);
	glEnable (GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);
}

void draw(){

}

int main(){
	// create the window
	srand(time(nullptr));
	screen.reset(new Screen());
	initGL();
	GameGlobals::init();
	world.reset(new World());
	std::unique_ptr<Player> stats;
	stats.reset(new Player());
 	ConfigNode::load(GameGlobals::fileSystem.fullpath("player.json"),[&] (ConfigNode& config){
		stats->load(config);
		player = world->spawn(*stats,Vector2d(20,-10));
 	});

	ASSERT(player != nullptr, "Main", "player = NULL");

	hud.reset(new HUD());
	sf::Clock fpsClock;

	bool running = true;
	while (running){
		double time = fpsClock.restart().asSeconds();
		if (time > 0.1)
			time = 0.1; // To avoid slow FPS screwing up physics
		player->checkInput(time,*screen.get());
		world->logic(time);

		doWindowEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		screen->startScene();
		glPushMatrix();
			screen->centerOn(*player);
			world->render(*screen);
		glPopMatrix();

		hud->draw(*screen, *player);
		screen->window->display();
	}
	return 0;
}

int floorInt(double d){
	return (int) (d - 0.5);
}
