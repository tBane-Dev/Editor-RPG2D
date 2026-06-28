#pragma once
#include "Controls/Button.hpp"

class ButtonWithSprite : public Button{
public:

	std::shared_ptr<Texture> _texture;
	std::shared_ptr<Texture> _hoverTexture;
	std::shared_ptr<Texture> _pressTexture;
	std::shared_ptr<Texture> _inactiveTexture;

	ButtonWithSprite(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> inactiveTexture = nullptr, sf::Vector2i position = sf::Vector2i(0, 0));
	~ButtonWithSprite();

	virtual void draw();

};
