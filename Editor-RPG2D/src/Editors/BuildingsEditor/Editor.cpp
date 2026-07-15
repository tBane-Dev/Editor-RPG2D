#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/Palette.hpp"
#include "DebugLog.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "WindowsManager.hpp"

namespace BuildingsEditor {

	Editor::Editor() {
		_margin = sf::Vector2i(32, 64);

		_title = std::make_unique<sf::Text>(basicFont, L"Buildings Editor", 28);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(32, 48));

	}

	Editor::~Editor() {

	}

	void Editor::init() {
		createMainMenu();
		createPalette();
		createListPanel();
		createBuildingPanel();
	}

	void Editor::createMainMenu() {
		_main_menu = std::make_shared<MainMenu>();
	}

	void Editor::createPalette() {
		_palette = std::make_shared<Palette>();
	}

	void Editor::createListPanel() {
		_list_panel = std::make_shared<BuildingsListPanel>(_margin);
	}

	void Editor::createBuildingPanel() {
		_building_panel = std::make_shared<BuildingPanel>(_margin);
	}

	void Editor::cursorHover() {

		if(Main::windows_manager->get_back())
			return;
		
		if (_main_menu->_state != Components::MainMenuStates::Closed) {
			_main_menu->cursorHover();
			return;
		}
		
		_list_panel->cursorHover();
		_building_panel->cursorHover();
		_palette->cursorHover();
		_main_menu->cursorHover();

	}

	void Editor::handleEvent(const sf::Event& event) {

		if (Main::windows_manager->get_back())
			return;
		
		_main_menu->handleEvent(event);
		
		if (_main_menu->_state != Components::MainMenuStates::Closed) {
			return;
		}
		
		_list_panel->handleEvent(event);
		_building_panel->handleEvent(event);
		_palette->handleEvent(event);

	}

	void Editor::update() {

		_list_panel->update();
		_building_panel->update();
		_main_menu->update();
		_palette->update();

	}

	void Editor::draw() {

		GUI_manager->setView();

		Main::render_window->draw(*_title);

		_building_panel->draw();
		_list_panel->draw();
		_palette->draw();
		_main_menu->draw();
	}

	std::shared_ptr<Editor> editor = nullptr;
}