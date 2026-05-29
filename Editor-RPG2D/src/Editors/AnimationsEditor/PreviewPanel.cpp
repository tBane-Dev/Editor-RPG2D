#include "Editors/AnimationsEditor/PreviewPanel.hpp"
#include "Editors/AnimationsEditor/Editor.hpp"
#include "Theme.hpp"
#include "Window.hpp"
#include "DebugLog.hpp"
#include "Objects/Monster.hpp"
#include "PrefabsManager.hpp"

namespace AnimationsEditor {

	PreviewPanel::PreviewPanel(sf::Vector2i margin) : Panel(sf::Vector2i(520, 840), sf::Vector2i(1132 + margin.x, AnimationsEditor::editor->_main_menu->getSize().y + margin.y)) {

		_title = std::make_unique<sf::Text>(basicFont, L"Preview", 20);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));
	
	}

	PreviewPanel::~PreviewPanel() {

	}

	void PreviewPanel::cursorHover() {
		Components::Panel::cursorHover();
	}

	void PreviewPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);
	}

	void PreviewPanel::update() {
		Components::Panel::update();
	}

	void PreviewPanel::draw() {
		Components::Panel::draw();

		window->draw(*_title);
	}
}