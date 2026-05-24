#pragma once
#include "Components/Panel.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"

class MeshPanel : public Panel {
public:

	std::unique_ptr<sf::Text> _title;
	std::shared_ptr<ButtonWithTextAndSprite> _set_mesh;

	MeshPanel(sf::Vector2i margin);
	~MeshPanel();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};