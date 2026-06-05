#pragma once
#include "Controls/ButtonWithSprite.hpp"

class ButtonWithTextAndSprite : public ButtonWithSprite{
public:

	std::unique_ptr<sf::Text> _text;

	ButtonWithTextAndSprite(std::wstring text, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position = sf::Vector2i(0, 0));
	~ButtonWithTextAndSprite();

	virtual void setPosition(sf::Vector2i position);
	virtual void draw();

};
