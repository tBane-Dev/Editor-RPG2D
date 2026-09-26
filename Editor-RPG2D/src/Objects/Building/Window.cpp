#pragma once
#include "Objects/Building/Window.hpp"
#include "Objects/Building/Wall.hpp"
#include "EditorsManager.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"

WindowPrefab::WindowPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh) : GameObject(name, animations, origin, collider, mesh) {
	_type = ObjectType::Window;
}

WindowPrefab::~WindowPrefab() {

}

Window::Window(std::weak_ptr<GameObject> prefab, std::weak_ptr<Wall> wall, int level) : PlacedGameObject(prefab) {
	_type = ObjectType::Window;
	_wall = wall;
	_level = level;
}

Window::~Window() {

}

void Window::draw(sf::RenderTarget& target, float scale) {

	if (_prefab.expired())
		return;

	float levelOffset = 0.0f;
	if(MapEditor::editor == Main::editor_manager->get_back()) {
		levelOffset = _level * 32.f;
	}

	if(BuildingsEditor::editor == Main::editor_manager->get_back()) {
		levelOffset = _level * 32.f * BuildingsEditor::editor->_building_panel->_building->_scale;
	}

	sf::Sprite sprite(*_animator->getAnimations().lock()->getTexture()->_texture);
	sprite.setPosition(sf::Vector2f(_position.x, _position.y * levelOffset));
	sprite.setTextureRect(_animator->getAnimations().lock()->getFrameRect(_animator->_animation, _animator->_frame));
	sprite.setScale(sf::Vector2f(scale, scale));
	target.draw(sprite);

}

void Window::draw() {
	if (Main::editor_manager->get_back() == BuildingsEditor::editor) {
		draw(*Main::render_window, BuildingsEditor::editor->_building_panel->_building->_scale);
	}

	if (Main::editor_manager->get_back() == MapEditor::editor) {
		draw(*Main::render_window, 1.f);
	}
}