#include "Objects/Building/Roof.hpp"

Roof::Roof() {
	_tiles.clear();
	_roofOverhangSize = sf::Vector2i(0, 0);
}

Roof::~Roof() {
	
}


void Roof::generate(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale) {
	_tiles = tiles;
}

void Roof::draw(sf::RenderTarget& target, sf::Vector2i position, float scale) {

}