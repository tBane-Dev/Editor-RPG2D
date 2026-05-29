#pragma once
#include "Controls/ButtonWithSprite.hpp"
#include "Objects/Object.hpp"
#include "Animator.hpp"

class Slot : public ButtonWithSprite {
public:
	std::shared_ptr<Object> _object;
	std::shared_ptr<Animator> _animator;

	Slot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position = sf::Vector2i(0, 0));
	~Slot();
};
