#include "Components/Palette/FloorSlot.hpp"
#include "RenderWindow.hpp"

std::shared_ptr<Texture> FloorSlot::_floorset = nullptr;

FloorSlot::FloorSlot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> inactiveTexture, sf::Vector2i position, int index) : Slot(texture, hoverTexture, pressTexture, inactiveTexture, position) {
	if(_floorset == nullptr)
		_floorset = textures_manager->getTexture(L"assets\\tex\\floorset.png");

	_index = index;
}

FloorSlot::~FloorSlot() {

}

void FloorSlot::setIndex(int index) {
	_index = index;
}

void FloorSlot::cursorHover() {
	ButtonWithSprite::cursorHover();
}

void FloorSlot::handleEvent(const sf::Event& event) {
	ButtonWithSprite::handleEvent(event);
}

void FloorSlot::update() {
	ButtonWithSprite::update();
}

void FloorSlot::draw() {
	Slot::draw();

	if(_index < 0)
		return;

	sf::Vector2f size(96, 96);
	sf::Vector2f offset((120.f - size.x) / 2.f, (120.f - size.y) / 2.f);

	sf::Sprite sprite(*_floorset->_texture);
	sprite.setScale(sf::Vector2f(size.x / 64.f, size.y / 64.f));
	sprite.setPosition(sf::Vector2f(_rect.position) + offset);
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(_index * 64, 0), sf::Vector2i(64, 64)));

	Main::render_window->draw(sprite);
}
