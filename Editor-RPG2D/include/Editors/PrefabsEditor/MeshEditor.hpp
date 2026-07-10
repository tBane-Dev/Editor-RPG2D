#pragma once
#include "WindowsManager.hpp"
#include "AnimationsManager.hpp"
#include "Objects/Mesh.hpp"

class MeshEditor : public Main::Window {
public:
	int margin;
	sf::IntRect _innerRect;
	sf::IntRect _canvasRect;

	float _tileSize = 8;

	std::shared_ptr<Animations> _animations;
	float _spriteScale;
	float _spriteOutlineThickness;
	sf::IntRect _spriteRect;

	sf::Vector2i _cursorPosition;
	std::weak_ptr<Shape> _activeShape;

	MeshEditor();
	~MeshEditor();

	virtual void setPosition(sf::Vector2i position);

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};