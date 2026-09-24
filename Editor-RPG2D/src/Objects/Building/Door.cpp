#pragma once
#include "Objects/Building/Door.hpp"
#include "EditorsManager.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
DoorPrefab::DoorPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh) : GameObject(name, animations, origin, collider, mesh) {
	_type = ObjectType::Door;
}

DoorPrefab::~DoorPrefab() {

}

Door::Door(std::weak_ptr<GameObject> prefab, std::weak_ptr<Building> building) : PlacedGameObject(prefab) {
	_type = ObjectType::Door;
	_building = building;
}

Door::~Door() {

}

void Door::draw(sf::RenderTarget& target, float scale) {

	if (_prefab.expired())
		return;

	
	sf::Sprite sprite(*_animator->getAnimations().lock()->getTexture()->_texture);
	sprite.setPosition(sf::Vector2f(_position));
	sprite.setTextureRect(_animator->getAnimations().lock()->getFrameRect(_animator->_animation, _animator->_frame));
	sprite.setScale(sf::Vector2f(scale, scale));
	target.draw(sprite);

}

void Door::draw() {
	if (Main::editor_manager->get_back() == BuildingsEditor::editor) {
		draw(*Main::render_window, BuildingsEditor::editor->_building_panel->_building->_scale);
	}

	if (Main::editor_manager->get_back() == MapEditor::editor) {
		draw(*Main::render_window, 1.f);
	}
}