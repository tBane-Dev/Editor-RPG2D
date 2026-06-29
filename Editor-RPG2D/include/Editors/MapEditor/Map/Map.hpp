#pragma once
#include <SFML/Graphics.hpp>
#include <set>
#include "Cursor.hpp"
#include "Tileset.hpp"
#include "TexturesManager.hpp"
#include "ShadersManager.hpp"
#include "Time.hpp"
#include "GUIManager.hpp"

class Tile {
public:
	const static int tileSize = 16;
	const static int borderWidth = 0;
	sf::Vector2i _coords;
	sf::Vector2f _position;
	int _type;

	Tile();
	~Tile();
};

sf::Vector2i worldToTile(sf::Vector2i position);

class Chunk : public std::enable_shared_from_this<Chunk> {
public:
	const static int tilesCols = 16;
	const static int tilesRows = 16;
	sf::Vector2i _coords;
	std::vector<std::shared_ptr<Tile>> _tiles;
	sf::VertexArray _vertexArray;
	std::unique_ptr<sf::Text> _coordsText;

	Chunk(int x, int y);
	~Chunk();

	bool operator<(const Chunk& other) const;
	std::shared_ptr<Tile> getTileByTileGlobalCoords(int x, int y);
	std::shared_ptr<Tile> getTileByGlobalPosition();

	void generateVertexArray(
		std::shared_ptr<Chunk> leftTopChunk,
		std::shared_ptr<Chunk> topChunk,
		std::shared_ptr<Chunk> rightTopChunk,
		std::shared_ptr<Chunk> leftChunk,
		std::shared_ptr<Chunk> rightChunk,
		std::shared_ptr<Chunk> leftBottomChunk,
		std::shared_ptr<Chunk> bottomChunk,
		std::shared_ptr<Chunk> rightBottomChunk
	);

	void drawCoords();
	void draw();
};

class Map : public Element {
public:
	std::vector<std::shared_ptr<Chunk>> _chunks;

	Map();
	~Map();

	void create(int width, int height);

	void drawCircle(sf::Vector2i center, int radius, int type);

	std::shared_ptr<Chunk> getChunkByCoords(int x, int y);
	std::shared_ptr<Chunk> getChunkByTileGlobalCoords(int x, int y);
	std::shared_ptr<Chunk> getChunkByGlobalPosition();
	std::shared_ptr<Tile> getTileByTileGlobalCoords(int x, int y);
	sf::IntRect getRect();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void draw();
};


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
);

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
	std::shared_ptr<Chunk> rightBottomChunk
);
