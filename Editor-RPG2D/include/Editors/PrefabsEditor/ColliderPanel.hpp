#pragma once
#include "Components/Panel.hpp"

class ColliderPanel : public Panel {
public:

	std::unique_ptr<sf::Text> _title;

	ColliderPanel(sf::Vector2i margin);
	~ColliderPanel();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};