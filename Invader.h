#pragma once
#include "Player.h"
#include <SFML/Graphics.hpp>

using namespace sf;
class Invader : public Player
{
public:
	static bool direction;
	static float speed;
	Player* target;
	Invader( Vector2f pos);
	
	Invader(Vector2f pos, Player* tar);
	Invader();
	void Update(const float &dt) override;
	~Invader() = default;
};
