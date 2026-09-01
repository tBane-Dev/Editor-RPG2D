#include "Objects/Building/Roof.hpp"

Roof::Roof() {
	_type = -1;
	_tiles.clear();
	_roofOverhangSize = sf::Vector2i(0, 0);
	_wallHeight = 3;
}

Roof::Roof(int type, int wallHeight) {
	_type = type;
	_tiles.clear();
	_roofOverhangSize = sf::Vector2i(0, 0);
	_wallHeight = wallHeight;
}

Roof::~Roof() {
	
}

int Roof::getTopOffset(float scale) {
	return (float)(_wallHeight) * 32.f * scale;
}

void Roof::generate(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale) {
	_tiles = tiles;
}

void Roof::draw(sf::RenderTarget& target, sf::Vector2i position, float scale) {

}