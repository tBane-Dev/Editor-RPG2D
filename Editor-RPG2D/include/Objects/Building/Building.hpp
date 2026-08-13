#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.hpp"
#include "Editors/MapEditor/Map/GameObjectOnMap.hpp"
#include "Objects/Wall.hpp"
#include "Objects/Building/Roof.hpp"

class BuildingPrefab : public GameObject {
public:

	static std::shared_ptr<Texture> _floorset;

	std::vector<std::vector<int>> _floor;
	std::vector<std::vector<int>> _walls;

	int _wallHeight = 3;

	sf::VertexArray _floorVertexArray;
	std::vector<std::shared_ptr<Wall>> _wallsObjects;
	std::shared_ptr<Roof> _roof;

	BuildingPrefab(std::wstring name, sf::Vector2i size);
	BuildingPrefab(std::wstring name, const BuildingPrefab& other);
	~BuildingPrefab();

	void generate(sf::Vector2i position, float scale = 1.0f, std::shared_ptr<Building> building = nullptr);

	void generateFloorVertexArray(float scale = 1.0f);
	void generateWalls(sf::Vector2i position, float scale = 1.0f, std::shared_ptr<Building> building = nullptr);
	void generateRoofs(sf::Vector2i position, float scale = 1.0f);
	void generateCollider(float scale = 1.0f);
	void generateMesh(float scale = 1.0f);

	void drawOnlyCollider(sf::RenderTarget& target, sf::Vector2i position);
	void drawOnlyFloor(sf::RenderTarget& target, sf::Vector2i position);
	void drawOnlyWalls(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f, int drawType = -1);
	void drawOnlyRoof(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f, std::shared_ptr<Building> building = nullptr);

	std::shared_ptr<sf::Texture> getPreviewOutsideTexture(bool drawOutside);
};

class Building : public GameObjectOnMap {
public:


	bool _renderOutsideLook;
	std::vector<std::shared_ptr<Wall>> _wallsObjects;
	

	Building(std::weak_ptr<GameObject> prefab);
	~Building();

	void generate();

	virtual void setPosition(sf::Vector2i position);
	
	void loadPrefab(std::shared_ptr<BuildingPrefab> buildingPrefab);
	

	void addWallsToGameObjects();
	void removeWallsFromGameObjects();

	

	virtual void cursorHover();
	virtual void update();
	virtual void draw(); // draw only roof
};