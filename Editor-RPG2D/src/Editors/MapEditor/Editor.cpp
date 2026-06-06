#include "Editors/MapEditor/Editor.hpp"
#include "DebugLog.hpp"
#include "Objects/Monster.hpp"
#include "PrefabsManager.hpp"
#include "WindowsManager.hpp"

namespace MapEditor {

	Editor::Editor() {

	}

	Editor::~Editor() {

	}

	void Editor::createTileset() {

		_tileset = std::make_shared<Tileset>();

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

		std::shared_ptr<GameObjectOnMap> golem1 = std::make_shared<Monster>(prefabs_manager->getPrefab(L"golem"));
		std::shared_ptr<GameObjectOnMap> golem2 = std::make_shared<Monster>(prefabs_manager->getPrefab(L"golem"));
		std::shared_ptr<GameObjectOnMap> golem3 = std::make_shared<Monster>(prefabs_manager->getPrefab(L"golem"));
		std::shared_ptr<GameObjectOnMap> golem4 = std::make_shared<Monster>(prefabs_manager->getPrefab(L"golem"));

		std::shared_ptr<GameObjectOnMap> dziobak1 = std::make_shared<Monster>(prefabs_manager->getPrefab(L"dziobak"));
		std::shared_ptr<GameObjectOnMap> goblin1 = std::make_shared<Monster>(prefabs_manager->getPrefab(L"goblin"));

		golem1->setPosition(sf::Vector2i(256, 128));
		golem2->setPosition(sf::Vector2i(1024, 128));
		golem3->setPosition(sf::Vector2i(256, 512));
		golem4->setPosition(sf::Vector2i(1024, 512));

		dziobak1->setPosition(sf::Vector2i(576 - 128, 320 + 64));
		goblin1->setPosition(sf::Vector2i(576 + 128, 320 + 64));

		std::shared_ptr<GameObjectOnMap> tree1 = std::make_shared<GameObjectOnMap>(prefabs_manager->getPrefab(L"tree_1"));
		std::shared_ptr<GameObjectOnMap> tree2 = std::make_shared<GameObjectOnMap>(prefabs_manager->getPrefab(L"tree_1"));
		std::shared_ptr<GameObjectOnMap> tree3 = std::make_shared<GameObjectOnMap>(prefabs_manager->getPrefab(L"tree_1"));
		std::shared_ptr<GameObjectOnMap> tree4 = std::make_shared<GameObjectOnMap>(prefabs_manager->getPrefab(L"tree_1"));

		std::shared_ptr<GameObjectOnMap> boulder1 = std::make_shared<GameObjectOnMap>(prefabs_manager->getPrefab(L"boulder_1"));
		std::shared_ptr<GameObjectOnMap> boulder2 = std::make_shared<GameObjectOnMap>(prefabs_manager->getPrefab(L"boulder_2"));

		sf::IntRect rect = _map->getRect();

		tree1->setPosition(sf::Vector2i(0, 0));
		tree2->setPosition(sf::Vector2i(rect.position.x + rect.size.x - 256, 0));
		tree3->setPosition(sf::Vector2i(0, rect.position.y + rect.size.y - 256));
		tree4->setPosition(sf::Vector2i(rect.position.x + rect.size.x - 256, rect.position.y + rect.size.y - 256));

		boulder1->setPosition(sf::Vector2i(rect.position.x + rect.size.x / 2 - 128, rect.position.y + rect.size.y / 2 - 128));
		boulder2->setPosition(sf::Vector2i(rect.position.x + rect.size.x / 2 + 128, rect.position.y + rect.size.y / 2 - 128));

		//_game_objects->addGameObject(golem1);
		//_game_objects->addGameObject(golem2);
		//_game_objects->addGameObject(golem3);
		//_game_objects->addGameObject(golem4);
		//_game_objects->addGameObject(dziobak1);
		//_game_objects->addGameObject(goblin1);
		//_game_objects->addGameObject(tree1);
		//_game_objects->addGameObject(tree2);
		//_game_objects->addGameObject(tree3);
		//_game_objects->addGameObject(tree4);
		//_game_objects->addGameObject(boulder1);
		//_game_objects->addGameObject(boulder2);
	}

	void Editor::createCamera() {

		_camera = std::make_shared<CameraOnMap>();

		_camera->_position = sf::Vector2f((float)(_map->getRect().size.x / 2), (float)(_map->getRect().size.y / 2));
		_camera->update();
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

		_camera->handleEvent(event);
		_map->handleEvent(event);
		_cursor_on_map->handleEvent(event);
		_palette->handleEvent(event);


	}

	void Editor::update() {

		_game_objects->update();

		_game_objects->sort();

		_main_menu->update();
		_palette->update();

		if (!Main::windows_manager->get_back() && _main_menu->_state == Components::MainMenuStates::Closed) {
			_camera->update();
			_cursor_on_map->update();
		}
		
		_map->update();

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