#pragma once
#include "Objects/Object.hpp"
#include "Objects/GameObject.hpp"
#include "Animator.hpp"

class GameObjectOnMap : public Object {
public:
	std::weak_ptr<GameObject> _prefab;
	std::shared_ptr<Animator> _animator;
    sf::Vector2i _position;

	GameObjectOnMap(std::weak_ptr<GameObject> prefab);
	~GameObjectOnMap();

	void drawFrame();

	virtual sf::Vector2i getPosition();
	virtual void setPosition(sf::Vector2i position);
	virtual void update();
	virtual void draw();
};