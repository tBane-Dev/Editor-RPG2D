#pragma once
#include "Controls/ButtonWithSprite.hpp"

class ButtonWithTextSpriteAndIcon : public ButtonWithSprite{
public:

	std::unique_ptr<sf::Text> _text;

	std::shared_ptr<Texture> _icon_texture;
	std::shared_ptr<Texture> _icon_hoverTexture;
	std::shared_ptr<Texture> _icon_pressTexture;

	ButtonWithTextSpriteAndIcon(std::wstring text, 
		std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, 
		std::shared_ptr<Texture> icon_texture, std::shared_ptr<Texture> icon_icon_hoverTexture, std::shared_ptr<Texture> icon_pressTexture, 
		sf::Vector2i position = sf::Vector2i(0, 0));
	~ButtonWithTextSpriteAndIcon();

	virtual void setPosition(sf::Vector2i position);
	virtual void draw();

};
