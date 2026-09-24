#pragma once
#include <SFML/Graphics.hpp>

namespace MapEditor {
	class CameraOnMap {
	public:
		const static float moveSpeed;

		sf::Vector2f _position;
		sf::View _view;
		bool _isMoving;
		float _zoom;

		sf::IntRect _visibleRect;

		CameraOnMap();
		~CameraOnMap();

		void setView();

		void handleEvent(const sf::Event& event);
		void update();

	};
}