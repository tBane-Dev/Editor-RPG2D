#pragma once
#include "Objects/Nature.hpp"

NaturePrefab::NaturePrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh) : GameObject(name, animations, origin, collider, mesh) {
	_type = ObjectType::Nature;
}

NaturePrefab::~NaturePrefab() {

}

Nature::Nature(std::weak_ptr<GameObject> prefab) : GameObjectOnMap(prefab) {
	_type = ObjectType::Nature;
}

Nature::~Nature() {

}