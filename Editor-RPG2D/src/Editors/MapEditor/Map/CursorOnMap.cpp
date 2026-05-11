#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "Cursor.hpp"
#include "Editors/MapEditor/Map/CameraOnMap.hpp"
#include "Editors/MapEditor/Map/Map.hpp"
#include "Editors/Editor.hpp"
#include "Editors/MapEditor/MapEditor.hpp"

CursorOnMap::CursorOnMap() {

	std::shared_ptr<MapEditor> map_editor = std::dynamic_pointer_cast<MapEditor>(editor_manager->get_back());
	if (!map_editor) return;

	std::shared_ptr<CameraOnMap> camera = map_editor->_camera;
    if (!camera) return;

    window->setView(camera->_view);
    _position = sf::Vector2i(window->mapPixelToCoords(cursor->_position));
}

CursorOnMap::~CursorOnMap() {

}

void CursorOnMap::update() {

    std::shared_ptr<MapEditor> map_editor = std::dynamic_pointer_cast<MapEditor>(editor_manager->get_back());
    if (!map_editor) return;

    std::shared_ptr<CameraOnMap> camera = map_editor->_camera;
    if (!camera) return;

    window->setView(camera->_view);
    _position = sf::Vector2i(window->mapPixelToCoords(cursor->_position));
}

void CursorOnMap::handleEvent(const sf::Event& event) {

}

void CursorOnMap::draw()
{
	std::shared_ptr<MapEditor> map_editor = std::dynamic_pointer_cast<MapEditor>(editor_manager->get_back());
	if (!map_editor) return;
	
    std::shared_ptr<Map> mapa = map_editor->_map;
	if (mapa == nullptr) return;

	std::shared_ptr<CameraOnMap> camera = map_editor->_camera;
	if (camera == nullptr) return;

    if (GUI_manager->Element_hovered != mapa) return;

    window->setView(camera->_view);

    int r = 5;
    sf::Color color(255, 0, 0, 127);

	std::shared_ptr<Chunk> hoveredChunk = mapa->getChunkByGlobalPosition();
	if (!hoveredChunk) return;

	std::shared_ptr<Tile> hoveredTile = hoveredChunk->getTileByGlobalPosition();
	if (!hoveredTile) return;

    float bw = Tile::borderWidth;

    sf::VertexArray vertexArray(sf::PrimitiveType::Triangles);

    for (int yy = -r / 2; yy <= r / 2; yy++) {
        for (int xx = -r / 2; xx <= r / 2; xx++) {
            int tx = hoveredTile->_coords.x + xx;
            int ty = hoveredTile->_coords.y + yy;

			std::shared_ptr<Chunk> chunk = mapa->getChunkByTileGlobalCoords(tx, ty);
			if (!chunk) continue;

            std::shared_ptr<Tile> tile = chunk->getTileByTileGlobalCoords(tx, ty);
            if (!tile) continue;

            float x = (float)(tx * Tile::tileSize);
            float y = (float)(ty * Tile::tileSize);

            sf::Vertex v1({ x + bw, y + bw }, color);
            sf::Vertex v2({ x + bw, y + Tile::tileSize - bw }, color);
            sf::Vertex v3({ x + Tile::tileSize - bw, y + Tile::tileSize - bw }, color);
            sf::Vertex v4({ x + Tile::tileSize - bw, y + bw }, color);

            vertexArray.append(v1);
            vertexArray.append(v2);
            vertexArray.append(v3);

            vertexArray.append(v1);
            vertexArray.append(v3);
            vertexArray.append(v4);
        }
    }

	window->draw(vertexArray);
}

std::shared_ptr<CursorOnMap> cursorOnMap;