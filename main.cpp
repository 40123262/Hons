#include <SFML/Graphics.hpp>
#include <iostream>
#include "Invader.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "Game.h"

using namespace sf;
using namespace std;

const Keyboard::Key controls[4] = {
	Keyboard::Left,
	Keyboard::Right,
	Keyboard::Space
};
vector<Player*> players;
Texture spritesheet, player_spritesheet;
Sprite invader;
void Load()
{
	if(!spritesheet.loadFromFile("res/img/invaders_sheet.png"))
	{
		cerr << "Failed to load spritesheet!" << endl;
	}
	if (!player_spritesheet.loadFromFile("res/img/body.png"))
	{
		cerr << "Failed to load spritesheet!" << endl;
	}
	HumanPlayer* p1 = new HumanPlayer();
	players.push_back(p1);
	for (int row = 0; row < invaders_rows; row++)
	{
		auto displacement = (row % 5);
		auto rect = IntRect(168, 23, 23, 26);
		for (int col = 0; col < invaders_columns; col++)
		{
			Vector2f pos = { 16.0f + 42.0f * col, 16.0f + 42.0f * row };
			Invader* inv = new Invader(rect, pos);
			//inv->scale({ 1.2f,1.7f });
			players.push_back(inv);
		}
	}
	
}
void reset()
{

}
void Update(RenderWindow &window)
{
	static Clock clock;
	float dt = clock.restart().asSeconds();

	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			window.close();
			return;
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		window.close();
	}
	for (auto &s : players)
	{
		s->Update(dt);
	}

}

void Render(RenderWindow &window)
{
	for (auto &s : players)
	{
		if(s->isAlive())
		window.draw(*s);
	}
}

int main()
{
	RenderWindow window(VideoMode(gameWidth, gameHeight), "Space Invaders");
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