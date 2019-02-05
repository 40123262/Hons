#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Invader.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "Game.h"
#include "levelsystem.h"
#include <windows.h>
#include <cstdio>
#include <math.h>

#include "utils.h"
#include "CController.h"
#include "CTimer.h"
#include "resource.h"
#include "CParams.h"

using namespace sf;
using namespace std;

//The controller class for this simulation
CController*	g_pController = NULL;

CParams   g_Params;
sf::View camera;
//global handle to the info window
HWND g_hwndInfo = NULL;

//global handle to the main window
HWND g_hwndMain = NULL;

sf::Vector2f cam_pos = sf::Vector2f(640, 360);
Texture spritesheet, player_spritesheet;
Font font;
float idle_time = 0;
void Cleanup()
{
	if (g_pController)

		delete g_pController;
}

void Load()
{
	g_Params.Initialize();
	if(!spritesheet.loadFromFile("res/img/invaders_sheet.png"))
	{
		cerr << "Failed to load spritesheet!" << endl;
	}
	if (!player_spritesheet.loadFromFile("res/img/body.png"))
	{
		cerr << "Failed to load spritesheet!" << endl;
	}
	if (!font.loadFromFile("res/fonts/font.ttf")) {
		cout << "Cannot load font!" << endl;
	}
	ls::loadLevelFile("res/levels/pacman.txt", 40.0f , 4, 4);

	//setup the controller
	g_pController = new CController(1280, 720);
	
	
	
}
void reset()
{
	

	//Spawn();
}
void Update(RenderWindow &window)
{
	static Clock clock;
	float dt = clock.restart().asSeconds();
	idle_time += dt;
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			window.close();
			return;
		}
	}
	//g_pController->Update();
	
	if (Keyboard::isKeyPressed(Keyboard::F10))
	{
		window.close();
		return;
	}
	
	camera.setCenter(cam_pos);
	window.setView(camera);
	g_pController->Update(dt);
	for (auto &s : m_vecEnemies)
	{
		s->Update(dt);
	}
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		cam_pos.x -= 500.0f*dt;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		cam_pos.x += 500.0f*dt;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		cam_pos.y -= 500.0f*dt;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		cam_pos.y += 500.0f*dt;
	}
	

}

void Render(RenderWindow &window)
{
	ls::Render(window);
	g_pController->Render(window);
//	std::cout << "Enemies: " << m_vecEnemies.size() << "    NEATs: " << m_vecPlayers.size() << endl;
	
	
}

int main()
{
	//FreeConsole();
	camera.setCenter(cam_pos); //in constructor
	camera.setSize(gameWidth, gameHeight); //in constructor
	RenderWindow window(VideoMode(gameWidth, gameHeight), "NEAT", sf::Style::None);
	window.setView(camera);
	Load();
	reset();
	while (window.isOpen())
	{
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}
	return 0;
}