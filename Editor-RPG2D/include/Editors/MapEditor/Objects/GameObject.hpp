#pragma once
#include "Editors/MapEditor/Objects/Object.hpp"
#include "AnimationsManager.hpp"

class GameObject : public Object {
public:
	std::wstring _name;
    sf::Vector2i _origin;
	std::shared_ptr<Animations> _animations;

	GameObject(std::wstring name, std::shared_ptr<Animations> animations, sf::Vector2i origin);
	virtual ~GameObject();

	std::wstring getName();
	sf::Vector2i getOrigin();
	std::shared_ptr<Animations> getAnimations();
};