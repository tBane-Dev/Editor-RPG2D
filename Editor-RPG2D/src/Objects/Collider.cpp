#include "Objects/Collider.hpp"
#include "RenderWindow.hpp"
#include "BinaryWriter.hpp"
#include "BinaryReader.hpp"

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
	sf::Vector2f colliderSize(
		(float)_rect.size.x * scale.x,
		(float)_rect.size.y * scale.y
	);

	sf::Vector2f colliderPosition(
		(float)position.x + (float)_rect.position.x * scale.x,
		(float)position.y + (float)_rect.position.y * scale.y
	);

	sf::RectangleShape collider(colliderSize);
	collider.setFillColor(sf::Color(255, 0, 0, 128));
	collider.setPosition(colliderPosition);

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

void saveCollider(std::shared_ptr<Collider> collider, std::ofstream& saver) {
	BinaryWriter writer(saver);

	writer.write_int8((int)collider->_type);

	if(collider->_type == ColliderType::Rectangular) {
		RectangularCollider* rectangularCollider = dynamic_cast<RectangularCollider*>(collider.get());
		writer.write_int32(rectangularCollider->_rect.position.x);
		writer.write_int32(rectangularCollider->_rect.position.y);
		writer.write_int32(rectangularCollider->_rect.size.x);
		writer.write_int32(rectangularCollider->_rect.size.y);
	}
	else if(collider->_type == ColliderType::Circular) {
		CircularCollider* circularCollider = dynamic_cast<CircularCollider*>(collider.get());
		writer.write_int32(circularCollider->_x);
		writer.write_int32(circularCollider->_y);
		writer.write_int32(circularCollider->_radiusX);
		writer.write_int32(circularCollider->_radiusY);
	}

}

std::shared_ptr<Collider> loadCollider(std::ifstream& loader) {
	BinaryReader reader(loader);

	int type = reader.read_int8();

	if(type == (int)ColliderType::Rectangular) {
		int x = reader.read_int32();
		int y = reader.read_int32();
		int w = reader.read_int32();
		int h = reader.read_int32();
		return std::make_shared<RectangularCollider>(x, y, w, h);
	}
	else if(type == (int)ColliderType::Circular) {
		int x = reader.read_int32();
		int y = reader.read_int32();
		int radiusX = reader.read_int32();
		int radiusY = reader.read_int32();
		return std::make_shared<CircularCollider>(x, y, radiusX, radiusY);
	}

	return nullptr;
}