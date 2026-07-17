#pragma once
#include "Components/Palette/Slot.hpp"

class FloorSlot : public Slot {
public:

	static std::shared_ptr<Texture> _floorset;
	int _index = -1;

	FloorSlot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> inactiveTexture, sf::Vector2i position = sf::Vector2i(0, 0), int index = -1);
	~FloorSlot();

	void setIndex(int index);

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};