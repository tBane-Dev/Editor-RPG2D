#pragma once
#include "WindowsManager.hpp"
#include "AnimationsManager.hpp"

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

	MeshEditor();
	~MeshEditor();

	virtual void setPosition(sf::Vector2i position);
	virtual void draw();
};