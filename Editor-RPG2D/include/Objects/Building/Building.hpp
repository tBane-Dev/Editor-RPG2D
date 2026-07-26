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

	int _wallHeight = 128;

	BuildingPrefab(std::wstring name, sf::Vector2i size);
	BuildingPrefab(std::wstring name, const BuildingPrefab& other);
	~BuildingPrefab();
};

class Building : public GameObjectOnMap {
public:

	sf::VertexArray _floorVertexArray;
	std::vector<std::shared_ptr<Wall>> _wallsObjects;

	std::shared_ptr<Roof> _roof;
	//TO-DO - to delete
	//std::vector<int> _roof;
	//sf::Vector2i _roofSize;
	//sf::VertexArray _roofVertexArray;
	//std::vector<std::shared_ptr<Roof>> _roofsObjects;
	//sf::VertexArray _roofOverhangVertexArray;

	Building(std::weak_ptr<GameObject> prefab);
	~Building();

	virtual void setPosition(sf::Vector2i position);
	void generateFloorVertexArray(float scale = 1.0f);
	void generateWalls(float scale = 1.0f, bool renderOutsideLook = false);	
	void loadPrefab(std::shared_ptr<BuildingPrefab> buildingPrefab);
	void generateRoofs(float scale = 1.0f);
};