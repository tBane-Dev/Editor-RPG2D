#include "Editors/MapEditor/Map/CameraOnMap.hpp"
#include "RenderWindow.hpp"
#include "Time.hpp"
#include "Cursor.hpp"
#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/MapEditor/Map/Map.hpp"
#include "DebugLog.hpp"
#include "WindowsManager.hpp"

namespace MapEditor {
	const float CameraOnMap::moveSpeed = 512.0f;


	CameraOnMap::CameraOnMap() {

		_view = sf::View();
		_view.setSize(sf::Vector2f(Main::render_window->getSize()));
		_view.setCenter(sf::Vector2f(0, 0));

		_isMoving = false;
		_zoom = 1.0f;
	}

	CameraOnMap::~CameraOnMap() {

	}

	void CameraOnMap::handleEvent(const sf::Event& event) {
		if (const auto* ms = event.getIf<sf::Event::MouseWheelScrolled>()) {
			
			if (ms->delta > 0)
				if(_zoom < 2.0f)
					_zoom += 0.25f;
			
			if (ms->delta < 0)
				if(_zoom > 0.5f)
				_zoom -= 0.25;

			_view.setSize(sf::Vector2f(Main::render_window->getSize()) / _zoom);
			Main::render_window->setView(_view);
		}
	}

	void CameraOnMap::setView() {
		Main::render_window->setView(_view);
	}

	void CameraOnMap::update() {

		sf::Vector2i margin = sf::Vector2i(256.f * _zoom, 256.f * _zoom);
		_visibleRect.position = sf::Vector2i(_position.x - _view.getSize().x / 2 - margin.x, _position.y - _view.getSize().y / 2 - margin.y);
		_visibleRect.size = sf::Vector2i(_view.getSize().x + 2 * margin.x, _view.getSize().y + 2 * margin.y);

		_isMoving = false;

		if (Main::windows_manager->get_back())
			return;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
			_position.y -= moveSpeed * deltaTime.asSeconds() / _zoom;
			_isMoving = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
			_position.y += moveSpeed * deltaTime.asSeconds() / _zoom;
			_isMoving = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
			_position.x -= moveSpeed * deltaTime.asSeconds() / _zoom;
			_isMoving = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
			_position.x += moveSpeed * deltaTime.asSeconds() / _zoom;
			_isMoving = true;

		}

		_view.setSize(sf::Vector2f(Main::render_window->getSize()) / _zoom);
		_view.setCenter(sf::Vector2f(_position));
	}
}