#include "Editors/AnimationsEditor/AnimationsEditor.hpp"
#include "DebugLog.hpp"
#include "Theme.hpp"
#include "Window.hpp"

AnimationsEditor::AnimationsEditor() {
	_margin = sf::Vector2i(32, 64);

	_title = std::make_unique<sf::Text>(basicFont, L"Animations Editor", 28);
	_title->setFillColor(basic_text_color);
	_title->setPosition(sf::Vector2f(32, 48));

}

AnimationsEditor::~AnimationsEditor() {
	
}

void AnimationsEditor::init() {
	createMainMenu();
}

void AnimationsEditor::createMainMenu() {
	_main_menu = std::make_shared<AnimationsEditorMainMenu>();
}

void AnimationsEditor::cursorHover() {
	_main_menu->cursorHover();

}

void AnimationsEditor::handleEvent(const sf::Event& event) {
	_main_menu->handleEvent(event);

}

void AnimationsEditor::update() {

	_main_menu->update();

}

void AnimationsEditor::draw() {
	
	GUI_manager->setView();

	window->draw(*_title);

	_main_menu->draw();
}

std::shared_ptr<AnimationsEditor> animations_editor = nullptr;