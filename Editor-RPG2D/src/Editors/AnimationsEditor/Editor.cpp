#include "Editors/AnimationsEditor/Editor.hpp"
#include "DebugLog.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "WindowsManager.hpp"

namespace AnimationsEditor {
	Editor::Editor() {
		_margin = sf::Vector2i(32, 64);

		_title = std::make_unique<sf::Text>(basicFont, L"Animations Editor", 28);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(32, 48));

		
	}

	Editor::~Editor() {

	}

	void Editor::init() {
		createMainMenu();

		createListPanel();
		createNamePanel();
		createSpriteSheetPanel();
		createActionsPanel();
		createPreviewPanel();
	}

	void Editor::createMainMenu() {
		_main_menu = std::make_shared<MainMenu>();
	}

	void Editor::createListPanel() {
		_list_panel = std::make_shared<ListPanel>(_margin);
	}

	void Editor::createNamePanel() {
		_name_panel = std::make_shared<NamePanel>(_margin);
	}

	void Editor::createSpriteSheetPanel() {
		_sprite_sheet_panel = std::make_shared<SpriteSheetPanel>(_margin);
	}

	void Editor::createActionsPanel() {
		_actions_panel = std::make_shared<ActionsPanel>(_margin);
	}

	void Editor::createPreviewPanel() {
		_preview_panel = std::make_shared<PreviewPanel>(_margin);
	}

	void Editor::cursorHover() {
		
		if (Main::windows_manager->get_back())
			return;

		if (_main_menu->_state != Components::MainMenuStates::Closed) {
			_main_menu->cursorHover();
			return;
		}
			
		_name_panel->cursorHover();
		_list_panel->cursorHover();
		_actions_panel->cursorHover();
		_sprite_sheet_panel->cursorHover();
		_preview_panel->cursorHover();
		_main_menu->cursorHover();

	}

	void Editor::handleEvent(const sf::Event& event) {
		if (Main::windows_manager->get_back())
			return;

		_main_menu->handleEvent(event);

		if (_main_menu->_state != Components::MainMenuStates::Closed)
			return;

		_name_panel->handleEvent(event);
		_list_panel->handleEvent(event);
		_actions_panel->handleEvent(event);
		_sprite_sheet_panel->handleEvent(event);
		_preview_panel->handleEvent(event);

	}

	void Editor::update() {

		if (_animator) {
			_animator->update();
		}

		_main_menu->update();
		_name_panel->update();
		_list_panel->update();
		_actions_panel->update();
		_sprite_sheet_panel->update();
		_preview_panel->update();

	}

	void Editor::draw() {

		GUI_manager->setView();

		Main::render_window->draw(*_title);
		_name_panel->draw();
		_list_panel->draw();
		_actions_panel->draw();
		_sprite_sheet_panel->draw();
		_preview_panel->draw();	

		_main_menu->draw();
	}

	std::shared_ptr<Editor> editor = nullptr;
}