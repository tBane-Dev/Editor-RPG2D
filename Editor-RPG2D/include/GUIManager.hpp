#pragma once
#include "SFML/Graphics.hpp"

// std::enable_shared_from_this for this->shared_from_this
class Element : public std::enable_shared_from_this<Element> {
public:

	Element();
	virtual ~Element();

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};

class GUIManager {
public:
	sf::View _view;
	std::shared_ptr<Element> Element_hovered;
	std::shared_ptr<Element> Element_pressed;

	GUIManager();
	~GUIManager();

	void setView();
};

extern std::shared_ptr<GUIManager> GUI_manager;