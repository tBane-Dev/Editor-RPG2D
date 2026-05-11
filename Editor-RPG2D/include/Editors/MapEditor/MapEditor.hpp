#pragma once
#include "Editors/Editor.hpp"
#include "Editors/MapEditor/Map/Tileset.hpp"
#include "Editors/MapEditor/Map/Map.hpp"
#include "Editors/MapEditor/Map/CameraOnMap.hpp"
#include "Editors/MapEditor/Map/GameObjectsOnMap.hpp"
#include "Editors/MapEditor/MapEditorMainMenu.hpp"

class MapEditor : public Editor {
public:
	std::shared_ptr<MapEditorMainMenu> _main_menu;
	//std::shared_ptr<Palette> _palette;

	std::shared_ptr<Tileset> _tileset;
	std::shared_ptr<Map> _map;
	std::shared_ptr<GameObjectsOnMap> _game_objects;
	std::shared_ptr<CameraOnMap> _camera;

	MapEditor();
	~MapEditor();

	void createTileset();
	void createMap(int width, int height);
	void createGameObjects();
	void createCamera();
	void createMainMenu();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};

extern std::shared_ptr<MapEditor> map_editor;