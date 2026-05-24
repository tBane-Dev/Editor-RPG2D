#pragma once
#include "Components/Panel.hpp"
#include "Controls/TextInput.hpp"

class ColliderPanel : public Panel {
public:

	std::unique_ptr<sf::Text> _title;

	std::unique_ptr<sf::Text> _typeLabel;
	std::unique_ptr<sf::Text> _xLabel;
	std::unique_ptr<sf::Text> _yLabel;
	std::unique_ptr<sf::Text> _wLabel;
	std::unique_ptr<sf::Text> _hLabel;

	std::shared_ptr<TextInput> _type;
	std::shared_ptr<TextInput> _x;
	std::shared_ptr<TextInput> _y;
	std::shared_ptr<TextInput> _w;
	std::shared_ptr<TextInput> _h;

	ColliderPanel(sf::Vector2i margin);
	~ColliderPanel();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};