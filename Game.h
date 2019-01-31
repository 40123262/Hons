#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

constexpr uint16_t gameWidth = 800;
constexpr uint16_t gameHeight = 600;
constexpr uint16_t invaders_rows = 1;
constexpr uint16_t invaders_columns = 1;

extern sf::Texture spritesheet, player_spritesheet;
extern std::vector<Player*> players;
