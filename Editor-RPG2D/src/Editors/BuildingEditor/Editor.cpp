#include "Editors/BuildingEditor/Editor.hpp"
//#include "Editors/PrefabsEditor/Palette.hpp"
#include "DebugLog.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "WindowsManager.hpp"

namespace BuildingsEditor {

	Editor::Editor() {
		_margin = sf::Vector2i(32, 64);

		_title = std::make_unique<sf::Text>(basicFont, L"Building Editor", 28);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(32, 48));

	}

	Editor::~Editor() {

	}

	void Editor::init() {
		//createMainMenu();
		//createPalette();
		
	}

	void Editor::createMainMenu() {
		//_main_menu = std::make_shared<MainMenu>();
	}

	void Editor::createPalette() {
		//_palette = std::make_shared<Palette>();
	}


	void Editor::cursorHover() {

		//if(Main::windows_manager->get_back())
		//	return;
		//
		//if (_main_menu->_state != Components::MainMenuStates::Closed) {
		//	_main_menu->cursorHover();
		//	return;
		//}
		//
		//_palette->cursorHover();
		//_main_menu->cursorHover();

	}

	void Editor::handleEvent(const sf::Event& event) {

		//if (Main::windows_manager->get_back())
		//	return;
		//
		//_main_menu->handleEvent(event);
		//
		//if (_main_menu->_state != Components::MainMenuStates::Closed) {
		//	return;
		//}
		//
		//_palette->handleEvent(event);
		//_main_panel->handleEvent(event);

	}

	void Editor::update() {

		//_main_menu->update();
		//_palette->update();

	}

	void Editor::draw() {

		GUI_manager->setView();

		Main::render_window->draw(*_title);

		//_palette->draw();
		//_main_menu->draw();
	}

	std::shared_ptr<Editor> editor = nullptr;
}