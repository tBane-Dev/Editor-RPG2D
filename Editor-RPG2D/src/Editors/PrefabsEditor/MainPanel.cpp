#include "Editors/PrefabsEditor/MainPanel.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "Theme.hpp"
#include "Window.hpp"
#include "DebugLog.hpp"

MainPanel::MainPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 800), sf::Vector2i(margin.x, prefabs_editor->_main_menu->getSize().y + margin.y)) {

	_nameText = std::make_unique<sf::Text>(basicFont, L"Name", 20);
	_nameText->setFillColor(basic_text_color);
	_nameText->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 24));

	_name = std::make_shared<TextInput>(sf::Vector2i(384, 48), L"Type here", 20, 20);
	_name->setPosition(sf::Vector2i(_rect.position.x + 16, _nameText->getGlobalBounds().position.y + 32));

	_typeText = std::make_unique<sf::Text>(basicFont, L"Type", 20);
	_typeText->setFillColor(basic_text_color);
	_typeText->setPosition(sf::Vector2f(_rect.position.x + 16, _name->getPosition().y + _name->getSize().y + 32));

	_type = std::make_shared<TextInput>(sf::Vector2i(384, 48), L"Type here", 20, 20);
	_type->setPosition(sf::Vector2i(_rect.position.x + 16, _typeText->getGlobalBounds().position.y + 32));
}

MainPanel::~MainPanel() {

}

void MainPanel::cursorHover() {
	Panel::cursorHover();

	_name->cursorHover();
	_type->cursorHover();

}

void MainPanel::handleEvent(const sf::Event& event) {
	Panel::handleEvent(event);

	_name->handleEvent(event);	
	_type->handleEvent(event);
}

void MainPanel::update() {
	Panel::update();

	_name->update();
	_type->update();
}

void MainPanel::draw() {
	Panel::draw();

	window->draw(*_nameText);
	_name->draw();

	window->draw(*_typeText);
	_type->draw();



	
}