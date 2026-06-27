#pragma once
#include "Controls/Button.hpp"

enum class ButtonWithTextAlignment { Left, Center, Right };

class ButtonWithText : public Button {
public:

	ButtonWithTextAlignment _textAlignment;
	std::unique_ptr<sf::Text> _text;

	ButtonWithText(std::wstring text, sf::Vector2i size, sf::Vector2i position = sf::Vector2i(0, 0));
	~ButtonWithText();

	void setAlignment(ButtonWithTextAlignment alignment);
	virtual void setPosition(sf::Vector2i position);
	virtual void draw();

};
