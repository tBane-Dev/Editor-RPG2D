#pragma once
#include "Objects/Object.hpp"
#include "Objects/GameObject.hpp"
#include "Animator.hpp"

class GameObjectOnMap : public Object {
public:
	std::shared_ptr<GameObject> _prefab;
	std::shared_ptr<Animator> _animator;
    sf::Vector2i _position;

	GameObjectOnMap(std::shared_ptr<GameObject> prefab);
	~GameObjectOnMap();

	void drawFrame();

	virtual void setPosition(sf::Vector2i position);
	virtual void update();
	virtual void draw();
};