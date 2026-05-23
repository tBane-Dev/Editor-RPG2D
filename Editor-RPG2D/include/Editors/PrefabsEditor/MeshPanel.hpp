#pragma once
#include "Components/Panel.hpp"

class MeshPanel : public Panel {
public:

	std::unique_ptr<sf::Text> _title;

	MeshPanel(sf::Vector2i margin);
	~MeshPanel();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};