#pragma once
#include "Objects/Wall.hpp"
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

Wall::Wall(std::weak_ptr<GameObject> prefab, std::weak_ptr<Building> building, sf::IntRect textureBottomRect, sf::IntRect textureTopRect, int height) : GameObjectOnMap(prefab) {
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

	std::shared_ptr<WallPrefab> wallPrefab =
		std::dynamic_pointer_cast<WallPrefab>(_prefab.lock());

	if (!wallPrefab)
		return;

	if (BuildingsEditor::editor && BuildingsEditor::editor->_main_menu->_render_walls_look->_checkbox->_value == 0) {
		sf::Sprite spriteBottom(*wallset->_texture->_texture);
		spriteBottom.setPosition(sf::Vector2f(_position));
		spriteBottom.setTextureRect(_textureBottomRect);
		spriteBottom.setScale(sf::Vector2f(scale, scale));

		target.draw(spriteBottom);
		return;
	}

	// ?ciana bez budynku zawsze renderuje wygl?d zewn?trzny.
	bool renderOutsideLook = true;

	if (std::shared_ptr<Building> building = _building.lock()) {
		renderOutsideLook = building->_renderOutsideLook;
	}

	if (renderOutsideLook) {

		sf::Sprite spriteCenter(*wallset->_texture->_texture);
		spriteCenter.setTextureRect(_textureBottomRect);
		spriteCenter.setScale(sf::Vector2f(scale, scale));

		for (int i = 1; i < _height; ++i) {
			spriteCenter.setPosition(sf::Vector2f(
				_position.x,
				_position.y - 32.f * i * scale
			));

			target.draw(spriteCenter);
		}

		sf::Sprite spriteBottom(*wallset->_texture->_texture);
		spriteBottom.setPosition(sf::Vector2f(_position));
		spriteBottom.setTextureRect(_textureBottomRect);
		spriteBottom.setScale(sf::Vector2f(scale, scale));

		target.draw(spriteBottom);
	}
	else {

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

void Wall::draw(sf::RenderTarget& target, float scale, int drawType) {

	if (_prefab.expired())
		return;

	std::shared_ptr<WallPrefab> wallPrefab =
		std::dynamic_pointer_cast<WallPrefab>(_prefab.lock());

	if (!wallPrefab)
		return;

	if (drawType == 0) {
		sf::Sprite spriteBottom(*wallset->_texture->_texture);
		spriteBottom.setPosition(sf::Vector2f(_position));
		spriteBottom.setTextureRect(_textureBottomRect);
		spriteBottom.setScale(sf::Vector2f(scale, scale));

		target.draw(spriteBottom);
	}

	else if(drawType == 1) {

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

	else if (drawType == 2) {

		sf::Sprite spriteCenter(*wallset->_texture->_texture);
		spriteCenter.setTextureRect(_textureBottomRect);
		spriteCenter.setScale(sf::Vector2f(scale, scale));

		for (int i = 1; i < _height; ++i) {
			spriteCenter.setPosition(sf::Vector2f(
				_position.x,
				_position.y - 32.f * i * scale
			));

			target.draw(spriteCenter);
		}

		sf::Sprite spriteBottom(*wallset->_texture->_texture);
		spriteBottom.setPosition(sf::Vector2f(_position));
		spriteBottom.setTextureRect(_textureBottomRect);
		spriteBottom.setScale(sf::Vector2f(scale, scale));

		target.draw(spriteBottom);
	}
	
}

void Wall::draw() {
	draw(*Main::render_window, 1.f);
}