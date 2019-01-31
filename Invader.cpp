#include "Invader.h"
#include "Game.h"
#include "maths.h"
#include <iostream>
using namespace sf;


bool Invader::direction = true;
float Invader::speed = 50.0f;

Invader::Invader() : Player() {};

Invader::Invader(IntRect ir, Vector2f pos) : Player(ir)
{
	alive = true;
	setOrigin(32, 32);
	setPosition(pos);
}

void Invader::Update(const float &dt)
{
	
	if (alive)
	{
		fireTime -= dt;
		Player::Update(dt);
		isMoving = false;

		Vector2f vec = (players[0]->getPosition() - players[0]->getOrigin()) - (getPosition() - getOrigin());
		if (length((players[0]->getPosition() - players[0]->getOrigin()) - (getPosition() - getOrigin())) >= 32.0f)
		{
			isMoving = true;
			if (vec.x >= 0 && vec.y >= 0)
			{
				if (vec.x > vec.y)
				{
					facing = 2;
					face_x = 1.0f;
					face_y = 0.0f;
				}
				else
				{
					facing = 3;
					face_x = 0.0f;
					face_y = 1.0f;
				}
			}
			else if (vec.x >= 0 && vec.y < 0)
			{
				if (abs(vec.x) > abs(vec.y))
				{
					facing = 2;
					face_x = 1.0f;
					face_y = 0.0f;
				}
				else
				{
					facing = 1;
					face_x = 0.0f;
					face_y = -1.0f;
				}
			}
			else if (vec.x < 0 && vec.y < 0)
			{
				if (abs(vec.x) > abs(vec.y))
				{
					facing = 4;
					face_x = -1.0f;
					face_y = 0.0f;
				}
				else
				{
					facing = 1;
					face_x = 0.0f;
					face_y = -1.0f;
				}
			}
			else if (vec.x < 0 && vec.y >= 0)
			{
				if (abs(vec.x) > abs(vec.y))
				{
					facing = 4;
					face_x = -1.0f;
					face_y = 0.0f;
				}
				else
				{
					facing = 3;
					face_x = 0.0f;
					face_y = 1.0f;
				}
			}
			move(90.0f*dt*normalize(players[0]->getPosition() - getPosition()));
		}
		else
		{
			if (fireTime <= 0.0f)
			{
				for (auto p : players)
				{
					if (p == this)
						continue;
					if (length((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin()) - Vector2f(32.0f*face_x, 32.0f*face_y)) <= 50.0f)
					{
						if (!p->isDefending())
						{
							std::cerr << "HIT!" << std::endl;
							Vector2f push = normalize((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin()));
							p->Push(push.x, push.y);
							p->getHit(20.0f);
						}
						else
						{
							Vector2f push = normalize((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin()));
							Push(-push.x, -push.y, false);
						}

					}
				}
				isAttacking = true;
				fireTime = 1.0f;
			}
		}
	}
	else
	{
		destructionDelay -= dt;
	}
	if (destructionDelay <= 0.0f)
		setPosition({ -100,-100 });
		
	
	
}


