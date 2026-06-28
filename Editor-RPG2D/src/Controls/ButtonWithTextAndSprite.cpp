#include "Controls/ButtonWithTextAndSprite.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"

ButtonWithTextAndSprite::ButtonWithTextAndSprite(std::wstring text, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> inactiveTexture, sf::Vector2i position)
: ButtonWithSprite(texture, hoverTexture, pressTexture, inactiveTexture, position) {
	
	_text = std::make_unique<sf::Text>(basicFont, text, basic_text_size);
	_text->setFillColor(basic_text_color);
	
	_text->setPosition(sf::Vector2f(
		position.x + (getSize().x - _text->getLocalBounds().size.x) / 2, 
		position.y + (getSize().y - _text->getFont().getLineSpacing(_text->getCharacterSize())) / 2
	));
}

ButtonWithTextAndSprite::~ButtonWithTextAndSprite() {
	
}

void ButtonWithTextAndSprite::setPosition(sf::Vector2i position) {
	ButtonWithSprite::setPosition(position);

	_text->setPosition(sf::Vector2f(
		position.x + (getSize().x - _text->getLocalBounds().size.x) / 2,
		position.y + (getSize().y - _text->getFont().getLineSpacing(_text->getCharacterSize())) / 2
	));
}

void ButtonWithTextAndSprite::draw() {

	ButtonWithSprite::draw();

	if (_text) {
		Main::render_window->draw(*_text);
	}

}