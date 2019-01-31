#pragma once
#include "Player.h"
#include <SFML/Graphics.hpp>

using namespace sf;
class Invader : public Player
{
public:
	static bool direction;
	static float speed;
	Invader(IntRect ir, Vector2f pos);
	Invader();
	void Update(const float &dt) override;
	~Invader() = default;
};
