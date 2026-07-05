#pragma once
#include <SFML/Graphics.hpp>
#include "Editors/MapEditor/Map/Tile.hpp"
#include "Editors/MapEditor/Map/GameObjectOnMap.hpp"

class Chunk : public std::enable_shared_from_this<Chunk> {
public:
	const static int tilesCols = 16;
	const static int tilesRows = 16;
	sf::Vector2i _coords;
	std::vector<std::shared_ptr<Tile>> _tiles;
	sf::VertexArray _vertexArray;
	bool _isVisible;
	std::unique_ptr<sf::Text> _coordsText;

	std::vector<std::shared_ptr<GameObjectOnMap>> _gameObjectsOnMap;
	sf::IntRect _gameObjectsOnMapRect;

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

	sf::IntRect getRect();
	void calculateGameObjectOnMapRect();
	sf::IntRect getGameObjectsOnMapRect();

	void addGameObjectOnMap(std::shared_ptr<GameObjectOnMap> gameObjectOnMap);
	void removeGameObjectOnMap(std::shared_ptr<GameObjectOnMap> gameObjectOnMap);
	
	void setVisible();

	void drawCoords();
	void draw();
};
