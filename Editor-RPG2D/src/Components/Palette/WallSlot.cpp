#include "Components/Palette/WallSlot.hpp"
#include "Objects/Wall.hpp"
#include "Wallset.hpp"
#include "ShadersManager.hpp"
#include "Time.hpp"
#include "RenderWindow.hpp"

int WallSlot::tileRects[3][3] =
{
	{ 8, 7, 9 },
	{ 10, 7, 11 },
	{ 48, 49, 50 }
};

WallSlot::WallSlot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> inactiveTexture, sf::Vector2i position) : Slot(texture, hoverTexture, pressTexture, inactiveTexture, position) {
	
	_topLeftSprite = nullptr;
	_topSprite = nullptr;
	_topRightSprite = nullptr;
	_leftSprite = nullptr;
	_centerSprite = nullptr;
	_rightSprite = nullptr;
	_bottomLeftSprite = nullptr;
	_bottomSprite = nullptr;
	_bottomRightSprite = nullptr;
}

WallSlot::~WallSlot() {

}

void WallSlot::cursorHover() {
	ButtonWithSprite::cursorHover();
}

void WallSlot::handleEvent(const sf::Event& event) {
	ButtonWithSprite::handleEvent(event);
}

void WallSlot::update() {
	ButtonWithSprite::update();
}

void WallSlot::draw() {
	Slot::draw();

	if (_object.expired())
		return;

	std::shared_ptr<Wall> wall = std::dynamic_pointer_cast<Wall>(_object.lock());

	if (!wall)
		return;

	sf::RenderStates states;
	states.shader = &*terrain_shader;

	terrain_shader->setUniform("resolution", sf::Vector2f(Main::render_window->getSize()));
	terrain_shader->setUniform("time", currentTime.asSeconds());
	terrain_shader->setUniform("startPos", sf::Vector2f(_rect.position));

	sf::Vector2f size(96, 96);
	sf::Vector2f tileSize(size.x / 3.f, size.y / 3.f);
	sf::Vector2f offset((120.f - size.x) / 2.f, (120.f - size.y) / 2.f);

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {

			sf::Sprite sprite(*wallset->_texture->_texture);
			sprite.setScale(sf::Vector2f(tileSize.x / 32.f,tileSize.y / 32.f));
			sprite.setPosition(sf::Vector2f(_rect.position) + offset + sf::Vector2f(x * tileSize.x, y * tileSize.y));
			sf::Vector2i textureRectPos = wallset->_groups[std::dynamic_pointer_cast<WallPrefab>(wall->_prefab.lock())->_id]->walls[tileRects[y][x]].get();
			sprite.setTextureRect(sf::IntRect(textureRectPos, sf::Vector2i(32, 32)));

			Main::render_window->draw(sprite, states);
		}
	}
}
