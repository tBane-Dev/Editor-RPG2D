#pragma once
#include "Objects/GameObject.hpp"
#include "RenderWindow.hpp"
#include "Objects/Collider.hpp"
#include "Objects/Mesh.hpp"

GameObject::GameObject(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh) : Object()
{
	_name = name;
	_animations = animations;
	_origin = origin;
	_collider = collider;
	_mesh = mesh;
}

GameObject::~GameObject() {

}

std::wstring GameObject::getName() {
	return _name;
}

sf::Vector2i GameObject::getOrigin() {
	return _origin;
}

std::weak_ptr<Animations> GameObject::getAnimations() {
	return _animations;
}

std::shared_ptr<Collider> GameObject::getCollider() {
	return _collider;
}

std::shared_ptr<Mesh> GameObject::getMesh() {
	return _mesh;
}