#pragma once
#include "Components/Palette/Slot.hpp"
#include "Controls/Scrollbar.hpp"

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
	int _selectedSlotId = -1;
	std::shared_ptr<Slot> _selectedSlot = nullptr;

	Slots();
	~Slots();

	sf::Vector2i getPosition();
	sf::Vector2i getSize();
	void setPosition(sf::Vector2i position);
	void createSlots(sf::Vector2i slotsCount);
	void generateScrollbar();
	void loadObjects();
	void updateObjects();
	void setCategory(ObjectType type);
	void setFunction(std::function<void(std::shared_ptr<Slot> slot, int selectedSlotId)> function);
	void selectSlot(int selectedSlotId);
	void selectLastSlot();
	sf::FloatRect getSlotsRect();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};