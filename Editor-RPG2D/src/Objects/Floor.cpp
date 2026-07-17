#pragma once
#include "Objects/Floor.hpp"

Floor::Floor() {
	_id = -1;
	_type = ObjectType::Floor;
	_textureRect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(16, 16));
}

Floor::Floor(int id, sf::IntRect textureRect) {
	_id = id;
	_type = ObjectType::Floor;
	_textureRect = textureRect;
}

Floor::~Floor() {

}
