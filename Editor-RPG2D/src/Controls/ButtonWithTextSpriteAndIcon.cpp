#include "Controls/ButtonWithTextSpriteAndIcon.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"

ButtonWithTextSpriteAndIcon::ButtonWithTextSpriteAndIcon(std::wstring text, 
	std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> inactiveTexture,
	std::shared_ptr<Texture> icon_texture, std::shared_ptr<Texture> icon_hoverTexture, std::shared_ptr<Texture> icon_pressTexture, 
	sf::Vector2i position)
: ButtonWithSprite(texture, hoverTexture, pressTexture, inactiveTexture, position) {
	
	_text = std::make_unique<sf::Text>(basicFont, text, basic_text_size);
	_text->setFillColor(basic_text_color);
	
	_text->setPosition(sf::Vector2f(
		position.x + (getSize().x - _text->getLocalBounds().size.x) / 2, 
		position.y + (getSize().y - _text->getFont().getLineSpacing(_text->getCharacterSize())) / 2
	));

	_icon_texture = icon_texture;
	_icon_hoverTexture = icon_hoverTexture;
	_icon_pressTexture = icon_pressTexture;
}

ButtonWithTextSpriteAndIcon::~ButtonWithTextSpriteAndIcon() {
	
}

void ButtonWithTextSpriteAndIcon::setPosition(sf::Vector2i position) {
	ButtonWithSprite::setPosition(position);

	sf::Vector2i size;
	size.x = size.y = (_texture->getSize().y - _icon_texture->getSize().y);

	sf::Vector2i iconPos;
	iconPos.x = _rect.position.x + size.x/2;
	iconPos.y = _rect.position.y + size.y/2 + 2;

	_text->setPosition(sf::Vector2f(
		iconPos.x + _icon_texture->getSize().x,
		position.y + (getSize().y - _text->getFont().getLineSpacing(_text->getCharacterSize())) / 2
	));
}

void ButtonWithTextSpriteAndIcon::draw() {

	ButtonWithSprite::draw();

	if (_text) {
		Main::render_window->draw(*_text);
	}

	std::shared_ptr<Texture> icon_texture;
	switch (_state)
	{
	case ButtonState::Idle:
		icon_texture = _icon_texture;
		break;
	case ButtonState::Hover:
		icon_texture = _icon_hoverTexture;
		break;
	case ButtonState::Pressed:
		icon_texture = _icon_pressTexture;
		break;
	default:
		icon_texture = _icon_texture;
		break;
	}

	sf::Sprite sprite(*icon_texture->_texture);
	sf::Vector2i size;
	size.x = size.y = (_texture->getSize().y - _icon_texture->getSize().y);
	sf::Vector2i iconPos;
	iconPos.x = _rect.position.x + size.x / 2;
	iconPos.y = _rect.position.y + size.y / 2 + 2;
	sprite.setPosition(sf::Vector2f(iconPos));
	Main::render_window->draw(sprite);

}