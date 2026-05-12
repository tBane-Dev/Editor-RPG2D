#pragma once
#include "GUIManager.hpp"
#include "Controls/ButtonWithSprite.hpp"
#include "Editors/MapEditor/Objects/Object.hpp"

class Slot : public ButtonWithSprite {
public:

	std::shared_ptr<Object> _object;
	int _frame = 0;
	std::shared_ptr<Texture> _texture = nullptr;
	std::shared_ptr<sf::Sprite> _sprite = nullptr;
	sf::Time _animationTimer;

	Slot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position = sf::Vector2i(0, 0));
	~Slot();

	void setObject(std::shared_ptr<Object> object);
	void animate();

	void update();
	void draw();
};

class Palette : public Element {
public:

	sf::IntRect _rect;
	std::vector<std::shared_ptr<Slot>> _slots;

	Palette();
	~Palette();

	sf::Vector2i getSize();
	void setPosition(sf::Vector2i position);

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};
