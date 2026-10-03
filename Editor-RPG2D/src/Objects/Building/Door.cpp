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
	_animator->_frame = 0;
	_animator->stop();
}

Door::~Door() {

}


void Door::draw(sf::RenderTarget& target, float scale) {

	if (_prefab.expired())
		return;

	
	sf::Sprite sprite(*_animator->getAnimations().lock()->getTexture()->_texture);
	sprite.setPosition(sf::Vector2f(_position));
	sprite.setOrigin(sf::Vector2f(0, 48));
	sprite.setTextureRect(_animator->getAnimations().lock()->getFrameRect(_animator->_animation, _animator->_frame));
	sprite.setScale(sf::Vector2f(scale, scale));
	target.draw(sprite);

}

void Door::draw() {
	if (Main::editor_manager->get_back() == BuildingsEditor::editor) {
		draw(*Main::render_window, BuildingsEditor::editor->_building_panel->_building->_scale);
	}

	if (Main::editor_manager->get_back() == MapEditor::editor) {
		bool renderAllColliders = MapEditor::editor->_main_menu->_render_colliders->_checkbox->_value == 1;

		sf::Vector2i pos = _position;

		if (_prefab.expired()) return;

		std::shared_ptr<Collider> collider = _prefab.lock()->getCollider();

		if (renderAllColliders) {
			if (collider->_type == ColliderType::Rectangular) {
				collider->draw(_position);
			}
			else if (collider->_type == ColliderType::Circular) {
				collider->draw(_position + _prefab.lock()->getOrigin());
			}
		}

		if (_isSelected == true) {
			drawFrame(sf::Color(255, 30, 45));
		}
		else if (MapEditor::editor->_main_menu->_render_sprites_outline->_checkbox->_value == 1) {
			drawFrame();
		}

		if (_animator->_animations.expired())
			return;

		std::shared_ptr<Animations> animations = _animator->getAnimations().lock();

		if (animations) {
			sf::IntRect frameRect = animations->getFrameRect(_animator->_animation, _animator->_frame);

			sf::Sprite sprite(*animations->getTexture()->_texture);
			sprite.setPosition(sf::Vector2f(_position));
			sprite.setOrigin(sf::Vector2f(0, 48));
			sprite.setTextureRect(frameRect);
			if (MapEditor::editor->_game_objects->_hoveredPlacedGameObject.lock().get() == this)
				sprite.setColor(sf::Color(255, 30 + 64, 45 + 64)); // TO-DO - must be a shader highlight
			else if (_isSelected == true)
				sprite.setColor(sf::Color(255, 30 + 64, 45 + 64));
			else
				sprite.setColor(sf::Color::White);
			Main::render_window->draw(sprite);
		}

		if (MapEditor::editor->_main_menu->_render_meshes->_checkbox->_value == 1) {
			if (_prefab.lock()->getMesh()) {
				_prefab.lock()->getMesh()->draw(_position, sf::Color::Red);
			}
		}
	}
}