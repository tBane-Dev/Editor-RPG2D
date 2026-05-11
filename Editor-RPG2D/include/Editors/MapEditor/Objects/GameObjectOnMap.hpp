#pragma once
#include "Editors/MapEditor/Objects/Object.hpp"
#include "Editors/MapEditor/Objects/GameObject.hpp"

class GameObjectOnMap : public Object {
public:
	std::shared_ptr<GameObject> _prefab;
    sf::Vector2i _position;
	int _frame;

	GameObjectOnMap(std::shared_ptr<GameObject> prefab);
	~GameObjectOnMap();

	virtual void setPosition(sf::Vector2i position);
	virtual void update();
	virtual void draw();
};