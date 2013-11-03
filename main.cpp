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
#include <iostream>
#include <time.h>
#include <memory>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "screen.h"
#include "entities/player.h"
#include "world.h"
#include "utils/hud.h"
#include "game.h"
#include <pugixml.hpp>
#include <assert.h>

class Game{
	std::unique_ptr<Screen> screen;
	std::unique_ptr<World> world;
	std::unique_ptr<HUD> hud;
	Player* player;
	sf::Clock fpsClock;
public:
	bool running = true;
	Game();
	~Game();
	void doWindowEvents();
	void tick();
	void initGL();
};

void Game::doWindowEvents(){
	sf::Event event;

	while (screen->window->pollEvent(event)){
		switch (event.type){
			case sf::Event::EventType::Closed:
				running = false;
				break;
			case sf::Event::EventType::Resized:
				screen->resize(Vector2::uToI(screen->window->getSize()));
				break;
			case sf::Event::EventType::MouseWheelMoved:
				player->onMouseWheel(event.mouseWheel.delta);
				break;
			case sf::Event::EventType::MouseEntered:
				screen->mouseInWindow = true;
				break;
			case sf::Event::EventType::MouseLeft:
				screen->mouseInWindow = false;
				break;
			case sf::Event::EventType::MouseButtonPressed:
				if (screen->mouseInWindow){
					hud->onMouseEvent(event, *screen, *player);
				}
				break;
			case sf::Event::EventType::KeyPressed:
				if (event.key.code == sf::Keyboard::Key::Space)
					player->tryJump();
				else if (event.key.code >= sf::Keyboard::Key::Num1 && event.key.code <= sf::Keyboard::Key::Num9)
					player->selectItem((int)(event.key.code) - (int)(sf::Keyboard::Key::Num1));
				else if (event.key.code == sf::Keyboard::Key::Num0)
					player->selectItem(9);
				else if (event.key.code == sf::Keyboard::Key::E)
					hud->toggleInventory();
				else if (event.key.code == sf::Keyboard::Key::P)
					GameGlobals::paused = !GameGlobals::paused;
				break;
		}
	}

	if (screen->mouseInWindow){
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
			if (!hud->hasFocus()){
				player->useItem(*screen.get());
			}
		}
	}
}

void Game::initGL(){
	glEnable(GL_TEXTURE_2D);
	glEnable (GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.1);
	glEnable(GL_ALPHA_TEST);
}

Game::Game(){
	srand(time(nullptr));
	screen.reset(new Screen());
	initGL();
	GameGlobals::init();
	world.reset(new World());
	std::unique_ptr<Player> stats;
	stats.reset(new Player());

	pugi::xml_document doc;
	auto result = doc.load_file(GameGlobals::fileSystem.fullpath("player.xml").c_str());
	if (result){
		auto playerNode = doc.child("player");
		stats->load(playerNode);
		player = world->spawn(*stats,Vector2d(20,-10));
	} else {
		std::cerr << result.description();
	}

	assert(player != nullptr);

	hud.reset(new HUD());
}

void Game::tick(){
	double time = fpsClock.restart().asSeconds();
	if (time > 0.1)
		time = 0.1; // To avoid slow FPS screwing up physics

	if (GameGlobals::paused)
		time = 0;

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

Game::~Game(){
	GameGlobals::clear();
}

int main(){
	// create the window
	Game game;
	while (game.running){
		game.tick();
	}
	return 0;
}
