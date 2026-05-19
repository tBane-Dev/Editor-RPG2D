#pragma once
#include "Controls/ButtonWithSprite.hpp"
#include "Editors/MapEditor/Objects/Object.hpp"
#include "Animator.hpp"
#include "Controls/Scrollbar.hpp"

class Slot : public ButtonWithSprite {
public:
	std::shared_ptr<Object> _object;
	std::shared_ptr<Animator> _animator;

	std::shared_ptr<Texture> _objectTexture;
	std::shared_ptr<sf::Sprite> _objectSprite;

	Slot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position = sf::Vector2i(0, 0));
	~Slot();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};

class Slots {
public:
	int _main_margin;
	int _top_margin;
	int _outer_margin;
	int _inner_margin;
	sf::IntRect _rect;

	std::unique_ptr<sf::Text> _text;

	ObjectType _type;

	sf::Vector2i _slotsCount;
	std::vector<std::shared_ptr<Slot>> _slots;
	std::shared_ptr<Scrollbar> _scrollbar;
	std::shared_ptr<Slot> _selectedSlot = nullptr;

	Slots();
	~Slots();

	sf::Vector2i getPosition();
	sf::Vector2i getSize();
	void setPosition(sf::Vector2i position);
	void createSlots(sf::Vector2i slotsCount);
	void loadObjects();
	void updateObjects();
	void setCategory(ObjectType type);
	void setFunction(std::function<void(std::shared_ptr<Slot> slot)> function);
	void selectSlot(std::shared_ptr<Slot> slot);
	sf::FloatRect getSlotsRect();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};