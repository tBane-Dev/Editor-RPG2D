#pragma once
#include "Objects/Terrain.hpp"

Terrain::Terrain() : Object() {
	_id = -1;
	_type = ObjectType::Terrain;
	_textureRect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(16, 16));
}

Terrain::Terrain(int id, sf::IntRect textureRect) : Object() {
	_id = id;
	_type = ObjectType::Terrain;
	_textureRect = textureRect;
}

Terrain::~Terrain() {

}
