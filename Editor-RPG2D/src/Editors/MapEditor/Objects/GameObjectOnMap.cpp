#pragma once
#include "Editors/MapEditor/Objects/GameObjectOnMap.hpp"
#include "Window.hpp"
#include "Editors/MapEditor/MapEditor.hpp"

GameObjectOnMap::GameObjectOnMap(std::shared_ptr<GameObject> prefab) : Object() {
	_prefab = prefab;
	_position = sf::Vector2i(0, 0);
	_frame = 0;
}

GameObjectOnMap::~GameObjectOnMap() {

}

void GameObjectOnMap::setPosition(sf::Vector2i position) {
	_position = position;
}

void GameObjectOnMap::update() {
	
}

void GameObjectOnMap::draw() {

	if (map_editor->_main_menu->_render_colliders->_checkbox->_value == 1) {
		sf::CircleShape collider(32);
		collider.setFillColor(sf::Color(255, 0, 0, 128));
		collider.setPosition(sf::Vector2f(_position + _prefab->getOrigin()));
		collider.setOrigin(sf::Vector2f(32, 32));
		window->draw(collider);
	}

	std::shared_ptr<Animations> anim = _prefab->getAnimations();
	sf::IntRect frameRect = anim->getFrameRect(0,0);

	if (map_editor->_main_menu->_render_sprites_outline->_checkbox->_value == 1) {
		sf::RectangleShape outlineRect(sf::Vector2f(frameRect.size));
		outlineRect.setPosition(sf::Vector2f(_position));
		outlineRect.setFillColor(sf::Color::Transparent);
		outlineRect.setOutlineThickness(2);
		outlineRect.setOutlineColor(sf::Color::Red);
		window->draw(outlineRect);
	}

	sf::Sprite sprite(*anim->getTexture()->_texture);
	sprite.setPosition(sf::Vector2f(_position));
	sprite.setTextureRect(frameRect);
	window->draw(sprite);
}