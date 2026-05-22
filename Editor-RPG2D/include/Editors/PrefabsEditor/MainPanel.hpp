#pragma once
#include "Components/Panel.hpp"
#include "Controls/TextInput.hpp"

class MainPanel : public Panel {
public:

	std::unique_ptr<sf::Text> _nameText;
	std::shared_ptr<TextInput> _name;

	std::unique_ptr<sf::Text> _typeText;
	std::shared_ptr<TextInput> _type;

	MainPanel(sf::Vector2i margin);
	~MainPanel();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};