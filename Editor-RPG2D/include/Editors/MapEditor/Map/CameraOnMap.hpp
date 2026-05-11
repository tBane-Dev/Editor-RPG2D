#pragma once
#include <SFML/Graphics.hpp>

class CameraOnMap {
	public:

	const static float zoomSpeed;
	const static float moveSpeed;
	const static float minZoom;
	const static float maxZoom;

	sf::Vector2f _position;
	sf::View _view;
	float _zoom;

	CameraOnMap();
	~CameraOnMap();

	void zoom(float delta);
	void setView();

	void handleEvent(const sf::Event& event);
	void update();

};