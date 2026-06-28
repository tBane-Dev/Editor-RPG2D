#include "Components/Palette/Slot.hpp"

Slot::Slot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position) : ButtonWithSprite(texture, hoverTexture, pressTexture, nullptr, position) {

}

Slot::~Slot() {

}
