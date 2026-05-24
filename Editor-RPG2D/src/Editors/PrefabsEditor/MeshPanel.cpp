#include "Editors/PrefabsEditor/MeshPanel.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "ShaderManager.hpp"
#include "Theme.hpp"
#include "Window.hpp"

MeshPanel::MeshPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 375), sf::Vector2i(840 + margin.x, prefabs_editor->_main_menu->getSize().y + margin.y + prefabs_editor->_collider_panel->getSize().y + 4)) {
	_title = std::make_unique<sf::Text>(basicFont, L"Mesh", 20);
	_title->setFillColor(basic_text_color);
	_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));

	_set_mesh = std::make_shared<ButtonWithTextAndSprite>(
		L"Set Mesh",
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\bottomButton.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\bottomButton_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\bottomButton_press.png"),
		sf::Vector2i(_rect.position.x + _rect.size.x / 2 - 192 / 2, _rect.position.y + margin.y + 192 + 16)
	);

}

MeshPanel::~MeshPanel() {

}

void MeshPanel::cursorHover() {
	Panel::cursorHover();

	_set_mesh->cursorHover();
}

void MeshPanel::handleEvent(const sf::Event& event) {
	Panel::handleEvent(event);

	_set_mesh->handleEvent(event);
}

void MeshPanel::update() {
	Panel::update();

	_set_mesh->update();
}

void MeshPanel::draw() {
	Panel::draw();
	window->draw(*_title);

	// draw checkerboard
	sf::RectangleShape rect(sf::Vector2f(sf::Vector2f(192, 192)));
	rect.setFillColor(sf::Color::Red);
	rect.setPosition(sf::Vector2f(getPosition().x + (getSize().x - 192) / 2, getPosition().y + _title->getFont().getLineSpacing(20) + 24 + 16));

	sf::RenderStates states;
	states.shader = &*checkerboard_shader;

	checkerboard_shader->setUniform("rectPos", rect.getPosition());

	window->draw(rect, states);

	// draw animation
	std::shared_ptr<Animator>& animator = prefabs_editor->_animation_panel->_animator;
	if (animator) {
		std::shared_ptr<Animations> animations = animator->getAnimations();
		sf::IntRect frameRect = animations->getFrameRect(0, 0);

		sf::Sprite sprite(*animations->getTexture()->_texture);
		sprite.setTextureRect(frameRect);
		sprite.setScale(sf::Vector2f(rect.getSize().x/ (float)frameRect.size.x, rect.getSize().y / (float)frameRect.size.y));
		sprite.setPosition(sf::Vector2f(rect.getPosition().x, rect.getPosition().y));
		window->draw(sprite);
	}

	_set_mesh->draw();
}