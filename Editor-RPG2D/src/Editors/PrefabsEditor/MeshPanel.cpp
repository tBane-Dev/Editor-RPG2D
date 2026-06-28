#include "Editors/PrefabsEditor/MeshPanel.hpp"
#include "Editors/PrefabsEditor/Editor.hpp"
#include "ShaderManager.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include <iostream>

namespace PrefabsEditor {
	MeshPanel::MeshPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 375), sf::Vector2i(840 + margin.x, PrefabsEditor::editor->_main_menu->getSize().y + margin.y + PrefabsEditor::editor->_collider_panel->getSize().y + 4)) {
		_title = std::make_unique<sf::Text>(basicFont, L"Mesh", 20);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));

		_set_mesh = std::make_shared<ButtonWithTextAndSprite>(
			L"Set Mesh",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\bottomButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\bottomButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\bottomButton_press.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\bottomButton_inactive.png"),
			sf::Vector2i(_rect.position.x + _rect.size.x / 2 - 192 / 2, _rect.position.y + margin.y + 192 + 16)
		);

		setButtonsActivity();
	}

	MeshPanel::~MeshPanel() {

	}

	void MeshPanel::setButtonsActivity() {

		if (!editor || !editor->_animator || editor->_animator->_animations.expired()) {
			_set_mesh->setActive(false);
		}
		else {
			_set_mesh->setActive(true);
		}

	}

	void MeshPanel::cursorHover() {
		Components::Panel::cursorHover();

		_set_mesh->cursorHover();
	}

	void MeshPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);

		_set_mesh->handleEvent(event);
	}

	void MeshPanel::update() {
		Components::Panel::update();

		_set_mesh->update();
	}

	void MeshPanel::draw() {
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
		std::weak_ptr<Animations> animations = (animator)? animator->getAnimations() : std::weak_ptr<Animations>();
		if (animator && !animations.expired()) {
			
			sf::IntRect frameRect = animations.lock()->getFrameRect(0, 0);

			sf::Sprite sprite(*animations.lock()->getTexture()->_texture);
			sprite.setTextureRect(frameRect);
			sprite.setScale(sf::Vector2f(rect.getSize().x / (float)frameRect.size.x, rect.getSize().y / (float)frameRect.size.y));
			sprite.setPosition(sf::Vector2f(rect.getPosition().x, rect.getPosition().y));
			Main::render_window->draw(sprite);
		}

		_set_mesh->draw();
	}
}