#pragma once
#include <SFML/Graphics.hpp>

class CameraOnMap {
	public:
	const static float moveSpeed;

	sf::Vector2f _position;
	sf::View _view;
	bool _isMoving;

	sf::IntRect _visibleRect;

	CameraOnMap();
	~CameraOnMap();

	void setView();

	void handleEvent(const sf::Event& event);
	void update();

};