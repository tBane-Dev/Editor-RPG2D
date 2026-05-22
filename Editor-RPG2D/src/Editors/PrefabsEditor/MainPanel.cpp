#include "Editors/PrefabsEditor/MainPanel.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "Theme.hpp"
#include "Window.hpp"

MainPanel::MainPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 700), sf::Vector2i(margin.x, prefabs_editor->_main_menu->getSize().y + margin.y)) {

	_title = std::make_unique<sf::Text>(basicFont, L"Main", 20);
	_title->setFillColor(basic_text_color);
	_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));
}

MainPanel::~MainPanel() {

}

void MainPanel::cursorHover() {
	Panel::cursorHover();
}

void MainPanel::handleEvent(const sf::Event& event) {
	Panel::handleEvent(event);
}

void MainPanel::update() {
	Panel::update();
}

void MainPanel::draw() {
	Panel::draw();
	window->draw(*_title);
}