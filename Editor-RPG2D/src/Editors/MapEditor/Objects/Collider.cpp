#include "Editors/MapEditor/Objects/Collider.hpp"
#include "Window.hpp"

Collider::Collider() {

}

Collider::~Collider() {

}

void Collider::draw(sf::Vector2i position, sf::Vector2f scale) {

}

RectangularCollider::RectangularCollider(int x, int y, int w, int h) : Collider() {
	_type = ColliderType::Rectangular;
	_rect.position.x = x;
	_rect.position.y = y;
	_rect.size.x = w;
	_rect.size.y = h;
}

RectangularCollider::~RectangularCollider() {

}

void RectangularCollider::draw(sf::Vector2i position, sf::Vector2f scale) {
	sf::RectangleShape collider(sf::Vector2f(_rect.size));
	collider.setFillColor(sf::Color(255, 0, 0, 128));
	collider.setPosition(sf::Vector2f(position));
	window->draw(collider);
}

CircularCollider::CircularCollider(int x, int y, int radiusX, int radiusY) : Collider() {
	_type = ColliderType::Circular;
	_x = x;
	_y = y;
	_radiusX = radiusX;
	_radiusY = radiusY;
}

CircularCollider::~CircularCollider() {

}

void CircularCollider::draw(sf::Vector2i position, sf::Vector2f scale) {

	sf::CircleShape collider((float)_radiusX);
	collider.setFillColor(sf::Color(255, 0, 0, 128));
	collider.setScale(sf::Vector2f(scale.x, scale.y * ((float)_radiusY / (float)_radiusX)));
	collider.setOrigin(sf::Vector2f((float)_radiusX, (float)_radiusX));
	collider.setPosition(sf::Vector2f(position));
	window->draw(collider);
}