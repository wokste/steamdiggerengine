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

std::unique_ptr<Screen> screen;
std::unique_ptr<Game> game;
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
		} else if (event.type == sf::Event::MouseButtonPressed){
			Vector2i mousePos(event.mouseButton.x,event.mouseButton.y);
			if (hud->onMousePressed(*screen.get(), *player, event.mouseButton.button, mousePos))
				break;

			if (event.mouseButton.button == sf::Mouse::Left) {
				player->useItem(*screen.get());
			} else if (event.mouseButton.button == sf::Mouse::Right){
				//TODO: events on the map.
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

	game.reset(new Game());
	world.reset(new World(game.get()));
	world->spawn(game->fileSystem.fullpath("ghost.json"),Vector2d(10,-20));
	player = dynamic_cast<Player*>(world->spawn(game->fileSystem.fullpath("player.json"),Vector2d(20,-10)));
	ASSERT(player != nullptr, "Main", "player = NULL");

	hud.reset(new HUD(*game.get()));

	bool running = true;
	while (running){
		player->checkInput(10,*screen.get());
		world->logic(10);

		doWindowEvents();

		glClearColor(0.2f, 0.0f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		screen->startScene();
		glPushMatrix();
			screen->centerOn(player);
			world->render();
		glPopMatrix();

		hud->draw(*screen.get(), *player);
		screen->window->display();
	}
	return 0;
}

int floorInt(double d){
	return (int) (d - 0.5);
}
