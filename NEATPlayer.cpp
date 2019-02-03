#include "NEATPlayer.h"
#include "Game.h"
#include <iostream>
#include "maths.h"

using namespace sf;
int chosen_output_id;
NEATPlayer::NEATPlayer() : m_dFitness(0), m_up(0), m_down(0), m_right(0), m_left(0), m_attack(0), m_defend(0)
{
	alive = true;
	show_hud = true;
	setTexture(player_spritesheet);
	setTextureRect({ 168, 23, 23, 26 });
	
	CreateSensors(m_Sensors, CParams::iNumSensors, CParams::dSensorRange);

	m_MemoryMap.Init(CParams::WindowWidth,
		CParams::WindowHeight);
}

void NEATPlayer::CreateSensors(vector<SPoint> &sensors,
	int            NumSensors,
	double         range)
{
	//make sure vector of sensors is empty before proceeding
	sensors.clear();

	double SegmentAngle = 100 / (NumSensors );

	//going clockwise from 90deg left of position calculate the fan of
	//points radiating out (not including the origin)
	for (int i = 0; i < CParams::iNumSensors; i++)
	{
		//calculate vertex position
		SPoint point;

		point.x = -sin(i * SegmentAngle - CParams::dHalfPi) * range;

		point.y = cos(i * SegmentAngle - CParams::dHalfPi) * range;

		sensors.push_back(point);
		
	}//next segment
	
}
bool NEATPlayer::isPlayer()
{
	return true;
}
void NEATPlayer::Die()
{
	//Reset();
	
	alive = false;
	setColor(Color::Black);
	m_MemoryMap.Reset();

}
void NEATPlayer::Reset()
{
	//reset the sweepers positions
	setColor(Color::White);
	//and the fitness
	setPosition(start_positon);
	m_dFitness = 0;
	health = 100;
	t_idle = 0.0f;
	damage_taken = 0;
	damage_done = 0;
	alive = true;
	kills = 0;
	time_alive = 0;

	//reset its memory
	m_MemoryMap.Reset();

}
bool NEATPlayer::NN_Update(const float &dt)
{
	//this will store all the inputs for the NN
	vector<double> inputs;
	t_idle += dt;
	if (t_idle > 45.0f)
		Die();
	//grab sensor readings
	TestSensors();

	for (int sr = 0; sr < m_vecdSensors.size(); ++sr)
	{
		inputs.push_back(m_vecdSensors[sr]);

		inputs.push_back(m_vecFeelers[sr]);
	}


	//update the brain and get feedback
	vector<double> output = m_pItsBrain->Update(inputs, CNeuralNet::active);

	//make sure there were no errors in calculating the 
	//output
	if (output.size() < CParams::iNumOutputs)
	{
		return false;
	}
	m_up = output[0];
	m_down = output[1];
	m_left = output[2];
	m_right = output[3];
	m_attack = output[4];
	m_defend = output[5];
	chosen_output_id = -1;
	double min_o = -100.0f;
	for (int i = 0; i < output.size(); i++)
	{
		if (output[i] > min_o)
		{
			chosen_output_id = i;
			min_o = output[i];
		}
	}
	Update(dt);
	m_MemoryMap.Update(getPosition().x, getPosition().y);
	return true;
}
void NEATPlayer::Update(const float &dt)
{
	
	if (alive)
	{
		Player::Update(dt);
		//movement
		fireTime -= dt;
		defendDelay -= dt;
		float direction = 0.0f;
		isMoving = false;
		defending = false;
	
		if (chosen_output_id==3)
		{
			if (validMove(getPosition() + Vector2f(100 * dt, 0)))
			{
				move(100 * dt, 0);
			}
			facing = 2;
			face_x = 1.0f;
			face_y = 0.0f;
			isMoving = true;
		}
		if (chosen_output_id == 2)
		{
			if (validMove(getPosition() + Vector2f(-100 * dt, 0)))
			{
				move(-100 * dt, 0);
			}
			facing = 4;
			face_x = -1.0f;
			face_y = 0.0f;
			isMoving = true;
		}
		if (chosen_output_id == 0)
		{
			if (validMove(getPosition() + Vector2f(0, -100 * dt)))
			{
				move(0, -100 * dt);
			}
			facing = 1;
			face_x = 0.0f;
			face_y = -1.0f;
			isMoving = true;
		}
		 if (chosen_output_id == 1)
		{
			 if (validMove(getPosition() + Vector2f(0, 100 * dt)))
			 {
				 move(0, 100 * dt);
			 }
			facing = 3;
			face_x = 0.0f;
			face_y = 1.0f;
			isMoving = true;
		}
		if (chosen_output_id == 5)
		 {
			
			if(defendDelay<=0)
				defending = true;
		}
		if (chosen_output_id == 4)
		if(fireTime <= 0.0f)
		{
			
		//	Bullet::Fire(getPosition(), false);
			for (auto p : m_vecEnemies)
			{
				if (p == this || !p->isAlive())
					continue;
				if (length((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin())- Vector2f(32.0f*face_x, 32.0f*face_y)) <= 32.0f)
				{
					if (!p->isDefending())
					{
						//std::cerr << "HIT!" << std::endl;
						Vector2f push = normalize((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin()));
						p->Push(push.x, push.y);
						p->getHit(20.0f);
						idle_time = 0.0f;
						damage_done += 20.0f;
						t_idle = 0.0f;
						if (p->getHealth() <= 0)
							addKill();
					}
					else
					{
						Vector2f push = 0.5f*normalize((p->getPosition() - p->getOrigin()) - (getPosition() - getOrigin()));
						Push(-push.x, -push.y, false);
						p->Push(push.x, push.y, false);
						t_idle = 0.0f;
					}
					
				}
			}
			isAttacking = true;
			fireTime = 0.5f;

		}
		
	}
	
	
}


void NEATPlayer::EndOfRunCalculations()
{
	//m_dFitness += time_alive + (kills*50.0f) - damage_taken + damage_done;
	m_dFitness += kills;
}
void NEATPlayer::WorldTransform(vector<SPoint> &sweeper, double scale)
{
	//create the world transformation matrix
	C2DMatrix matTransform;

	//scale
	matTransform.Scale(scale, scale);

	//rotate
	//matTransform.Rotate(m_dRotation);

	//and translate
	matTransform.Translate(getPosition().x, getPosition().y);

	//now transform the ships vertices
	matTransform.TransformSPoints(sweeper);
}
void NEATPlayer::TestSensors()
{
	m_bCollided = false;


	m_tranSensors = m_Sensors;

	WorldTransform(m_tranSensors, 1);  //scale is 1

	//flush the sensors
	m_vecdSensors.clear();
	m_vecFeelers.clear();

	//now to check each sensor against the objects in the world
	for (int sr = 0; sr < m_Sensors.size(); ++sr)
	{
		bool bHit = false;

		double dist = 0;

		for (int seg = 0; seg < m_vecEnemies.size(); seg ++)
		{
			if (LineIntersection2D(SPoint(getPosition().x, getPosition().y),
				SPoint(getPosition().x+ m_Sensors[sr].x, getPosition().y+ m_Sensors[sr].y),
				SPoint(m_vecEnemies[seg]->getPosition().x-20.0f, m_vecEnemies[seg]->getPosition().y-20.0f),
				SPoint(m_vecEnemies[seg]->getPosition().x+20.0f, m_vecEnemies[seg]->getPosition().y + 20.0f),
				dist) ||
				LineIntersection2D(SPoint(getPosition().x, getPosition().y),
					SPoint(getPosition().x + m_Sensors[sr].x, getPosition().y + m_Sensors[sr].y),
					SPoint(m_vecEnemies[seg]->getPosition().x + 20.0f, m_vecEnemies[seg]->getPosition().y - 20.0f),
					SPoint(m_vecEnemies[seg]->getPosition().x - 20.0f, m_vecEnemies[seg]->getPosition().y + 20.0f),
					dist))
			{
				bHit = true;
				break;
			}
		}

		if (bHit)
		{
			m_vecdSensors.push_back(dist);

			//implement very simple collision detection
			if (dist < CParams::dCollisionDist)
			{
				m_bCollided = true;
			}
		}

		else
		{
			m_vecdSensors.push_back(-1);
		}

		//check how many times the minesweeper has visited the cell
		//at the current position
		int HowOften = m_MemoryMap.TicksLingered(m_tranSensors[sr].x,
			m_tranSensors[sr].y);


		//Update the memory info according to HowOften. The maximum
		//value is 1 (because we want all the inputs into the
		//ANN to be scaled between -1 < n < 1)
		if (HowOften == 0)
		{
			m_vecFeelers.push_back(-1);

			continue;
		}

		if (HowOften < 10)
		{
			m_vecFeelers.push_back(0);

			continue;
		}

		if (HowOften < 20)
		{
			m_vecFeelers.push_back(0.2);

			continue;
		}

		if (HowOften < 30)
		{
			m_vecFeelers.push_back(0.4);

			continue;
		}

		if (HowOften < 50)
		{
			m_vecFeelers.push_back(0.6);

			continue;
		}

		if (HowOften < 80)
		{
			m_vecFeelers.push_back(0.8);

			continue;
		}

		m_vecFeelers.push_back(1);

	}//next sensor
}
