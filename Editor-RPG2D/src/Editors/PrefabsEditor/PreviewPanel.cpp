#include "Editors/PrefabsEditor/PreviewPanel.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "Theme.hpp"
#include "Window.hpp"

PreviewPanel::PreviewPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 700), sf::Vector2i(420 + margin.x, prefabs_editor->_main_menu->getSize().y + margin.y)) {
	_title = std::make_unique<sf::Text>(basicFont, L"Preview", 20);
	_title->setFillColor(basic_text_color);
	_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));
}

PreviewPanel::~PreviewPanel() {

}

void PreviewPanel::cursorHover() {
	Panel::cursorHover();
}

void PreviewPanel::handleEvent(const sf::Event& event) {
	Panel::handleEvent(event);
}

void PreviewPanel::update() {
	Panel::update();
}

void PreviewPanel::draw() {
	Panel::draw();
	window->draw(*_title);
}