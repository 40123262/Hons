#pragma once
#include <SFML/Graphics.hpp>

class Player : public sf::Sprite
{
protected:
	sf::IntRect _sprite;
	bool alive = true;
	Player();

	/////////////////////////////ANIMATION AND SPRITES
	bool isMoving = false;
	bool pushed = false;
	bool defending = false;
	float pushTimer = 0.1f;
	bool isAttacking = false;
	bool animation_incomplete = false;
//	std::shared_ptr<sf::Sprite> _sprite;
	int facing=3; //direction player/npc is facing 1 - north, then clockwise (1-4)
	float face_x = 0;
	float push_x = 0;
	float push_y = 0;
	float face_y = 0;
	float fireTime = 0.0f;
	float health = 100;
	int frame = 0; //Current frame of player/npc animation (goes from 0 to 3)
	const float AnimationDelay = 0.05f; //delay between frames of animation
	float AnimationCounter = 0.15f; //used as a count-down between frames, then set to AnimationDelay when frame changes
	std::vector<sf::IntRect> walkingAnimationDown;
	std::vector<sf::IntRect> walkingAnimationUp;
	std::vector<sf::IntRect> walkingAnimationRight;
	std::vector<sf::IntRect> walkingAnimationLeft;
	std::vector<sf::IntRect> swordAnimationDown;
	std::vector<sf::IntRect> swordAnimationUp;
	std::vector<sf::IntRect> swordAnimationRight;
	std::vector<sf::IntRect> swordAnimationLeft;
	std::vector<sf::IntRect> defendingAnimationUp;
	std::vector<sf::IntRect> defendingAnimationDown;
	std::vector<sf::IntRect> defendingAnimationRight;
	std::vector<sf::IntRect> defendingAnimationLeft;
public:
	float destructionDelay = 0.5f;
	bool isAlive() const;
	virtual void Die();
	virtual bool isPlayer();
	void getHit(float x);
	void Push(float x, float y);
	void Push(float x, float y, bool f);
	bool isDefending() const;
	explicit Player(sf::IntRect ir);

	virtual ~Player() = 0;

	virtual void Update(const float &dt);
};
