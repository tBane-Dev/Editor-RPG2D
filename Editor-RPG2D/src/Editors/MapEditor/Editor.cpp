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
		_game_objects = std::make_shared<GameObjectsOnMap>();

		std::shared_ptr<Map> map = MapEditor::editor->_map;
		sf::IntRect mapRect = map->getRect();
		sf::Vector2i texSize = sf::Vector2i(prefabs_manager->getPrefab(L"tree_1")->getAnimations().lock()->getTexture()->_texture->getSize());

		std::shared_ptr<GameObjectOnMap> tree_1 = std::make_shared<Nature>(prefabs_manager->getPrefab(L"tree_1"));
		tree_1->setPosition(sf::Vector2i(0, 0));
		MapEditor::editor->_map->getChunkByGlobalPosition(tree_1->getPosition())->addGameObjectOnMap(tree_1);

		std::shared_ptr<GameObjectOnMap> tree_2 = std::make_shared<Nature>(prefabs_manager->getPrefab(L"tree_1"));
		tree_2->setPosition(sf::Vector2i(mapRect.size.x - texSize.x, 0));
		MapEditor::editor->_map->getChunkByGlobalPosition(tree_2->getPosition())->addGameObjectOnMap(tree_2);

		std::shared_ptr<GameObjectOnMap> tree_3 = std::make_shared<Nature>(prefabs_manager->getPrefab(L"tree_1"));
		tree_3->setPosition(sf::Vector2i(0, mapRect.size.y - texSize.y));
		MapEditor::editor->_map->getChunkByGlobalPosition(tree_3->getPosition())->addGameObjectOnMap(tree_3);

		std::shared_ptr<GameObjectOnMap> tree_4 = std::make_shared<Nature>(prefabs_manager->getPrefab(L"tree_1"));
		tree_4->setPosition(sf::Vector2i(mapRect.size.x - texSize.x, mapRect.size.y - texSize.y));
		MapEditor::editor->_map->getChunkByGlobalPosition(tree_4->getPosition())->addGameObjectOnMap(tree_4);


		//for (int i = 0; i < 1000; i += 1) {
		//	std::shared_ptr<GameObjectOnMap> boulder = std::make_shared<Nature>(prefabs_manager->getPrefab(L"boulder_1"));
		//	boulder->setPosition(sf::Vector2i(rand() % mapRect.size.x, rand() % mapRect.size.y));
		//	MapEditor::editor->_map->getChunkByGlobalPosition(boulder->getPosition())->addGameObjectOnMap(boulder);
		//}
		
	}

	void Editor::createCamera() {

		_camera = std::make_shared<CameraOnMap>();

		_camera->_position = sf::Vector2f((float)(_map->getRect().size.x / 2), (float)(_map->getRect().size.y / 2));
		_camera->update();
	}

	void Editor::setVisibleChunks() {
		_map->setVisibleChunks();
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

		_game_objects->_hoveredGameObjectOnMap = std::weak_ptr<GameObjectOnMap>();
		_game_objects->cursorHover();

		_map->cursorHover();
		_palette->cursorHover();
		_main_menu->cursorHover();
	}

	void Editor::handleEvent(const sf::Event& event) {

		if (Main::windows_manager->get_back())
			return;

		_main_menu->handleEvent(event);

		if (_main_menu->_state != Components::MainMenuStates::Closed)
			return;

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