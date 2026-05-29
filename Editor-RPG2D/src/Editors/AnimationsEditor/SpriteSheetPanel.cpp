#include "Editors/AnimationsEditor/SpriteSheetPanel.hpp"
#include "Editors/AnimationsEditor/Editor.hpp"
#include "Theme.hpp"
#include "Window.hpp"
#include "DebugLog.hpp"
#include "Objects/Monster.hpp"
#include "PrefabsManager.hpp"

namespace AnimationsEditor {

	SpriteSheetPanel::SpriteSheetPanel(sf::Vector2i margin) : Panel(sf::Vector2i(712, 712), sf::Vector2i(420 + margin.x, AnimationsEditor::editor->_main_menu->getSize().y + margin.y + 128)) {

		_title = std::make_unique<sf::Text>(basicFont, L"Sprite Sheet", 20);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));
	
	}

	SpriteSheetPanel::~SpriteSheetPanel() {

	}

	void SpriteSheetPanel::cursorHover() {
		Components::Panel::cursorHover();
	}

	void SpriteSheetPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);
	}

	void SpriteSheetPanel::update() {
		Components::Panel::update();
	}

	void SpriteSheetPanel::draw() {
		Components::Panel::draw();
		
		// draw checkerboard
		sf::RectangleShape rect(sf::Vector2f(sf::Vector2f(512, 512)));
		rect.setFillColor(sf::Color::White);
		rect.setPosition(sf::Vector2f(getPosition().x + (getSize().x - 512) / 2, getPosition().y + _title->getFont().getLineSpacing(20) + 24 + 16));
		sf::RenderStates states;
		states.shader = &*checkerboard_shader;
		checkerboard_shader->setUniform("rectPos", rect.getPosition());
		window->draw(rect, states);

		// draw sprite sheet
		if (AnimationsEditor::editor->_animations) {
			sf::Sprite sprite(*AnimationsEditor::editor->_animations->getTexture()->_texture);
			sprite.setPosition(rect.getPosition());
			window->draw(sprite);
		}

		window->draw(*_title);
	}
}