#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include "Controls/Button.hpp"
#include "Controls/ButtonWithSprite.hpp"

class NavBarItem : public Button {
public:
	std::filesystem::path _path;
	std::unique_ptr<sf::Text> _text;
	std::shared_ptr<ButtonWithSprite> _arrow;

	NavBarItem(std::filesystem::path path, sf::Vector2i position);
	~NavBarItem();

	virtual void setPosition(sf::Vector2i position);
	virtual sf::Vector2i getSize();
	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};

class NavBar {
public:
	sf::IntRect _rect;
	std::shared_ptr<ButtonWithSprite> _iconPC;
	std::vector<std::shared_ptr<NavBarItem>> _items;

	NavBar(sf::Vector2i position, sf::Vector2i size);
	~NavBar();

	void clear();
	void add(std::filesystem::path path, std::function<void()> onclick_func);
	void pop_back();
	void setPosition(sf::Vector2i position);

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw(); 
};