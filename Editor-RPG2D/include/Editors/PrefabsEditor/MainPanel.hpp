#pragma once
#include "Components/Panel.hpp"

class MainPanel : public Panel {
public:

	std::unique_ptr<sf::Text> _title;

	MainPanel(sf::Vector2i margin);
	~MainPanel();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};