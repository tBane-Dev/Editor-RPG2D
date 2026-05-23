#include "Editors/PrefabsEditor/ColliderPanel.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "Theme.hpp"
#include "Window.hpp"

ColliderPanel::ColliderPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 400), sf::Vector2i(840 + margin.x, prefabs_editor->_main_menu->getSize().y + margin.y)) {
	_title = std::make_unique<sf::Text>(basicFont, L"Collider", 20);
	_title->setFillColor(basic_text_color);
	_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));
}

ColliderPanel::~ColliderPanel() {

}

void ColliderPanel::cursorHover() {
	Panel::cursorHover();
}

void ColliderPanel::handleEvent(const sf::Event& event) {
	Panel::handleEvent(event);
}

void ColliderPanel::update() {
	Panel::update();
}

void ColliderPanel::draw() {
	Panel::draw();
	window->draw(*_title);
}