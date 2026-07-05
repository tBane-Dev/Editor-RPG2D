#include "Editors/MapEditor/Map/CameraOnMap.hpp"
#include "RenderWindow.hpp"
#include "Time.hpp"
#include "Cursor.hpp"
#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "DebugLog.hpp"
#include "WindowsManager.hpp"

const float CameraOnMap::moveSpeed = 512.0f;


CameraOnMap::CameraOnMap() {

	_view = sf::View();
	_view.setSize(sf::Vector2f(Main::render_window->getSize()));
	_view.setCenter(sf::Vector2f(0,0));

	_isMoving = false;
}

CameraOnMap::~CameraOnMap() {

}

void CameraOnMap::handleEvent(const sf::Event& event) {

}

void CameraOnMap::setView() {
	Main::render_window->setView(_view);
}

void CameraOnMap::update() {

	sf::Vector2i margin = sf::Vector2i(256, 256);
	_visibleRect.position = sf::Vector2i(_position.x - Main::render_window->getSize().x / 2 - margin.x, _position.y - Main::render_window->getSize().y / 2 - margin.y);
	_visibleRect.size = sf::Vector2i(Main::render_window->getSize().x + 2 * margin.x, Main::render_window->getSize().y + 2 * margin.y);

	_isMoving = false;

	if (Main::windows_manager->get_back())
		return;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		_position.y -= moveSpeed * deltaTime.asSeconds();
		_isMoving = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		_position.y += moveSpeed * deltaTime.asSeconds();
		_isMoving = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		_position.x -= moveSpeed * deltaTime.asSeconds();
		_isMoving = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		_position.x += moveSpeed * deltaTime.asSeconds();
		_isMoving = true;
		
	}

	_view.setSize(sf::Vector2f(Main::render_window->getSize()));
	_view.setCenter(sf::Vector2f(_position));
}