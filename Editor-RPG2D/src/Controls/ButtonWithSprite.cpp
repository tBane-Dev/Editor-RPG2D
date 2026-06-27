#include "Controls/ButtonWithSprite.hpp"
#include "Theme.hpp"
#include "Time.hpp"
#include "Cursor.hpp"
#include "RenderWindow.hpp"

ButtonWithSprite::ButtonWithSprite(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position)
: Button() {

	_texture = texture;
	_hoverTexture = hoverTexture;
	_pressTexture = pressTexture;

	setSize(sf::Vector2i(_texture->_texture->getSize()));
	setPosition(position);

}

ButtonWithSprite::~ButtonWithSprite() {
	
}

void ButtonWithSprite::draw() {

	Button::draw();

	std::shared_ptr<Texture> texture;
	switch (_state)
	{
	case ButtonState::Idle:
		texture = _texture;
		break;
	case ButtonState::Hover:	
		texture = _hoverTexture;
		break;
	case ButtonState::Pressed:
		texture = _pressTexture;
		break;
	default:
		texture = _texture;
		break;
	}

	sf::Sprite sprite(*texture->_texture);
	sprite.setPosition(sf::Vector2f(_rect.position));
	Main::render_window->draw(sprite);

}