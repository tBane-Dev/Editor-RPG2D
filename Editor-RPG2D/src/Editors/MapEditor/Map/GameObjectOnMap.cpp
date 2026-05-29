#pragma once
#include "Editors/MapEditor/Map/GameObjectOnMap.hpp"
#include "Window.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "DebugLog.hpp"
#include "Animator.hpp"

GameObjectOnMap::GameObjectOnMap(std::shared_ptr<GameObject> prefab) : Object() {
	_prefab = prefab;
	_animator = std::make_shared<Animator>(prefab->getAnimations(), 0.1f);
	_position = sf::Vector2i(0, 0);
}

GameObjectOnMap::~GameObjectOnMap() {

}

void GameObjectOnMap::setPosition(sf::Vector2i position) {
	_position = position;
}

void GameObjectOnMap::update() {
	_animator->update();
}

void GameObjectOnMap::draw() {

	bool renderAllColliders = MapEditor::editor->_main_menu->_render_colliders->_checkbox->_value == 1;

	if (renderAllColliders || _prefab->_collider->cursorHover(MapEditor::editor->_cursor_on_map->_position, _position)) {
		_prefab->getCollider()->draw(_position + _prefab->getOrigin());
	}

	std::shared_ptr<Animations> animations = _animator->getAnimations();
	sf::IntRect frameRect = animations->getFrameRect(0,0);

	if (MapEditor::editor->_main_menu->_render_sprites_outline->_checkbox->_value == 1) {
		sf::RectangleShape outlineRect(sf::Vector2f(frameRect.size));
		outlineRect.setPosition(sf::Vector2f(_position));
		outlineRect.setFillColor(sf::Color::Transparent);
		outlineRect.setOutlineThickness(2);
		outlineRect.setOutlineColor(sf::Color::Red);
		window->draw(outlineRect);
	}

	sf::Sprite sprite(*animations->getTexture()->_texture);
	sprite.setPosition(sf::Vector2f(_position));
	sprite.setTextureRect(frameRect);
	window->draw(sprite);
}