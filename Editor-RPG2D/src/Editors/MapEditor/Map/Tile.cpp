#include "Editors/MapEditor/Map/Tile.hpp"

Tile::Tile() {
	_type = 0;
}

Tile::~Tile() {

}

sf::Vector2i worldToTile(sf::Vector2i position) {
	return sf::Vector2i(position.x / Tile::tileSize, position.y / Tile::tileSize);
}
