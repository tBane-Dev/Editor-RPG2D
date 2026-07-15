#include "Editors/BuildingsEditor/NamePanel.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"

namespace BuildingsEditor {

	NamePanel::NamePanel(sf::Vector2i margin) : Panel(
		sf::Vector2i(Main::render_window->getSize().x - BuildingsEditor::editor->_list_panel->getSize().x - BuildingsEditor::editor->_palette->getSize().x - margin.x - 8, 128),
		sf::Vector2i(420 + margin.x, BuildingsEditor::editor->_main_menu->getSize().y + margin.y)) {

		_nameText = std::make_unique<sf::Text>(basicFont, L"Name", 20);
		_nameText->setFillColor(basic_text_color);
		_nameText->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 24));

		_name = std::make_shared<TextInput>(sf::Vector2i(384, 48), L"Type here", 32, 20);
		_name->setPosition(sf::Vector2i(_rect.position.x + 16, _nameText->getGlobalBounds().position.y + 32));

		_name->_onEditedFunction = [this]() {

		};
	}

	NamePanel::~NamePanel() {

	}

	void NamePanel::cursorHover() {
		Components::Panel::cursorHover();

		_name->cursorHover();

	}

	void NamePanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);

		_name->handleEvent(event);
	}

	void NamePanel::update() {
		Components::Panel::update();

		_name->update();
	}

	void NamePanel::draw() {
		Components::Panel::draw();

		Main::render_window->draw(*_nameText);
		_name->draw();

	}
}