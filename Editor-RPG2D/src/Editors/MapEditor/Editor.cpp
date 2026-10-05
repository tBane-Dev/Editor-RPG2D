#include "Editors/MapEditor/Editor.hpp"
#include "DebugLog.hpp"
#include "Objects/Monster.hpp"
#include "Objects/Nature.hpp"
#include "PrefabsManager.hpp"
#include "WindowsManager.hpp"

namespace MapEditor {

	Editor::Editor() {

	}

	Editor::~Editor() {

	}

	void Editor::createMap(int width, int height) {

		_map = std::make_shared<Map>();

		_map->create(width, height);

		_map->drawCircle(sf::Vector2i(24, 28), 16, 4);
		_map->drawCircle(sf::Vector2i(48, 16), 16, 4);
		_map->drawCircle(sf::Vector2i(42, 28), 16, 4);
	}

	void Editor::createCursorOnMap() {
		_cursor_on_map = std::make_shared<CursorOnMap>();
	}

	void Editor::createGameObjects() {
		_game_objects = std::make_shared<PlacedGameObjects>();
	}

	void Editor::createCamera() {

		_camera = std::make_shared<CameraOnMap>();

		_camera->_position = sf::Vector2f((float)(_map->getRect().size.x / 2), (float)(_map->getRect().size.y / 2));
		_camera->update();
	}

	void Editor::setVisibleChunks() {
		_map->setVisibleChunks();

		auto& visibleObjects = MapEditor::editor->_game_objects->_visiblePlacedGameObjects;

		for (const auto& selectedObject :
			MapEditor::editor->_cursor_on_map->_selectedObjects) {

			auto object = selectedObject->_object;
			if (!object)
				continue;

			if (
				std::find(visibleObjects.begin(), visibleObjects.end(), object) == visibleObjects.end()) {
				
				visibleObjects.push_back(selectedObject->_object);

				if (selectedObject->_object->_type == ObjectType::Building) {
					std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(selectedObject->_object);
					b->addDoorsToVisibleGameObjects(MapEditor::editor);
					b->addWallsToVisibleGameObjects(MapEditor::editor);
					b->addWindowsToVisibleGameObjects(MapEditor::editor);
					b->addSkeletsToVisibleGameObjects(MapEditor::editor);
					b->addWallMountedToVisibleGameObjects(MapEditor::editor);
					b->addOutsideToVisibleGameObjects(MapEditor::editor);
				}
			}
		}
	}

	void Editor::createMainMenu() {
		_main_menu = std::make_shared<MainMenu>();
	}

	void Editor::createPalette() {
		_palette = std::make_shared<Palette>();
	}

	void Editor::cursorHover() {

		if (Main::windows_manager->get_back())
			return;

		if (_main_menu->_state != Components::MainMenuStates::Closed) {
			_main_menu->cursorHover();
			return;
		}

		_game_objects->_hoveredPlacedGameObject = std::weak_ptr<PlacedGameObject>();
		_game_objects->cursorHover();

		_map->cursorHover();
		_palette->cursorHover();
		_main_menu->cursorHover();
	}

	void Editor::handleEvent(const sf::Event& event) {

		if (Main::windows_manager->get_back())
			return;

		if(const auto& kp = event.getIf<sf::Event::KeyPressed>(); kp && kp->code == sf::Keyboard::Key::Delete) {
			
			if (!_cursor_on_map->_selectedObjects.empty()) {

				for(auto& selectedObject : _cursor_on_map->_selectedObjects) {
					if (auto object = selectedObject->_object) {
						_game_objects->removeGameObject(object);
						for(auto& chunk : _map->_chunks) {
							chunk->removePlacedGameObject(object);
						}
					}
				}

				return;
			}
			
		}

		_main_menu->handleEvent(event);

		if (_main_menu->_state != Components::MainMenuStates::Closed)
			return;

		_camera->handleEvent(event);

		_map->handleEvent(event);
		_cursor_on_map->handleEvent(event);
		_palette->handleEvent(event);


	}

	void Editor::update() {

		_main_menu->update();
		_palette->update();

		_camera->update();
		_cursor_on_map->update();
		
		_map->update();

		_game_objects->update();

		_game_objects->sort();

	}

	void Editor::draw() {

		_camera->setView();
		_map->draw();
		_game_objects->draw();
		_cursor_on_map->draw();

		GUI_manager->setView();
		_main_menu->draw();
		_palette->draw();
	}

	std::shared_ptr<Editor> editor = nullptr;
}