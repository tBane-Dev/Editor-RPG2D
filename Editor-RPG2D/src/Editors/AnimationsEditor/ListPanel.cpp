#include "Editors/AnimationsEditor/ListPanel.hpp"
#include "Editors/AnimationsEditor/Editor.hpp"
#include "Theme.hpp"
#include "Window.hpp"
#include "DebugLog.hpp"
#include "Objects/Monster.hpp"
#include "PrefabsManager.hpp"

namespace AnimationsEditor {

	ListPanel::ListPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 840), sf::Vector2i(margin.x, AnimationsEditor::editor->_main_menu->getSize().y + margin.y)) {

		_title = std::make_unique<sf::Text>(basicFont, L"Animations List", 20);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));
	
	}

	ListPanel::~ListPanel() {

	}

	void ListPanel::cursorHover() {
		Components::Panel::cursorHover();

	}

	void ListPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);
	}

	void ListPanel::update() {
		Components::Panel::update();
	}

	void ListPanel::draw() {
		Components::Panel::draw();

		window->draw(*_title);

	}
}