#include "Editors/MapEditor/Map/CameraOnMap.hpp"
#include "Window.hpp"
#include "Time.hpp"
#include "Cursor.hpp"
#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "DebugLog.hpp"

const float CameraOnMap::moveSpeed = 512.0f;


CameraOnMap::CameraOnMap() {

	_view = sf::View();
	_view.setSize(sf::Vector2f(window->getSize()));
	_view.setCenter(sf::Vector2f(0,0));
}

CameraOnMap::~CameraOnMap() {

}

void CameraOnMap::handleEvent(const sf::Event& event) {

}

void CameraOnMap::setView() {
	window->setView(_view);
}

void CameraOnMap::update() {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		_position.y -= moveSpeed * deltaTime.asSeconds();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		_position.y += moveSpeed * deltaTime.asSeconds();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		_position.x -= moveSpeed * deltaTime.asSeconds();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		_position.x += moveSpeed * deltaTime.asSeconds();
	}

	_view.setSize(sf::Vector2f(window->getSize()));
	_view.setCenter(sf::Vector2f(_position));
}