#include "Objects/Collider.hpp"
#include "RenderWindow.hpp"

Collider::Collider() {

}

Collider::~Collider() {

}

bool Collider::cursorHover(sf::Vector2i cursorPosition, sf::Vector2i position, sf::Vector2f scale) {
	return false;
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

bool RectangularCollider::cursorHover(sf::Vector2i cursorPosition, sf::Vector2i position, sf::Vector2f scale) {
	sf::IntRect rect;

	rect.position.x = position.x + (int)((float)_rect.position.x * scale.x);
	rect.position.y = position.y + (int)((float)_rect.position.y * scale.y);

	rect.size.x = (int)((float)_rect.size.x * scale.x);
	rect.size.y = (int)((float)_rect.size.y * scale.y);

	return rect.contains(cursorPosition);
}

void RectangularCollider::draw(sf::Vector2i position, sf::Vector2f scale) {
	sf::RectangleShape collider(sf::Vector2f(_rect.size));
	collider.setFillColor(sf::Color(255, 0, 0, 128));
	collider.setPosition(sf::Vector2f(position));
	Main::render_window->draw(collider);
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

bool CircularCollider::cursorHover(sf::Vector2i cursorPosition, sf::Vector2i position, sf::Vector2f scale) {
	float centerX = (float)position.x + (float)_x * scale.x;
	float centerY = (float)position.y + (float)_y * scale.y;

	float radiusX = (float)_radiusX * scale.x;
	float radiusY = (float)_radiusY * scale.y;

	float dx = ((float)cursorPosition.x - centerX) / radiusX;
	float dy = ((float)cursorPosition.y - centerY) / radiusY;

	return dx * dx + dy * dy <= 1.f;
}

void CircularCollider::draw(sf::Vector2i position, sf::Vector2f scale) {

	sf::CircleShape collider((float)_radiusX);

	collider.setFillColor(sf::Color(255, 0, 0, 128));

	collider.setOrigin(sf::Vector2f(
		(float)_radiusX,
		(float)_radiusX
	));

	collider.setScale(sf::Vector2f(
		scale.x,
		scale.y * ((float)_radiusY / (float)_radiusX)
	));

	collider.setPosition(sf::Vector2f(position));

	Main::render_window->draw(collider);
}