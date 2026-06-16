#include "Components/Palette/GameObjectSlot.hpp"
#include "RenderWindow.hpp"

GameObjectSlot::GameObjectSlot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position) : Slot(texture, hoverTexture, pressTexture, position) {
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

		std::weak_ptr<Animations> animations = _animator->getAnimations();
		
		if (animations.expired())
			return;

		sf::IntRect frameRect = animations.lock()->getFrameRect(_animator->_animation, _animator->_frame);

		_objectTexture = animations.lock()->getTexture();

		_objectSprite = std::make_shared<sf::Sprite>(*_objectTexture->_texture);
		_objectSprite->setTextureRect(frameRect);

		float frameWidth = (float)(_objectTexture->getSize().x / animations.lock()->_framesCount);
		float frameHeight = (float)(_objectTexture->getSize().y / animations.lock()->_animationsCount);

		_objectSprite->setScale(sf::Vector2f(80.f / frameWidth, 80.f / frameHeight));
		_objectSprite->setPosition(sf::Vector2f(_rect.position) + sf::Vector2f(40, 40));
		_objectSprite->setOrigin(sf::Vector2f(frameWidth / 2.f, frameHeight / 2.f));

		Main::render_window->draw(*_objectSprite);
	}
}