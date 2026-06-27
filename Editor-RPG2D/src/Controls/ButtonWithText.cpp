#include "Controls/ButtonWithText.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "Time.hpp"


ButtonWithText::ButtonWithText(std::wstring text, sf::Vector2i size, sf::Vector2i position) : Button() {

	setSize(size);

	_text = std::make_unique<sf::Text>(basicFont, text, basic_text_size);
	_text->setFillColor(basic_text_color);

	_textAlignment = ButtonWithTextAlignment::Center;

	setPosition(position);
}

ButtonWithText::~ButtonWithText() {
	
}

void ButtonWithText::setAlignment(ButtonWithTextAlignment alignment) {
	_textAlignment = alignment;
	setPosition(getPosition());
}

void ButtonWithText::setPosition(sf::Vector2i position) {
	Button::setPosition(position);

	if(_textAlignment == ButtonWithTextAlignment::Left) {
		_text->setPosition(sf::Vector2f(
			position.x + 8,
			position.y + (getSize().y - _text->getFont().getLineSpacing(_text->getCharacterSize())) / 2
		));
	}
	else if (_textAlignment == ButtonWithTextAlignment::Right) {
		_text->setPosition(sf::Vector2f(
			position.x + (getSize().x - _text->getLocalBounds().size.x) - 8,
			position.y + (getSize().y - _text->getFont().getLineSpacing(_text->getCharacterSize())) / 2
		));
	}
	else {
		_text->setPosition(sf::Vector2f(
			position.x + (getSize().x - _text->getLocalBounds().size.x) / 2,
			position.y + (getSize().y - _text->getFont().getLineSpacing(_text->getCharacterSize())) / 2
		));
	}
	
}

void ButtonWithText::draw() {

	Button::draw();

	if (_text) {
		Main::render_window->draw(*_text);
	}

}