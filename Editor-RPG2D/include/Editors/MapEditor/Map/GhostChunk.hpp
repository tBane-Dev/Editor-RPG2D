#pragma once
#include <SFML/Graphics.hpp>
#include "GUIManager.hpp"
#include "Controls/ButtonWithSprite.hpp"

class GhostChunk : public Element {
public:

	bool _isVisible = false;
	sf::IntRect _rect;
	std::shared_ptr<ButtonWithSprite> _button;

	GhostChunk();
	~GhostChunk();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};
