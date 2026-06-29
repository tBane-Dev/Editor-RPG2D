#include "Components/Palette/Slot.hpp"

Slot::Slot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> inactiveTexture, sf::Vector2i position) : ButtonWithSprite(texture, hoverTexture, pressTexture, inactiveTexture, position) {

}

Slot::~Slot() {

}
