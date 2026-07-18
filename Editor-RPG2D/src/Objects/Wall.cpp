#pragma once
#include "Objects/Wall.hpp"
#include "Wallset.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"

WallPrefab::WallPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh, int id) : GameObject(name, animations, origin, collider, mesh) {
	_type = ObjectType::Wall;
	_id = id;
}

WallPrefab::~WallPrefab() {

}

Wall::Wall(std::weak_ptr<GameObject> prefab, sf::IntRect textureBottomRect, sf::IntRect textureTopRect) : GameObjectOnMap(prefab) {
	_type = ObjectType::Wall;
	_textureBottomRect = textureBottomRect;
	_textureTopRect = textureTopRect;
}

Wall::~Wall() {

}

void Wall::draw(float scale) {

	if (_prefab.expired()) return;

	std::shared_ptr<WallPrefab> wallPrefab = std::dynamic_pointer_cast<WallPrefab>(_prefab.lock());
	if (!wallPrefab) return;

	sf::Sprite spriteTop(*wallset->_texture->_texture);
	spriteTop.setPosition(sf::Vector2f(_position.x, (int)(_position.y - 32.f*scale)));
	spriteTop.setTextureRect(_textureTopRect);
	spriteTop.setScale(sf::Vector2f(scale, scale));
	Main::render_window->draw(spriteTop);

	sf::Sprite spriteBottom(*wallset->_texture->_texture);
	spriteBottom.setPosition(sf::Vector2f(_position));
	spriteBottom.setTextureRect(_textureBottomRect);
	spriteBottom.setScale(sf::Vector2f(scale, scale));
	Main::render_window->draw(spriteBottom);
}