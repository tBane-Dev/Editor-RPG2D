#include "Editors/MapEditor/Map/Map.hpp"
#include <set>
#include "Editors/MapEditor/Editor.hpp"
#include "RenderWindow.hpp"
#include "ShadersManager.hpp"
#include "BinaryWriter.hpp"
#include "BinaryReader.hpp"

Map::Map() {
	_ghostChunk = std::make_shared<GhostChunk>();
}

Map::~Map() {

}

void Map::addChunk(std::shared_ptr<Chunk> chunk, int tileType) {

	int x = chunk->_coords.x;
	int y = chunk->_coords.y;

	for (int ty = 0; ty < Chunk::tilesRows; ty++) {
		for (int tx = 0; tx < Chunk::tilesCols; tx++) {
			std::shared_ptr<Tile> tile = std::make_shared<Tile>();

			tile->_type = tileType;

			tile->_coords.x = x * Chunk::tilesCols + tx;
			tile->_coords.y = y * Chunk::tilesRows + ty;

			tile->_position.x = (float)(tile->_coords.x * Tile::tileSize);
			tile->_position.y = (float)(tile->_coords.y * Tile::tileSize);

			chunk->_tiles.push_back(tile);
		}
	}

	_chunks.push_back(chunk);

	for (int yy = -1; yy <= 1; yy++) {
		for (int xx = -1; xx <= 1; xx++) {

			int cx = x + xx;
			int cy = y + yy;

			std::shared_ptr<Chunk> currentChunk = getChunkByCoords(cx, cy);

			if (!currentChunk)
				continue;

			currentChunk->generateVertexArray(
				getChunkByCoords(cx - 1, cy - 1), getChunkByCoords(cx, cy - 1), getChunkByCoords(cx + 1, cy - 1),
				getChunkByCoords(cx - 1, cy), getChunkByCoords(cx + 1, cy),
				getChunkByCoords(cx - 1, cy + 1), getChunkByCoords(cx, cy + 1), getChunkByCoords(cx + 1, cy + 1)
			);
		}
	}
}

void Map::create(int width, int height) {

	int tileType = 2;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(x,y);

			addChunk(chunk, tileType);
			

		}
	}
}

void Map::drawCircle(sf::Vector2i center, int radius, int type) {

	std::set<std::shared_ptr<Chunk>> chunksToEdit;
	for (int yy = center.y - radius; yy <= center.y + radius; yy++) {
		for (int xx = center.x - radius; xx <= center.x + radius; xx++) {

			if ((yy - center.y) * (yy - center.y) + (xx - center.x) * (xx - center.x) >= radius * radius) continue;

			std::shared_ptr<Chunk> c = getChunkByTileGlobalCoords(xx, yy);
			if (!c) continue;

			std::shared_ptr<Tile> t = c->getTileByTileGlobalCoords(xx, yy);
			if (!t) continue;

			t->_type = type;
			chunksToEdit.insert(c);

			if (getChunkByCoords(c->_coords.x - 1, c->_coords.y - 1)) chunksToEdit.insert(getChunkByCoords(c->_coords.x - 1, c->_coords.y - 1));
			if (getChunkByCoords(c->_coords.x, c->_coords.y - 1)) chunksToEdit.insert(getChunkByCoords(c->_coords.x, c->_coords.y - 1));
			if (getChunkByCoords(c->_coords.x + 1, c->_coords.y - 1)) chunksToEdit.insert(getChunkByCoords(c->_coords.x + 1, c->_coords.y - 1));

			if (getChunkByCoords(c->_coords.x - 1, c->_coords.y)) chunksToEdit.insert(getChunkByCoords(c->_coords.x - 1, c->_coords.y));
			if (getChunkByCoords(c->_coords.x + 1, c->_coords.y)) chunksToEdit.insert(getChunkByCoords(c->_coords.x + 1, c->_coords.y));

			if (getChunkByCoords(c->_coords.x - 1, c->_coords.y + 1)) chunksToEdit.insert(getChunkByCoords(c->_coords.x - 1, c->_coords.y + 1));
			if (getChunkByCoords(c->_coords.x, c->_coords.y + 1)) chunksToEdit.insert(getChunkByCoords(c->_coords.x, c->_coords.y + 1));
			if (getChunkByCoords(c->_coords.x + 1, c->_coords.y + 1)) chunksToEdit.insert(getChunkByCoords(c->_coords.x + 1, c->_coords.y + 1));
		}
	}

	for (auto& c : chunksToEdit)
		c->generateVertexArray(
			getChunkByCoords(c->_coords.x - 1, c->_coords.y - 1), getChunkByCoords(c->_coords.x, c->_coords.y - 1), getChunkByCoords(c->_coords.x + 1, c->_coords.y - 1),
			getChunkByCoords(c->_coords.x - 1, c->_coords.y), getChunkByCoords(c->_coords.x + 1, c->_coords.y),
			getChunkByCoords(c->_coords.x - 1, c->_coords.y + 1), getChunkByCoords(c->_coords.x, c->_coords.y + 1), getChunkByCoords(c->_coords.x + 1, c->_coords.y + 1)
		);
}

std::shared_ptr<Chunk> Map::getChunkByCoords(int x, int y) {
	for (auto& chunk : _chunks) {
		if (chunk->_coords.x == x && chunk->_coords.y == y) {
			return chunk;
		}
	}

	return nullptr;
}

std::shared_ptr<Chunk> Map::getChunkByTileGlobalCoords(int x, int y) {
	x = std::floor((float)x / (float)Chunk::tilesCols);
	y = std::floor((float)y / (float)Chunk::tilesRows);

	for (auto& chunk : _chunks) {
		if (chunk->_coords.x == x && chunk->_coords.y == y) {
			return chunk;
		}
	}

	return nullptr;
}

std::shared_ptr<Chunk> Map::getChunkByGlobalPosition() {

	int x = std::floor((float)MapEditor::editor->_cursor_on_map->_position.x / (float)Tile::tileSize);
	int y = std::floor((float)MapEditor::editor->_cursor_on_map->_position.y / (float)Tile::tileSize);

	return getChunkByTileGlobalCoords(x, y);
}

std::shared_ptr<Tile> Map::getTileByTileGlobalCoords(int x, int y) {

	std::shared_ptr<Chunk> chunk = getChunkByTileGlobalCoords(x, y);

	if (chunk == nullptr) {
		return nullptr;
	}

	return chunk->getTileByTileGlobalCoords(x, y);
}

sf::IntRect Map::getRect() {
	sf::Vector2i minCoords = sf::Vector2i(INT_MAX, INT_MAX);
	sf::Vector2i maxCoords = sf::Vector2i(0, 0);

	for (auto& chunk : _chunks) {
		if (chunk->_coords.x < minCoords.x) minCoords.x = chunk->_coords.x;
		if (chunk->_coords.y < minCoords.y) minCoords.y = chunk->_coords.y;
		if (chunk->_coords.x > maxCoords.x) maxCoords.x = chunk->_coords.x;
		if (chunk->_coords.y > maxCoords.y) maxCoords.y = chunk->_coords.y;
	}


	sf::IntRect rect;
	rect.position = sf::Vector2i(
		minCoords.x * Chunk::tilesCols * Tile::tileSize,
		minCoords.y * Chunk::tilesRows * Tile::tileSize
	);

	rect.size = sf::Vector2i(
		(maxCoords.x - minCoords.x + 1) * Chunk::tilesCols * Tile::tileSize,
		(maxCoords.y - minCoords.y + 1) * Chunk::tilesRows * Tile::tileSize
	);

	return rect;
}

void Map::save(std::ofstream& saver) {
	BinaryWriter writer(saver);

	writer.write_int32((int32_t)_chunks.size());

	for (auto& chunk : _chunks) {
		writer.write_Vector2i(chunk->_coords);

		for (int ty = 0; ty < Chunk::tilesRows; ty++) {
			for (int tx = 0; tx < Chunk::tilesCols; tx++) {
				int index = ty * Chunk::tilesCols + tx;
				writer.write_int8(chunk->_tiles[index]->_type);
			}
		}
	}
}

void Map::load(std::ifstream& loader) {

	// clear all
	_chunks.clear();

	BinaryReader reader(loader);

	int chunksCount = reader.read_int32();

	for(int i=0; i < chunksCount; i++) {

		sf::Vector2i chunkCoords = reader.read_Vector2i();
		std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(chunkCoords.x, chunkCoords.y);

		for (int ty = 0; ty < Chunk::tilesRows; ty++) {
			for (int tx = 0; tx < Chunk::tilesCols; tx++) {
				std::shared_ptr<Tile> tile = std::make_shared<Tile>();
				tile->_type = reader.read_int8();
				tile->_coords.x = chunkCoords.x * Chunk::tilesCols + tx;
				tile->_coords.y = chunkCoords.y * Chunk::tilesRows + ty;
				tile->_position.x = (float)(tile->_coords.x * Tile::tileSize);
				tile->_position.y = (float)(tile->_coords.y * Tile::tileSize);
				chunk->_tiles.push_back(tile);
			}
		}

		_chunks.push_back(chunk);
	}

	for (auto& chunk : _chunks) {
		chunk->generateVertexArray(
			getChunkByCoords(chunk->_coords.x - 1, chunk->_coords.y - 1), getChunkByCoords(chunk->_coords.x, chunk->_coords.y - 1), getChunkByCoords(chunk->_coords.x + 1, chunk->_coords.y - 1),
			getChunkByCoords(chunk->_coords.x - 1, chunk->_coords.y), getChunkByCoords(chunk->_coords.x + 1, chunk->_coords.y),
			getChunkByCoords(chunk->_coords.x - 1, chunk->_coords.y + 1), getChunkByCoords(chunk->_coords.x, chunk->_coords.y + 1), getChunkByCoords(chunk->_coords.x + 1, chunk->_coords.y + 1)
		);
	}
}

void Map::cursorHover() {

	if (MapEditor::editor->_cursor_on_map != nullptr && getRect().contains(MapEditor::editor->_cursor_on_map->_position)) {
		GUI_manager->Element_hovered = shared_from_this();
	}

	_ghostChunk->cursorHover();
}

void Map::handleEvent(const sf::Event& event) {

	if (GUI_manager->Element_hovered.get() == this) {
		if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
			GUI_manager->Element_pressed = shared_from_this();
		}
			
	}

	else if (GUI_manager->Element_pressed.get() == this) {
		if (!(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)))
			GUI_manager->Element_pressed = nullptr;
	}


	_ghostChunk->handleEvent(event);
}

void Map::update() {
	_ghostChunk->update();
}

void Map::draw() {

	std::shared_ptr<MapEditor::Editor> map_editor = std::dynamic_pointer_cast<MapEditor::Editor>(Main::editor_manager->get_back());
	if (!map_editor) return;

	std::shared_ptr<CameraOnMap> camera = map_editor->_camera;
	if (!camera) return;

	Main::render_window->setView(camera->_view);

	for (auto& chunk : _chunks) {
		chunk->draw();
	}

	if (map_editor->_main_menu->_render_chunks_info->_checkbox->_value == 1) {

		float gridWidth = 2.0f;

		// render grid
		sf::Vector2f position_rect;
		position_rect.x = std::max((float)map_editor->_map->getRect().position.x, map_editor->_camera->_position.x-(float)Main::render_window->getSize().x/2);
		position_rect.y = std::max((float)map_editor->_map->getRect().position.y, map_editor->_camera->_position.y-(float)Main::render_window->getSize().y/2);
		position_rect -= sf::Vector2f(gridWidth, gridWidth);

		sf::Vector2f size_rect;
		size_rect.x = std::min((float)Main::render_window->getSize().x, (float)(map_editor->_map->getRect().position.x + map_editor->_map->getRect().size.x - position_rect.x));
		size_rect.y = std::min((float)Main::render_window->getSize().y, (float)(map_editor->_map->getRect().position.y + map_editor->_map->getRect().size.y - position_rect.y));
		size_rect += sf::Vector2f(gridWidth, gridWidth);

		sf::Vector2f mapStart;
		mapStart.x = (float)map_editor->_map->getRect().position.x;
		mapStart.y = (float)map_editor->_map->getRect().position.y;
		
		grid_shader->setUniform("rectPosition", mapStart);
		grid_shader->setUniform("gridSize", 16.f);
		grid_shader->setUniform("chunkSize", 16.0f);
		grid_shader->setUniform("gridWidth", gridWidth);
		grid_shader->setUniform("gridMainColor", sf::Glsl::Vec4(sf::Color(7, 7, 7, 255)));
		grid_shader->setUniform("gridSecondColor", sf::Glsl::Vec4(sf::Color(47, 47, 47, 255)));

		sf::RenderStates rs;
		rs.shader = &*grid_shader;

		sf::RectangleShape rect(size_rect);
		rect.setPosition(position_rect);
		rect.setFillColor(sf::Color(127, 47, 47, 127));
		Main::render_window->draw(rect, rs);

		for (auto& chunk : _chunks) {
			chunk->drawCoords();
		}
	}

	_ghostChunk->draw();

}

int getNeighbourTypeLocal(
	int nx, int ny,
	const std::shared_ptr<Chunk>& chunk,
	const std::shared_ptr<Chunk>& leftTopChunk,
	const std::shared_ptr<Chunk>& topChunk,
	const std::shared_ptr<Chunk>& rightTopChunk,
	const std::shared_ptr<Chunk>& leftChunk,
	const std::shared_ptr<Chunk>& rightChunk,
	const std::shared_ptr<Chunk>& leftBottomChunk,
	const std::shared_ptr<Chunk>& bottomChunk,
	const std::shared_ptr<Chunk>& rightBottomChunk
)
{
	constexpr int EMPTY_TYPE = -1;

	if (nx >= 0 && nx < chunk->tilesCols && ny >= 0 && ny < chunk->tilesRows) {
		return chunk->_tiles[ny * chunk->tilesCols + nx]->_type;
	}

	const int dx = (nx < 0) ? -1 : (nx >= chunk->tilesCols ? 1 : 0);
	const int dy = (ny < 0) ? -1 : (ny >= chunk->tilesRows ? 1 : 0);

	std::shared_ptr<Chunk> c = nullptr;

	if (dx == -1 && dy == -1) c = leftTopChunk;
	else if (dx == 0 && dy == -1) c = topChunk;
	else if (dx == 1 && dy == -1) c = rightTopChunk;

	else if (dx == -1 && dy == 0) c = leftChunk;
	else if (dx == 1 && dy == 0) c = rightChunk;

	else if (dx == -1 && dy == 1) c = leftBottomChunk;
	else if (dx == 0 && dy == 1) c = bottomChunk;
	else if (dx == 1 && dy == 1) c = rightBottomChunk;

	if (!c) return EMPTY_TYPE;

	int lx = nx;
	int ly = ny;

	if (lx < 0) lx += chunk->tilesCols;
	else if (lx >= chunk->tilesCols) lx -= chunk->tilesCols;

	if (ly < 0) ly += chunk->tilesRows;
	else if (ly >= chunk->tilesRows) ly -= chunk->tilesRows;
	return c->_tiles[ly * chunk->tilesCols + lx]->_type;
}

int getTileType(
	int type,
	int x, int y,
	std::shared_ptr<Chunk> chunk,
	std::shared_ptr<Chunk> leftTopChunk,
	std::shared_ptr<Chunk> topChunk,
	std::shared_ptr<Chunk> rightTopChunk,
	std::shared_ptr<Chunk> leftChunk,
	std::shared_ptr<Chunk> rightChunk,
	std::shared_ptr<Chunk> leftBottomChunk,
	std::shared_ptr<Chunk> bottomChunk,
	std::shared_ptr<Chunk> rightBottomChunk) {

	int tileType = 5; // domyślnie środek

	// Sprawdzanie sąsiadów z uwzględnieniem krawędzi
	bool hasLeft = areFriends(type, getNeighbourTypeLocal(
		x - 1, y, chunk,
		leftTopChunk, topChunk, rightTopChunk,
		leftChunk, rightChunk,
		leftBottomChunk, bottomChunk, rightBottomChunk
	));

	bool hasRight = areFriends(type, getNeighbourTypeLocal(
		x + 1, y, chunk,
		leftTopChunk, topChunk, rightTopChunk,
		leftChunk, rightChunk,
		leftBottomChunk, bottomChunk, rightBottomChunk
	));

	bool hasTop = areFriends(type, getNeighbourTypeLocal(
		x, y - 1, chunk,
		leftTopChunk, topChunk, rightTopChunk,
		leftChunk, rightChunk,
		leftBottomChunk, bottomChunk, rightBottomChunk
	));

	bool hasBottom = areFriends(type, getNeighbourTypeLocal(
		x, y + 1, chunk,
		leftTopChunk, topChunk, rightTopChunk,
		leftChunk, rightChunk,
		leftBottomChunk, bottomChunk, rightBottomChunk
	));

	// Narożniki:
	bool hasTopLeft = areFriends(type, getNeighbourTypeLocal(
		x - 1, y - 1, chunk,
		leftTopChunk, topChunk, rightTopChunk,
		leftChunk, rightChunk,
		leftBottomChunk, bottomChunk, rightBottomChunk
	));

	bool hasTopRight = areFriends(type, getNeighbourTypeLocal(
		x + 1, y - 1, chunk,
		leftTopChunk, topChunk, rightTopChunk,
		leftChunk, rightChunk,
		leftBottomChunk, bottomChunk, rightBottomChunk
	));

	bool hasBottomLeft = areFriends(type, getNeighbourTypeLocal(
		x - 1, y + 1, chunk,
		leftTopChunk, topChunk, rightTopChunk,
		leftChunk, rightChunk,
		leftBottomChunk, bottomChunk, rightBottomChunk
	));

	bool hasBottomRight = areFriends(type, getNeighbourTypeLocal(
		x + 1, y + 1, chunk,
		leftTopChunk, topChunk, rightTopChunk,
		leftChunk, rightChunk,
		leftBottomChunk, bottomChunk, rightBottomChunk
	));

	// Logika dobierania odpowiedniego fragmentu
	if (!hasTop && !hasLeft) {
		if (hasRight && hasBottom && !hasBottomRight) {
			tileType = 13; // wewnętrzny lewo-góra
		}
		else {
			tileType = 1; // standardowy lewo-góra
		}
	}
	else if (!hasTop && !hasRight) {
		tileType = 3; // prawo-góra
	}
	else if (!hasBottom && !hasLeft) {
		tileType = 7; // lewo-dół
	}
	else if (!hasBottom && !hasRight) {
		tileType = 9; // prawo-dół
	}
	else if (!hasTop) {
		tileType = 2; // góra
	}
	else if (!hasLeft) {
		tileType = 4; // lewo
	}
	else if (!hasRight) {
		tileType = 6; // prawo
	}
	else if (!hasBottom) {
		tileType = 8; // dół
	}
	else {
		// Dla pełnego otoczenia - sprawdź wewnętrzne narożniki
		if (!hasTopLeft && hasTop && hasLeft) {
			tileType = 11; // wewnętrzny lewo-góra dla środka
		}
		else if (!hasTopRight && hasTop && hasRight) {
			tileType = 12; // wewnętrzny prawo-góra dla środka
		}
		else if (!hasBottomLeft && hasBottom && hasLeft) {
			tileType = 13; // wewnętrzny lewo-dół dla środka
		}
		else if (!hasBottomRight && hasBottom && hasRight) {
			tileType = 14; // wewnętrzny prawo-dół dla środka
		}
		else {
			tileType = 5; // środek
		}
	}

	return tileType;
}