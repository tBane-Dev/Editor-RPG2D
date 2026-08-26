#include "Objects/Building/Roof2.hpp"
#include "RenderWindow.hpp"

Roof2::Roof2() : Roof() {

}

Roof2::~Roof2() {

}

int Roof2::getTopOffset(int wallHeight, float scale) {
	float tileSize = 32.f * scale;
	float roofWidth = _tiles[0].size() * tileSize + _roofOverhangSize.x * 2.f * scale;
	float roofHeight = (roofWidth / 2.f) * (24.f / 32.f);

	float topOffset = wallHeight * tileSize + _roofOverhangSize.y * scale + roofHeight - tileSize;
	return topOffset;
}

void Roof2::generate(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale) {
	Roof::generate(tiles, position, scale);
	
	generateOverhang(tiles, position, scale);
	generateShape(tiles, position, scale);
	generateTexture(tiles, position, scale);
}

void Roof2::generateOverhang(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale) {
	_roofOverhangSize = sf::Vector2i(16, 12);
}

void Roof2::generateShape(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale)
{
	float tileSize = 32.f * scale;

	float overhangX = _roofOverhangSize.x * scale;
	float overhangY = _roofOverhangSize.y * scale;

	float roofWidth = tiles[0].size() * tileSize + 2.f * overhangX;

	// długość połaci w osi Y
	float roofDepth = (tiles.size() - 1) * tileSize + 2.f * overhangY;

	float roofHeight = (roofWidth / 2.f) * (24.f / 32.f);
	float centerX = roofWidth / 2.f;

	sf::Color roofColor(127, 15, 15);
	sf::Color frontColor(95, 15, 15);

	_topTriangle.resize(1);
	_bottomTriangle.resize(1);
	_rect.resize(1);

	_topTriangle[0].setPointCount(3);
	_topTriangle[0].setPoint(0, sf::Vector2f(0.f, roofHeight));
	_topTriangle[0].setPoint(1, sf::Vector2f(roofWidth, roofHeight));
	_topTriangle[0].setPoint(2, sf::Vector2f(centerX, 0.f));
	_topTriangle[0].setFillColor(roofColor);
 
	_bottomTriangle[0].setPointCount(3);
	_bottomTriangle[0].setPoint(0, sf::Vector2f(0.f, roofHeight + roofDepth));
	_bottomTriangle[0].setPoint(1, sf::Vector2f(roofWidth, roofHeight + roofDepth));
	_bottomTriangle[0].setPoint(2, sf::Vector2f(centerX, roofDepth));
	_bottomTriangle[0].setFillColor(frontColor);

	_rect[0].setPointCount(4);
	_rect[0].setPoint(0, _topTriangle[0].getPoint(0));
	_rect[0].setPoint(1, _topTriangle[0].getPoint(1));
	_rect[0].setPoint(2,_bottomTriangle[0].getPoint(1));
	_rect[0].setPoint(3, _bottomTriangle[0].getPoint(0));
	_rect[0].setFillColor(roofColor);
}

void Roof2::generateTexture(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale)
{
	float tileSize = 32.f * scale;

	float overhangX = _roofOverhangSize.x * scale;
	float overhangY = _roofOverhangSize.y * scale;

	float roofWidth = tiles[0].size() * tileSize + 2.f * overhangX;
	float roofDepth = (tiles.size() - 1) * tileSize + 2.f * overhangY;
	float roofHeight = (roofWidth / 2.f) * (24.f / 32.f);

	int width = roofWidth;
	int height = roofHeight + roofDepth;

	sf::RenderTexture rtex;
	rtex.resize(sf::Vector2u(width, height));
	rtex.clear(sf::Color::Transparent);

	rtex.draw(_topTriangle[0]);
	rtex.draw(_rect[0]);
	rtex.draw(_bottomTriangle[0]);

	rtex.display();

	_roofTexture = rtex.getTexture();
}


void Roof2::draw(sf::RenderTarget& target, sf::Vector2i position, float scale) {
	
	float wallsHeight = 3.f * 32.f * scale;
	float overhangX = _roofOverhangSize.x * scale;
	float overhangY = _roofOverhangSize.y * scale;

	float roofWidth = _tiles[0].size() * 32.f * scale + 2.f * overhangX;
	float roofHeight = (roofWidth / 2.f) * (24.f / 32.f);

	sf::Sprite sprite(_roofTexture);
	sprite.setPosition(sf::Vector2f(position.x - overhangX, position.y - wallsHeight - overhangY - roofHeight + 32.f * scale));
	target.draw(sprite);
}