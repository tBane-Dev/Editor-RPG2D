#include "Components/Palette/GameObjectSlot.hpp"
#include "RenderWindow.hpp"

GameObjectSlot::GameObjectSlot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> inactiveTexture, sf::Vector2i position) : Slot(texture, hoverTexture, pressTexture, inactiveTexture, position) {
	_object = std::weak_ptr<Object>();
	_animator = nullptr;
}

GameObjectSlot::~GameObjectSlot() {

}

void GameObjectSlot::cursorHover() {
	ButtonWithSprite::cursorHover();
}

void GameObjectSlot::handleEvent(const sf::Event& event) {
	ButtonWithSprite::handleEvent(event);
}

void GameObjectSlot::update() {
	ButtonWithSprite::update();
	if(_animator != nullptr) {
		_animator->update();
	}

}

void GameObjectSlot::draw() {

	Slot::draw();

	if (_object.expired())
		return;

	if (_animator != nullptr) {

		std::shared_ptr<Animations> animations = _animator->getAnimations().lock();
		
		if (!animations)
			return;

		sf::IntRect frameRect = animations->getFrameRect(_animator->_animation, _animator->_frame);

		_objectTexture = animations->getTexture();

		_objectSprite = std::make_shared<sf::Sprite>(*_objectTexture->_texture);
		_objectSprite->setTextureRect(frameRect);

		float scaleX = (float)(80) / (float)frameRect.size.x;
		float scaleY = (float)(80) / (float)frameRect.size.y;

		float scale = std::min(scaleX, scaleY);

		_objectSprite->setScale(sf::Vector2f(scale, scale));
		_objectSprite->setPosition(sf::Vector2f(_rect.position) + sf::Vector2f(40, 40));
		_objectSprite->setOrigin(sf::Vector2f(frameRect.size.x / 2.f, frameRect.size.y / 2.f));

		Main::render_window->draw(*_objectSprite);
	}
}