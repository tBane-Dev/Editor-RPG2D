#include "Editors/MapEditor/Map/CameraOnMap.hpp"
#include "Window.hpp"
#include "Time.hpp"
#include "Cursor.hpp"
#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "DebugLog.hpp"

const float CameraOnMap::zoomSpeed = 0.1f;
const float CameraOnMap::moveSpeed = 512.0f;
const float CameraOnMap::minZoom = 0.125f;
const float CameraOnMap::maxZoom = 4.0f;

CameraOnMap::CameraOnMap() {
	_zoom = 1.0f;

	_view = sf::View();
	_view.setSize(sf::Vector2f(window->getSize()) * _zoom);
	_view.setCenter(sf::Vector2f(0,0));
}

CameraOnMap::~CameraOnMap() {

}

void CameraOnMap::zoom(float delta) {

	sf::Vector2f mouseBeforeZoom = sf::Vector2f(cursorOnMap->_position) - _position;
	float oldZoom = _zoom;

	 _zoom = std::clamp(_zoom + delta, minZoom, maxZoom);

	sf::Vector2f mouseAfterZoom = sf::Vector2f(mouseBeforeZoom) * (_zoom / oldZoom);
	_position += (sf::Vector2f(cursorOnMap->_position) - (_position + mouseAfterZoom));
}

void CameraOnMap::handleEvent(const sf::Event& event) {
	if (const auto* ms = event.getIf<sf::Event::MouseWheelScrolled>(); ms) {
		if (event.getIf<sf::Event::MouseWheelScrolled>()->wheel == sf::Mouse::Wheel::Vertical) {
			zoom(-event.getIf<sf::Event::MouseWheelScrolled>()->delta * zoomSpeed);
		}
	}
}

void CameraOnMap::setView() {
	window->setView(_view);
}

void CameraOnMap::update() {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		_position.y -= moveSpeed * _zoom * deltaTime.asSeconds();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		_position.y += moveSpeed * _zoom * deltaTime.asSeconds();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		_position.x -= moveSpeed * _zoom * deltaTime.asSeconds();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		_position.x += moveSpeed * _zoom * deltaTime.asSeconds();
	}

	_view.setSize(sf::Vector2f(window->getSize()) * _zoom);
	_view.setCenter(sf::Vector2f(_position));
}