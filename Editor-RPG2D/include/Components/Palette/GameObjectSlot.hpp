#pragma once
#include "Components/Palette/Slot.hpp"
#include "Editors/MapEditor/Objects/Object.hpp"

class GameObjectSlot : public Slot {
public:
	
	std::shared_ptr<Texture> _objectTexture;
	std::shared_ptr<sf::Sprite> _objectSprite;

	GameObjectSlot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position = sf::Vector2i(0, 0));
	~GameObjectSlot();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};
