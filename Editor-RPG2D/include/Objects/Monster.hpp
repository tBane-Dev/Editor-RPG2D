#pragma once
#include "Editors/MapEditor/Map/GameObjectOnMap.hpp"
#include "Editors/MapEditor/Map/Path.hpp"
#include "Animator.hpp"

enum class MonsterState { Idle, Moving, Attacking };
enum class Direction { Down, Left, Right, Up };

class MonsterPrefab : public GameObject {
public:
	int _stepSize;

	MonsterPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, int stepSize, std::shared_ptr<Collider> collider);
	~MonsterPrefab();
};

class Monster : public GameObjectOnMap {
public:

	MonsterState _state;
	sf::Vector2i _basePosition;
	Direction _direction;

	std::shared_ptr<Path> _path;

	Monster(std::weak_ptr<GameObject> prefab);
	~Monster();

	virtual void setPosition(sf::Vector2i position);
	virtual void update();

	virtual void draw();
};