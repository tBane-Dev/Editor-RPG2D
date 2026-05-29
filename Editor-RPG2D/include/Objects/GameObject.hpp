#pragma once
#include "Objects/Object.hpp"
#include "AnimationsManager.hpp"
#include "Objects/Collider.hpp"

class GameObject : public Object {
public:
	std::wstring _name;
    sf::Vector2i _origin;
	std::shared_ptr<Animations> _animations;
	std::shared_ptr<Collider> _collider;

	GameObject(std::wstring name, std::shared_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider);
	virtual ~GameObject();

	std::wstring getName();
	sf::Vector2i getOrigin();
	std::shared_ptr<Animations> getAnimations();
	std::shared_ptr<Collider> getCollider();
};