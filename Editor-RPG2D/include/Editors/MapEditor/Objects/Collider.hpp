#pragma once
#include <SFML/Graphics.hpp>

enum class ColliderType { Rectangular, Circular };

class Collider {
public:
	ColliderType _type;

	Collider();
	virtual ~Collider();
	virtual void draw(sf::Vector2i position, sf::Vector2f scale = sf::Vector2f(1,1));
};

class RectangularCollider : public Collider {
public:
	sf::IntRect _rect;

	RectangularCollider(int x, int y, int w, int h);
	virtual ~RectangularCollider();
	virtual void draw(sf::Vector2i position, sf::Vector2f scale = sf::Vector2f(1,1));
};

class CircularCollider : public Collider {
public:
	int _x;
	int _y;
	int _radiusX;
	int _radiusY;

	CircularCollider(int x, int y, int radiusX, int radiusY);
	virtual ~CircularCollider();
	virtual void draw(sf::Vector2i position, sf::Vector2f scale = sf::Vector2f(1,1));
};