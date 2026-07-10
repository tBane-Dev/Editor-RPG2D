#pragma once
#include "Objects/Object.hpp"
#include "AnimationsManager.hpp"
#include "Objects/Collider.hpp"
#include "Objects/Mesh.hpp"

class GameObject : public Object {
public:
	std::wstring _name;
    sf::Vector2i _origin;
	std::weak_ptr<Animations> _animations;
	std::shared_ptr<Collider> _collider;
	std::shared_ptr<Mesh> _mesh;

	GameObject(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh);
	virtual ~GameObject();

	std::wstring getName();
	sf::Vector2i getOrigin();
	std::weak_ptr<Animations> getAnimations();
	std::shared_ptr<Collider> getCollider();
	std::shared_ptr<Mesh> getMesh();
};