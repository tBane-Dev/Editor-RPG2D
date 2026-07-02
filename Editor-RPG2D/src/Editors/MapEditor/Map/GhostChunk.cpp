#include "Editors/MapEditor/Map/GhostChunk.hpp"
#include "Editors/MapEditor/Map/Tile.hpp"
#include "Editors/MapEditor/Map/Chunk.hpp"
#include "Editors/MapEditor/Map/Map.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "Objects/Monster.hpp"
#include "DebugLog.hpp"
#include "Cursor.hpp"
#include "RenderWindow.hpp"

GhostChunk::GhostChunk() {

	_isVisible = false;

	_rect = sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(Chunk::tilesCols * Tile::tileSize, Chunk::tilesRows * Tile::tileSize));

	_button = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\ghost_chunk_plus.png"),
		textures_manager->getTexture(L"assets\\tex\\ghost_chunk_plus_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\ghost_chunk_plus_press.png"),
		textures_manager->getTexture(L"assets\\tex\\ghost_chunk_plus_press.png")
	);
}

GhostChunk::~GhostChunk() {

}

void GhostChunk::cursorHover() {

	sf::IntRect renderWindowRect;
	renderWindowRect.position = Main::render_window->getPosition();
	renderWindowRect.size = sf::Vector2i(Main::render_window->getSize());

	if (!renderWindowRect.contains(Main::cursor->_position + renderWindowRect.position))
		return;

	sf::Vector2i coords;
	coords.x = std::floor((float)MapEditor::editor->_cursor_on_map->_position.x / (float)(Chunk::tilesCols * Tile::tileSize));
	coords.y = std::floor((float)MapEditor::editor->_cursor_on_map->_position.y / (float)(Chunk::tilesRows * Tile::tileSize));

	if (
		MapEditor::editor->_map->getChunkByCoords(coords.x, coords.y) == nullptr && (
			MapEditor::editor->_map->getChunkByCoords(coords.x - 1, coords.y) ||
			MapEditor::editor->_map->getChunkByCoords(coords.x + 1, coords.y) ||
			MapEditor::editor->_map->getChunkByCoords(coords.x, coords.y - 1) ||
			MapEditor::editor->_map->getChunkByCoords(coords.x, coords.y + 1))
		)
	{
		
		_rect.position = coords * (Chunk::tilesCols * Tile::tileSize);
		_button->setPosition(_rect.position + _rect.size/2 - sf::Vector2i(24,24));
		_button->_onclick_func = [this, coords]() {

			int offsetX = 0;
			int offsetY = 0;

			if (coords.x < 0) offsetX = -coords.x;
			if (coords.y < 0) offsetY = -coords.y;

			int pixelOffsetX = offsetX * Chunk::tilesCols * Tile::tileSize;
			int pixelOffsetY = offsetY * Chunk::tilesRows * Tile::tileSize;

			// move the camera
			if (offsetX != 0) MapEditor::editor->_camera->_position.x += (float)pixelOffsetX;
			if (offsetY != 0) MapEditor::editor->_camera->_position.y += (float)pixelOffsetY;

			// move the gameobjects
			for(auto& go : MapEditor::editor->_game_objects->_gameObjectsOnMap) {

				sf::Vector2i pos = go->getPosition();
				if(offsetX != 0) pos.x += pixelOffsetX;
				if(offsetY != 0) pos.y += pixelOffsetY;
				go->setPosition(pos);

				if(go->_type == ObjectType::Monster) {
					std::shared_ptr<Monster> monster = std::dynamic_pointer_cast<Monster>(go);
					monster->_position.x += pixelOffsetX;
					monster->_position.y += pixelOffsetY;
				}
			}


			std::shared_ptr<Map> map = MapEditor::editor->_map;

			// move existing chunks if new chunk would have negative coords
			if (offsetX != 0 || offsetY != 0) {
				for (auto& c : map->_chunks) {
					c->_coords.x += offsetX;
					c->_coords.y += offsetY;

					for (auto& tile : c->_tiles) {
						tile->_coords.x += offsetX * Chunk::tilesCols;
						tile->_coords.y += offsetY * Chunk::tilesRows;

						tile->_position.x = (float)(tile->_coords.x * Tile::tileSize);
						tile->_position.y = (float)(tile->_coords.y * Tile::tileSize);
					}
				}
			}

			std::shared_ptr<Chunk> newChunk = std::make_shared<Chunk>(
				coords.x + offsetX,
				coords.y + offsetY
			);

			map->addChunk(newChunk, 2);

			// refresh all chunks, because existing chunks may have changed position
			for (auto& c : map->_chunks) {
				int x = c->_coords.x;
				int y = c->_coords.y;

				c->generateVertexArray(
					map->getChunkByCoords(x - 1, y - 1), map->getChunkByCoords(x, y - 1), map->getChunkByCoords(x + 1, y - 1),
					map->getChunkByCoords(x - 1, y), map->getChunkByCoords(x + 1, y),
					map->getChunkByCoords(x - 1, y + 1), map->getChunkByCoords(x, y + 1), map->getChunkByCoords(x + 1, y + 1)
				);
			}
		};

		if (_rect.contains(MapEditor::editor->_cursor_on_map->_position)) {
			GUI_manager->Element_hovered = shared_from_this();
			
			// manual cursor hover for button
			if(_button->_rect.contains(MapEditor::editor->_cursor_on_map->_position)) {
				GUI_manager->Element_hovered = _button;
			}
		}
	}

}

void GhostChunk::handleEvent(const sf::Event& event) {

	_button->handleEvent(event);
}

void GhostChunk::update() {

	if (GUI_manager->Element_pressed != nullptr &&
		GUI_manager->Element_pressed != _button &&
		GUI_manager->Element_pressed != MapEditor::editor->_map
		) {
		_isVisible = false;
		return;
	}

	if (MapEditor::editor->_cursor_on_map->_state == CursorOnMapState::Drawing) {
		_isVisible = false;
		return;
	}

	_isVisible = (GUI_manager->Element_hovered.get() == this || GUI_manager->Element_hovered == _button);

	if(_isVisible)
		_button->update();
}

void GhostChunk::draw() {
	if (!_isVisible)
		return;

	int borderWidth = 4;
	sf::RectangleShape rect(sf::Vector2f(Chunk::tilesCols * Tile::tileSize - 2*borderWidth, Chunk::tilesRows * Tile::tileSize - 2* borderWidth));
	rect.setPosition(sf::Vector2f(_rect.position.x + borderWidth, _rect.position.y + borderWidth));
	rect.setFillColor(sf::Color(63,63,63,127));
	rect.setOutlineThickness(borderWidth);
	rect.setOutlineColor(sf::Color(31, 31, 31, 255));
	Main::render_window->draw(rect);

	_button->draw();
}