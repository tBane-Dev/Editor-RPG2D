#include "Cursor.hpp"


Cursor::Cursor() {
	_position = sf::Mouse::getPosition(*window);
	//_hoveredElementGUI = nullptr;
}

Cursor::~Cursor() {

}

void Cursor::update() {
	_position = sf::Mouse::getPosition(*window);
}

void Cursor::handleEvent(const sf::Event& event) {

}

void Cursor::draw()
{
	
}

std::shared_ptr<Cursor> cursor;