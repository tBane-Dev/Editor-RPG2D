#include "Editors/MapEditor/Map/Chunk.hpp"
#include "Theme.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "RenderWindow.hpp"
#include "ShadersManager.hpp"
#include "Time.hpp"

Chunk::Chunk(int x, int y) {

	_coords.x = x;
	_coords.y = y;

	_vertexArray = sf::VertexArray(sf::PrimitiveType::Triangles);

	std::wstring text = std::to_wstring(_coords.x) + L"x" + std::to_wstring(_coords.y);
	float padding = 8.0f;
	_coordsText = std::make_unique<sf::Text>(basicFont, text, 24);
	_coordsText->setFillColor(basic_text_color);
	_coordsText->setPosition(sf::Vector2f(_coords.x * tilesCols * Tile::tileSize + padding, _coords.y * tilesRows * Tile::tileSize + padding));

}

Chunk::~Chunk() {

}


bool Chunk::operator<(const Chunk& other) const {
	if (_coords.x == other._coords.x)
		return _coords.y < other._coords.y;
	return _coords.x < other._coords.x;
}

std::shared_ptr<Tile> Chunk::getTileByTileGlobalCoords(int x, int y) {

	int xx = x - _coords.x * tilesCols;
	int yy = y - _coords.y * tilesRows;

	if (xx < 0 || yy < 0 || xx >= tilesCols || yy >= tilesRows)
		return nullptr;

	return _tiles[yy * Chunk::tilesCols + xx];
}

std::shared_ptr<Tile> Chunk::getTileByGlobalPosition() {

	sf::IntRect rect;
	rect.position.x = _coords.x * tilesCols * Tile::tileSize;
	rect.position.y = _coords.y * tilesRows * Tile::tileSize;
	rect.size.x = tilesCols * Tile::tileSize;
	rect.size.y = tilesRows * Tile::tileSize;

	if (rect.contains(MapEditor::editor->_cursor_on_map->_position)) {
		int x = MapEditor::editor->_cursor_on_map->_position.x / Tile::tileSize;
		int y = MapEditor::editor->_cursor_on_map->_position.y / Tile::tileSize;
		return getTileByTileGlobalCoords(x, y);
	}

	return nullptr;
}

void Chunk::generateVertexArray(
	std::shared_ptr<Chunk> leftTopChunk,
	std::shared_ptr<Chunk> topChunk,
	std::shared_ptr<Chunk> rightTopChunk,
	std::shared_ptr<Chunk> leftChunk,
	std::shared_ptr<Chunk> rightChunk,
	std::shared_ptr<Chunk> leftBottomChunk,
	std::shared_ptr<Chunk> bottomChunk,
	std::shared_ptr<Chunk> rightBottomChunk
) {

	_vertexArray.clear();
	_vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

	std::shared_ptr<Tileset> tileset = MapEditor::editor->_tileset;
	if (tileset == nullptr) return;

	float borderWidth = float(Tile::borderWidth);

	for (int y = 0; y < tilesRows; y++) {
		for (int x = 0; x < tilesCols; x++) {

			int tx = _coords.x * tilesCols + x;
			int ty = _coords.y * tilesRows + y;

			float xx = float(tx * Tile::tileSize);
			float yy = float(ty * Tile::tileSize);

			sf::Color color = sf::Color::White;

			int tileType = 5;
			int type = getTileByTileGlobalCoords(tx, ty)->_type;
			
			tileType = getTileType(
				type, x, y, shared_from_this(),
				leftTopChunk, topChunk, rightTopChunk,
				leftChunk, rightChunk,
				leftBottomChunk, bottomChunk, rightBottomChunk
			);

			std::shared_ptr<Tile> t = getTileByTileGlobalCoords(tx, ty);
			Tileset::Tile tile = tileset->groups[t->_type]->tiles[tileType];

			int u = (tile.x) * Tile::tileSize + tx * Tile::tileSize % 64;
			int v = (tile.y) * Tile::tileSize + ty * Tile::tileSize % 64;

			sf::Vertex v1 = sf::Vertex({ xx + borderWidth, yy + borderWidth }, color, sf::Vector2f((float)(u + 1), (float)(v + 1)));
			sf::Vertex v2 = sf::Vertex({ xx + borderWidth, yy + Tile::tileSize - borderWidth }, color, sf::Vector2f((float)(u + 1), (float)(v + Tile::tileSize - 1)));
			sf::Vertex v3 = sf::Vertex({ xx + Tile::tileSize - borderWidth, yy + Tile::tileSize - borderWidth }, color, sf::Vector2f((float)(u + Tile::tileSize - 1), (float)(v + Tile::tileSize - 1)));
			sf::Vertex v4 = sf::Vertex({ xx + Tile::tileSize - borderWidth, yy + borderWidth }, color, sf::Vector2f((float)(u + Tile::tileSize - 1), (float)(v + 1)));

			_vertexArray.append(v1);
			_vertexArray.append(v2);
			_vertexArray.append(v3);

			_vertexArray.append(v1);
			_vertexArray.append(v3);
			_vertexArray.append(v4);
		}
	}
}

void Chunk::drawCoords() {
	Main::render_window->draw(*_coordsText);
}

void Chunk::draw() {

	sf::RenderStates states;
	std::shared_ptr<Texture> texture = textures_manager->getTexture(L"assets\\tex\\tileset.png");
	terrain_shader->setUniform("resolution", sf::Vector2f(Main::render_window->getSize()));
	terrain_shader->setUniform("time", currentTime.asSeconds());
	terrain_shader->setUniform("startPos", sf::Vector2f(0, 0));
	states.texture = &*texture->_texture;
	states.shader = &*terrain_shader;
	Main::render_window->draw(_vertexArray, states);
	
}
