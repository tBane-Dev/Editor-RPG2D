#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.hpp"
#include "Editors/MapEditor/Map/GameObjectOnMap.hpp"
#include "Objects/Wall.hpp"
#include "Objects/Building/Roof.hpp"

class BuildingPrefab : public GameObject {
public:

	static std::shared_ptr<Texture> _floorset;

	std::vector<int> _floor;
	sf::Vector2i _floorSize;

	std::vector<int> _walls;
	sf::Vector2i _wallsSize;

	int _wallHeight = 3;

	BuildingPrefab(std::wstring name, sf::Vector2i size);
	BuildingPrefab(std::wstring name, const BuildingPrefab& other);
	~BuildingPrefab();
};

class Building : public GameObjectOnMap {
public:


	bool _renderOutsideLook;

	sf::VertexArray _floorVertexArray;
	std::vector<std::shared_ptr<Wall>> _wallsObjects;
	std::shared_ptr<Roof> _roof;

	Building(std::weak_ptr<GameObject> prefab);
	~Building();

	void generate();

	virtual void setPosition(sf::Vector2i position);
	void generateFloorVertexArray(float scale = 1.0f);
	void generateWalls(float scale = 1.0f, bool renderOutsideLook = false);	
	void loadPrefab(std::shared_ptr<BuildingPrefab> buildingPrefab);
	void generateRoofs(float scale = 1.0f);
	void generateCollider(float scale = 1.0f);

	void addWallsToGameObjects();
	void removeWallsFromGameObjects();

	void drawOnlyCollider();
	void drawOnlyFloor(sf::RenderTarget& target, sf::Vector2i position);
	void drawOnlyWalls(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f);
	void drawOnlyRoof(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f);

	virtual void cursorHover();
	virtual void update();
	virtual void draw(); // draw only roof
};