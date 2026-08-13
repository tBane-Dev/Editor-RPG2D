#pragma once
#include "Components/Palette/Slot.hpp"
#include "Objects/Building/Building.hpp"

class BuildingSlot : public Slot {
public:

	std::shared_ptr<sf::Texture> _buildingCloseTexture;
	std::shared_ptr<sf::Texture> _buildingOpenTexture;

	BuildingSlot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> inactiveTexture, sf::Vector2i position = sf::Vector2i(0, 0), std::shared_ptr<BuildingPrefab> buildingPrefab = nullptr);
	~BuildingSlot();

	void generate();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};