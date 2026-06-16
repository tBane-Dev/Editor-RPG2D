#include "Components/Palette/TerrainSlot.hpp"
#include "Objects/Terrain.hpp"
#include "ShaderManager.hpp"
#include "Time.hpp"
#include "RenderWindow.hpp"

sf::Vector2i TerrainSlot::tileRects[3][3] =
{
	{ {8, 0},  {12, 0}, {4, 0} },
	{ {10, 0}, {15, 0}, {5, 0} },
	{ {2, 0},  {3, 0},  {1, 0} }
};

TerrainSlot::TerrainSlot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position) : Slot(texture, hoverTexture, pressTexture, position) {
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

TerrainSlot::~TerrainSlot() {

}

void TerrainSlot::cursorHover() {
	ButtonWithSprite::cursorHover();
}

void TerrainSlot::handleEvent(const sf::Event& event) {
	ButtonWithSprite::handleEvent(event);
}

void TerrainSlot::update() {
	ButtonWithSprite::update();
}

void TerrainSlot::draw() {
	Slot::draw();

	if (_object.expired())
		return;

	std::weak_ptr<Terrain> terrain = std::dynamic_pointer_cast<Terrain>(_object.lock());

	if (terrain.expired())
		return;

	sf::RenderStates states;
	states.shader = &*terrain_shader;

	terrain_shader->setUniform("resolution", sf::Vector2f(Main::render_window->getSize()));
	terrain_shader->setUniform("time", currentTime.asSeconds());
	terrain_shader->setUniform("startPos", sf::Vector2f(_rect.position));
	std::shared_ptr<Texture> texture = textures_manager->getTexture(L"assets\\tex\\tileset.png");

	sf::Vector2f size(132.f, 132.f);
	sf::Vector2f tileSize(size.x / 3.f, size.y / 3.f);
	sf::Vector2f offset((160.f - size.x) / 2.f, (160.f - size.y) / 2.f);

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {

			sf::Sprite sprite(*texture->_texture);
			sprite.setScale(sf::Vector2f(tileSize.x / 16.f,tileSize.y / 16.f));
			sprite.setPosition(sf::Vector2f(_rect.position) + offset + sf::Vector2f(x * tileSize.x, y * tileSize.y));
			sprite.setTextureRect(sf::IntRect(sf::Vector2i(tileRects[y][x].x * 64, terrain.lock()->_id * 64), sf::Vector2i(16, 16)));

			Main::render_window->draw(sprite, states);
		}
	}
}
