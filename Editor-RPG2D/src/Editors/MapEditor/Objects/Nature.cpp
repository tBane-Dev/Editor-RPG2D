#pragma once
#include "Editors/MapEditor/Objects/Nature.hpp"

NaturePrefab::NaturePrefab(std::wstring name, std::shared_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider) : GameObject(name, animations, origin, collider) {
	_type = ObjectType::Nature;
}

NaturePrefab::~NaturePrefab() {

}

Nature::Nature(std::shared_ptr<GameObject> prefab) : GameObjectOnMap(prefab) {
	_type = ObjectType::Nature;
}

Nature::~Nature() {

}