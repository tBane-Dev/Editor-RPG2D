#include "Editors/PrefabsEditor/MeshPanel.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "Theme.hpp"
#include "Window.hpp"

MeshPanel::MeshPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 400), sf::Vector2i(840 + margin.x, prefabs_editor->_main_menu->getSize().y + margin.y + prefabs_editor->_collider_panel->getSize().y + 4)) {
	_title = std::make_unique<sf::Text>(basicFont, L"Mesh", 20);
	_title->setFillColor(basic_text_color);
	_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));
}

MeshPanel::~MeshPanel() {

}

void MeshPanel::cursorHover() {
	Panel::cursorHover();
}

void MeshPanel::handleEvent(const sf::Event& event) {
	Panel::handleEvent(event);
}

void MeshPanel::update() {
	Panel::update();
}

void MeshPanel::draw() {
	Panel::draw();
	window->draw(*_title);
}