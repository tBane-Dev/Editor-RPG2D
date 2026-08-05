#pragma once
#include "Objects/Building/FlatRoofPart.hpp"
#include "Roofset.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"

FlatRoofPartPrefab::FlatRoofPartPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Mesh> mesh, int id) : GameObject(name, animations, origin, nullptr, mesh) {
	_type = ObjectType::Roof;
	_id = id;
}

FlatRoofPartPrefab::~FlatRoofPartPrefab() {

}

FlatRoofPart::FlatRoofPart(std::weak_ptr<GameObject> prefab, sf::IntRect textureRect) : GameObjectOnMap(prefab) {
	_type = ObjectType::Roof;
	_textureRect = textureRect;
}

FlatRoofPart::~FlatRoofPart() {

}

void FlatRoofPart::draw(sf::RenderTarget& target, float scale) {
	if (_prefab.expired()) return;

	std::shared_ptr<FlatRoofPartPrefab> roofPrefab = std::dynamic_pointer_cast<FlatRoofPartPrefab>(_prefab.lock());
	if (!roofPrefab) return;

	sf::Sprite sprite(*roofset->_texture->_texture);
	sprite.setPosition(sf::Vector2f(_position));
	sprite.setTextureRect(_textureRect);
	sprite.setScale(sf::Vector2f(scale, scale));
	target.draw(sprite);

	
}