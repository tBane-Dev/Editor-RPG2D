#include "Editors/PrefabsEditor/ColliderPanel.hpp"
#include "Editors/PrefabsEditor/Editor.hpp"
#include "ShadersManager.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"

namespace PrefabsEditor {
	ColliderPanel::ColliderPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 475), sf::Vector2i(840 + margin.x, PrefabsEditor::editor->_main_menu->getSize().y + margin.y)) {
		_title = std::make_unique<sf::Text>(basicFont, L"Collider", 20);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));

		// text inputs
		_type = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"type", 24, 18);
		_type->setPosition(sf::Vector2i(_rect.position.x + 84, _rect.position.y + margin.y + 192 + 16));
		_type->setText(L"Circular");

		_x = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"x", 24, 18);
		_x->setPosition(sf::Vector2i(_rect.position.x + 84, _type->getPosition().y + _type->getSize().y + 8));
		_x->setText(L"0");
		
		_y = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"y", 24, 18);
		_y->setPosition(sf::Vector2i(_rect.position.x + 84, _x->getPosition().y + _x->getSize().y + 8));
		_y->setText(L"0");

		_w = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"width", 24, 18);
		_w->setPosition(sf::Vector2i(_rect.position.x + 84, _y->getPosition().y + _y->getSize().y + 8));
		_w->setText(L"0");

		_h = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"height", 24, 18);
		_h->setPosition(sf::Vector2i(_rect.position.x + 84, _w->getPosition().y + _w->getSize().y + 8));
		_h->setText(L"0");

		// functions
		_x->_onEditedFunction = [this]() {
			editor->_main_panel->setButtonsActivity();
			editor->_main_panel->setTooltips();
		};

		_y->_onEditedFunction = [this]() {
			editor->_main_panel->setButtonsActivity();
			editor->_main_panel->setTooltips();
			};


		_w->_onEditedFunction = [this]() {
			editor->_main_panel->setButtonsActivity();
			editor->_main_panel->setTooltips();
			};


		_h->_onEditedFunction = [this]() {
			editor->_main_panel->setButtonsActivity();
			editor->_main_panel->setTooltips();
			};

		// texts labels
		int x = _x->getPosition().x - 32;

		_typeLabel = std::make_unique<sf::Text>(basicFont, L"t", 18);
		_typeLabel->setFillColor(basic_text_color);
		_typeLabel->setPosition(sf::Vector2f(x - _typeLabel->getGlobalBounds().size.x / 2, _type->getPosition().y));

		_xLabel = std::make_unique<sf::Text>(basicFont, L"x", 18);
		_xLabel->setFillColor(basic_text_color);
		_xLabel->setPosition(sf::Vector2f(x - _xLabel->getGlobalBounds().size.x / 2, _x->getPosition().y));

		_yLabel = std::make_unique<sf::Text>(basicFont, L"y", 18);
		_yLabel->setFillColor(basic_text_color);
		_yLabel->setPosition(sf::Vector2f(x - _yLabel->getGlobalBounds().size.x / 2, _y->getPosition().y));

		_wLabel = std::make_unique<sf::Text>(basicFont, L"w", 18);
		_wLabel->setFillColor(basic_text_color);
		_wLabel->setPosition(sf::Vector2f(x - _wLabel->getGlobalBounds().size.x / 2, _w->getPosition().y));

		_hLabel = std::make_unique<sf::Text>(basicFont, L"h", 18);
		_hLabel->setFillColor(basic_text_color);
		_hLabel->setPosition(sf::Vector2f(x - _hLabel->getGlobalBounds().size.x / 2, _h->getPosition().y));
	}

	ColliderPanel::~ColliderPanel() {

	}

	void ColliderPanel::cursorHover() {
		Components::Panel::cursorHover();

		_type->cursorHover();
		_x->cursorHover();
		_y->cursorHover();
		_w->cursorHover();
		_h->cursorHover();
	}

	void ColliderPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);

		_type->handleEvent(event);
		_x->handleEvent(event);
		_y->handleEvent(event);
		_w->handleEvent(event);
		_h->handleEvent(event);
	}

	void ColliderPanel::update() {
		Components::Panel::update();

		_type->update();
		_x->update();
		_y->update();
		_w->update();
		_h->update();
	}

	void ColliderPanel::draw() {
		Components::Panel::draw();
		Main::render_window->draw(*_title);

		// draw checkerboard
		sf::RectangleShape rect(sf::Vector2f(sf::Vector2f(192, 192)));
		rect.setFillColor(sf::Color::Red);
		rect.setPosition(sf::Vector2f(getPosition().x + (getSize().x - 192) / 2, getPosition().y + _title->getFont().getLineSpacing(20) + 24 + 16));

		sf::RenderStates states;
		states.shader = &*checkerboard_shader;

		checkerboard_shader->setUniform("rectPos", rect.getPosition());

		Main::render_window->draw(rect, states);

		// draw animation
		std::shared_ptr<Animator>& animator = PrefabsEditor::editor->_animator;
		std::weak_ptr<Animations> animations = (animator) ? animator->getAnimations() : std::weak_ptr<Animations>();

		if (animator && !animations.expired()) {

			sf::IntRect frameRect = animations.lock()->getFrameRect(0, 0);

			float scale = std::min(rect.getSize().x / (float)frameRect.size.x, rect.getSize().y / (float)frameRect.size.y);

			// TO-DO - must be in update
			if (_type->getText() == L"Rectangular") {

				std::shared_ptr<RectangularCollider> rectCollider = std::make_shared<RectangularCollider>(std::stoi(_x->getText()), std::stoi(_y->getText()), std::stoi(_w->getText()), std::stoi(_h->getText()));
				_collider = rectCollider;
				sf::Vector2i colliderPosition(rect.getPosition());
				_collider->draw(colliderPosition, sf::Vector2f(scale, scale));
			}
			else if (_type->getText() == L"Circular") {

				std::shared_ptr<CircularCollider> circularCollider = std::make_shared<CircularCollider>(std::stoi(_x->getText()), std::stoi(_y->getText()), std::stoi(_w->getText()) / 2, std::stoi(_h->getText()) / 2);
				_collider = circularCollider;
				sf::Vector2i colliderPosition(
					rect.getPosition().x + (float)(circularCollider->_x) * scale,
					rect.getPosition().y + (float)(circularCollider->_y) * scale
				);

				_collider->draw(colliderPosition, sf::Vector2f(scale, scale));
			}

			sf::Sprite sprite(*animations.lock()->getTexture()->_texture);
			sprite.setTextureRect(frameRect);
			sprite.setScale(sf::Vector2f(scale, scale));
			sprite.setPosition(sf::Vector2f(rect.getPosition().x, rect.getPosition().y));
			Main::render_window->draw(sprite);
		}

		// texts labels
		Main::render_window->draw(*_typeLabel);
		Main::render_window->draw(*_xLabel);
		Main::render_window->draw(*_yLabel);
		Main::render_window->draw(*_wLabel);
		Main::render_window->draw(*_hLabel);

		// text inputs
		_type->draw();
		_x->draw();
		_y->draw();
		_w->draw();
		_h->draw();

	}
}