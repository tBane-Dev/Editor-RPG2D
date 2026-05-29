#pragma once
#include "Objects/GameObject.hpp"
#include "Window.hpp"
#include "Objects/Collider.hpp"

GameObject::GameObject(std::wstring name, std::shared_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider) : Object()
{
	_name = name;
	_animations = animations;
	_origin = origin;
	_collider = collider;
}

GameObject::~GameObject() {

}

std::wstring GameObject::getName() {
	return _name;
}

sf::Vector2i GameObject::getOrigin() {
	return _origin;
}

std::shared_ptr<Animations> GameObject::getAnimations() {
	return _animations;
}

std::shared_ptr<Collider> GameObject::getCollider() {
	return _collider;
}