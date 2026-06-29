#pragma once
#include "Components/Palette/Slot.hpp"

class TerrainSlot : public Slot {
public:

	static sf::Vector2i tileRects[3][3];

	std::shared_ptr<sf::Sprite> _topLeftSprite;
	std::shared_ptr<sf::Sprite> _topSprite;
	std::shared_ptr<sf::Sprite> _topRightSprite;
	std::shared_ptr<sf::Sprite> _leftSprite;
	std::shared_ptr<sf::Sprite> _centerSprite;
	std::shared_ptr<sf::Sprite> _rightSprite;
	std::shared_ptr<sf::Sprite> _bottomLeftSprite;
	std::shared_ptr<sf::Sprite> _bottomSprite;
	std::shared_ptr<sf::Sprite> _bottomRightSprite;


	TerrainSlot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> inactiveTexture, sf::Vector2i position = sf::Vector2i(0, 0));
	~TerrainSlot();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};