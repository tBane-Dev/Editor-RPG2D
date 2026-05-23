#include "Controls/ButtonWithTextAndSprite.hpp"
#include "Theme.hpp"
#include "Window.hpp"

ButtonWithTextAndSprite::ButtonWithTextAndSprite(std::wstring text, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position)
: ButtonWithSprite(texture, hoverTexture, pressTexture, position) {
	
	_text = std::make_unique<sf::Text>(basicFont, text, basic_text_size);
	_text->setFillColor(basic_text_color);
	
	_text->setPosition(sf::Vector2f(
		position.x + (getSize().x - _text->getLocalBounds().size.x) / 2, 
		position.y + (getSize().y - _text->getFont().getLineSpacing(_text->getCharacterSize())) / 2
	));
}

ButtonWithTextAndSprite::~ButtonWithTextAndSprite() {
	
}

void ButtonWithTextAndSprite::draw() {

	ButtonWithSprite::draw();

	if (_text) {
		window->draw(*_text);
	}

}