#pragma once
#include "Objects/Building/Wall.hpp"
#include "Wallset.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"
#include "EditorsManager.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Objects/Building/Building.hpp"

WallPrefab::WallPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh, int id, int height) : GameObject(name, animations, origin, collider, mesh) {
	_type = ObjectType::Wall;
	_id = id;
	_height = height;
}

WallPrefab::~WallPrefab() {

}

Wall::Wall(std::weak_ptr<GameObject> prefab, std::weak_ptr<Building> building, sf::IntRect textureBottomRect, sf::IntRect textureTopRect, int height) : PlacedGameObject(prefab) {
	_type = ObjectType::Wall;
	_building = building;
	_textureBottomRect = textureBottomRect;
	_textureTopRect = textureTopRect;
	_height = height;
}


Wall::~Wall() {

}

void Wall::draw(sf::RenderTarget& target, float scale) {

	if (_prefab.expired())
		return;

	std::shared_ptr<WallPrefab> wallPrefab = std::dynamic_pointer_cast<WallPrefab>(_prefab.lock());

	if (!wallPrefab)
		return;

	if (BuildingsEditor::editor && BuildingsEditor::editor->_main_menu->_render_walls_look->_checkbox->_value == 0) {
		
		sf::IntRect bottomWallRect = sf::IntRect(_position, sf::Vector2i(32.f*scale, 32.f*scale));
		for(auto& door : _building.lock()->_doorsObjects) {
			sf::IntRect doorRect = sf::IntRect(door->_position, sf::Vector2i(64.f*scale, 64.f*scale));
			if(doorRect.findIntersection(bottomWallRect)) {
				return;
			}
		}

		sf::Sprite spriteBottom(*wallset->_texture->_texture);
		spriteBottom.setPosition(sf::Vector2f(_position));
		spriteBottom.setTextureRect(_textureBottomRect);
		spriteBottom.setScale(sf::Vector2f(scale, scale));
		target.draw(spriteBottom);

		return;
	}

	bool renderOutsideLook = true;

	if(BuildingsEditor::editor) {
		renderOutsideLook = BuildingsEditor::editor->_main_menu->_render_walls_look->_checkbox->_value == 2;
	}
	else if (std::shared_ptr<Building> building = _building.lock()) {
		renderOutsideLook = building->_renderOutsideLook;
	}
	

	if (renderOutsideLook) {

		bool collidedWithDoor = false;
		sf::IntRect bottomWallRect = sf::IntRect(_position, sf::Vector2i(32.f*scale, 32.f*scale));
		for (auto& door : _building.lock()->_doorsObjects) {
			sf::IntRect doorRect = sf::IntRect(door->_position, sf::Vector2i(64.f*scale, 64.f*scale));
			if (doorRect.findIntersection(bottomWallRect)) {
				collidedWithDoor = true;
				break;
			}
		}

		sf::Sprite spriteCenter(*wallset->_texture->_texture);
		spriteCenter.setTextureRect(_textureBottomRect);
		spriteCenter.setScale(sf::Vector2f(scale, scale));

		for (int i = (collidedWithDoor)? 2: 1; i < _height; ++i) {
			spriteCenter.setPosition(sf::Vector2f(
				_position.x,
				_position.y - 32.f * i * scale
			));

			target.draw(spriteCenter);
		}

		if (!collidedWithDoor) {
			sf::Sprite spriteBottom(*wallset->_texture->_texture);
			spriteBottom.setPosition(sf::Vector2f(_position));
			spriteBottom.setTextureRect(_textureBottomRect);
			spriteBottom.setScale(sf::Vector2f(scale, scale));
			target.draw(spriteBottom);
		}
		
	}
	else {

		bool collidedWithDoor = false;
		sf::IntRect bottomWallRect = sf::IntRect(_position, sf::Vector2i(32.f*scale, 32.f*scale));
		for (auto& door : _building.lock()->_doorsObjects) {
			sf::IntRect doorRect = sf::IntRect(door->_position, sf::Vector2i(64.f*scale, 64.f*scale));
			if (doorRect.findIntersection(bottomWallRect)) {
				return; 
			}
		}

		sf::Sprite spriteTop(*wallset->_texture->_texture);
		spriteTop.setPosition(sf::Vector2f(
			_position.x,
			_position.y - 32.f * scale
		));

		spriteTop.setTextureRect(_textureTopRect);
		spriteTop.setScale(sf::Vector2f(scale, scale));

		target.draw(spriteTop);

		sf::Sprite spriteBottom(*wallset->_texture->_texture);
		spriteBottom.setPosition(sf::Vector2f(_position));
		spriteBottom.setTextureRect(_textureBottomRect);
		spriteBottom.setScale(sf::Vector2f(scale, scale));

		target.draw(spriteBottom);
	}
}
void Wall::draw(sf::RenderTarget& target, float scale, int drawType,bool collidedWithDoor) {

	if (_prefab.expired())
		return;

	std::shared_ptr<WallPrefab> wallPrefab = std::dynamic_pointer_cast<WallPrefab>(_prefab.lock());

	if (!wallPrefab)
		return;

	if (drawType == 0) {

		if (collidedWithDoor)
			return;

		sf::Sprite spriteBottom(*wallset->_texture->_texture);
		spriteBottom.setPosition(sf::Vector2f(_position));
		spriteBottom.setTextureRect(_textureBottomRect);
		spriteBottom.setScale(sf::Vector2f(scale, scale));
		target.draw(spriteBottom);
	}
	else if (drawType == 1) {

		if (collidedWithDoor)
			return;

		sf::Sprite spriteTop(*wallset->_texture->_texture);
		spriteTop.setPosition(sf::Vector2f(_position.x, _position.y - 32.f * scale));
		spriteTop.setTextureRect(_textureTopRect);
		spriteTop.setScale(sf::Vector2f(scale, scale));
		target.draw(spriteTop);

		sf::Sprite spriteBottom(*wallset->_texture->_texture);
		spriteBottom.setPosition(sf::Vector2f(_position));
		spriteBottom.setTextureRect(_textureBottomRect);
		spriteBottom.setScale(sf::Vector2f(scale, scale));
		target.draw(spriteBottom);
	}
	else if (drawType == 2) {

		sf::Sprite spriteCenter(*wallset->_texture->_texture);
		spriteCenter.setTextureRect(_textureBottomRect);
		spriteCenter.setScale(sf::Vector2f(scale, scale));

		for (int i = collidedWithDoor ? 2 : 1; i < _height; ++i) {
			spriteCenter.setPosition(sf::Vector2f(_position.x, _position.y - 32.f * i * scale));
			target.draw(spriteCenter);
		}

		if (!collidedWithDoor) {
			sf::Sprite spriteBottom(*wallset->_texture->_texture);
			spriteBottom.setPosition(sf::Vector2f(_position));
			spriteBottom.setTextureRect(_textureBottomRect);
			spriteBottom.setScale(sf::Vector2f(scale, scale));
			target.draw(spriteBottom);
		}
	}
}

void Wall::draw() {
	
	if (Main::editor_manager->get_back() == BuildingsEditor::editor) {
		draw(*Main::render_window, BuildingsEditor::editor->_building_panel->_building->_scale);
	}

	if(Main::editor_manager->get_back() == MapEditor::editor) {
		draw(*Main::render_window, 1.f);
	}
}