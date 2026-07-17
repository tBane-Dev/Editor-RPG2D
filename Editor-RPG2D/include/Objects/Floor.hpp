#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Object.hpp"

class Floor : public Object {
public:

	int _id = -1;
	sf::IntRect _textureRect;

	Floor();
	Floor(int id, sf::IntRect textureRect);
	~Floor();
};