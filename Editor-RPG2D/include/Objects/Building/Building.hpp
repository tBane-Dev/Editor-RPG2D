#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.hpp"
#include "Objects/PlacedGameObject.hpp"
#include "Objects/Building/Wall.hpp"
#include "Objects/Building/Door.hpp"
#include "Objects/Building/Window.hpp"
#include "Objects/Building/WallMounted.hpp"
#include "Objects/Building/Skelet.hpp"
#include "Objects/Building/Roof.hpp"
#include "Objects/Building/Outside.hpp"
#include "EditorsManager.hpp"

class BuildingPrefab : public GameObject {
public:

	static std::shared_ptr<Texture> _floorset;

	std::vector<std::vector<int>> _floor;
	std::vector<std::vector<int>> _walls;
	std::vector<std::shared_ptr<Door>> _doors;
	std::vector<std::shared_ptr<Window>> _windows;
	std::vector<std::shared_ptr<WallMounted>> _wallMounted;

	int _wallHeight = 3;
	int _skeletType = 0;

	// generated
	sf::VertexArray _floorVertexArray;
	std::vector<std::shared_ptr<Wall>> _wallsObjects;
	std::vector<std::shared_ptr<Skelet>> _skeletObjects;
	std::shared_ptr<Roof> _roof;

	// generated preview textures
	std::shared_ptr<sf::Texture> _insideTexture;
	std::shared_ptr<sf::Texture> _outsideTexture;

	BuildingPrefab(std::wstring name, sf::Vector2i size);
	BuildingPrefab(std::wstring name, const BuildingPrefab& other);
	~BuildingPrefab();

	void generate(sf::Vector2i position, float scale = 1.0f, std::shared_ptr<Building> building = nullptr);
	void generateFloorVertexArray(float scale = 1.0f);
	void generateWalls(sf::Vector2i position, float scale = 1.0f, std::shared_ptr<Building> building = nullptr);
	void generateRoofs(sf::Vector2i position, float scale = 1.0f);
	void generateSkelet(sf::Vector2i position, float scale = 1.0f, std::shared_ptr<Building> building = nullptr);
	void generateCollider(float scale = 1.0f);
	void generateMesh(float scale = 1.0f);


	void copyDoorsFromPrefab(sf::Vector2i position, float scale, std::shared_ptr<Building> building);
	void copyWindowsFromPrefab(sf::Vector2i position, float scale, std::shared_ptr<Building> building);
	void copyWallMountedFromPrefab(sf::Vector2i position, float scale, std::shared_ptr<Building> building);
	void copyWallsFromPrefab(sf::Vector2i position, float scale, std::shared_ptr<Building> building);
	void copySkeletFromPrefab(sf::Vector2i position, float scale, std::shared_ptr<Building> building);

	void generatePreviewTexture(std::shared_ptr<sf::Texture>& texture, bool drawOutside = true);
	void generatePreviewTextures();
	std::shared_ptr<sf::Texture> getPreviewInsideTexture();
	std::shared_ptr<sf::Texture> getPreviewOutsideTexture();

	void drawOnlyCollider(sf::RenderTarget& target, sf::Vector2i position);
	void drawOnlyFloor(sf::RenderTarget& target, sf::Vector2i position);
	void drawOnlyWalls(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f, int drawType = -1);
	void drawOnlySkelet(sf::RenderTarget& target, sf::Vector2i botttomPosition, float scale = 1.0f, int drawType = -1);
	void drawOnlyRoof(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f, std::shared_ptr<Building> building = nullptr);
	void drawOutsideLook(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f, std::shared_ptr<Building> building = nullptr);
};

class Building : public PlacedGameObject {
public:


	bool _renderOutsideLook;
	std::vector<std::shared_ptr<Door>> _doorsObjects;
	std::vector<std::shared_ptr<Window>> _windowsObjects;
	std::vector<std::shared_ptr<WallMounted>> _wallMountedObjects;
	std::vector<std::shared_ptr<Wall>> _wallsObjects;
	std::vector<std::shared_ptr<Skelet>> _skeletsObjects;
	std::shared_ptr<Outside> _outsideObject;

	Building(std::weak_ptr<GameObject> prefab);
	~Building();

	void generate();

	virtual void setPosition(sf::Vector2i position);
	
	void loadPrefab(std::shared_ptr<BuildingPrefab> buildingPrefab);
	
	void addDoorsToGameObjects(std::shared_ptr<Main::Editor> editor);
	void addWindowsToGameObjects(std::shared_ptr<Main::Editor> editor);
	void addWallMountedToGameObjects(std::shared_ptr<Main::Editor> editor);
	void addWallsToGameObjects(std::shared_ptr<Main::Editor> editor);
	void addSkeletsToGameObjects(std::shared_ptr<Main::Editor> editor);
	void addOutsideToGameObjects(std::shared_ptr<Main::Editor> editor);

	void removeDoorsFromGameObjects(std::shared_ptr<Main::Editor> editor);
	void removeWindowsFromGameObjects(std::shared_ptr<Main::Editor> editor);
	void removeWallMountedFromGameObjects(std::shared_ptr<Main::Editor> editor);
	void removeWallsFromGameObjects(std::shared_ptr<Main::Editor> editor);
	void removeSkeletsFromGameObjects(std::shared_ptr<Main::Editor> editor);
	void removeOutsideFromGameObjects(std::shared_ptr<Main::Editor> editor);

	void addDoorsToVisibleGameObjects(std::shared_ptr<Main::Editor> editor);
	void addWindowsToVisibleGameObjects(std::shared_ptr<Main::Editor> editor);
	void addWallMountedToVisibleGameObjects(std::shared_ptr<Main::Editor> editor);
	void addWallsToVisibleGameObjects(std::shared_ptr<Main::Editor> editor);
	void addSkeletsToVisibleGameObjects(std::shared_ptr<Main::Editor> editor);
	void addOutsideToVisibleGameObjects(std::shared_ptr<Main::Editor> editor);

	virtual void cursorHover();
	virtual void update();
	virtual void draw(); // draw only frame
};