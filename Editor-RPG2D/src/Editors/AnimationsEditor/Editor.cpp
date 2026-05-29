#include "Editors/AnimationsEditor/Editor.hpp"
#include "DebugLog.hpp"
#include "Theme.hpp"
#include "Window.hpp"

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
	}

	void Editor::createMainMenu() {
		_main_menu = std::make_shared<MainMenu>();
	}

	void Editor::cursorHover() {
		_main_menu->cursorHover();

	}

	void Editor::handleEvent(const sf::Event& event) {
		_main_menu->handleEvent(event);

	}

	void Editor::update() {

		_main_menu->update();

	}

	void Editor::draw() {

		GUI_manager->setView();

		window->draw(*_title);

		_main_menu->draw();
	}

	std::shared_ptr<Editor> editor = nullptr;
}