#pragma once
#include "Editors/MapEditor/Objects/GameObject.hpp"

GameObject::GameObject(std::wstring name, std::shared_ptr<Animations> animations, sf::Vector2i origin) : Object()
{
	_name = name;
	_animations = animations;
	_origin = origin;
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