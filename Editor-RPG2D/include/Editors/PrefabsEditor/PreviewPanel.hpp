#pragma once
#include "Components/Panel.hpp"

class PreviewPanel : public Panel {
public:

	std::unique_ptr<sf::Text> _title;

	PreviewPanel(sf::Vector2i margin);
	~PreviewPanel();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};