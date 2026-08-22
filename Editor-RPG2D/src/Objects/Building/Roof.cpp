#include "Objects/Building/Roof.hpp"
#include "Roofset.hpp"
#include "RenderWindow.hpp"
#include <queue>
#include "DebugLog.hpp"


Roof::Roof() {
	_tiles.clear();
	_roofOverhangSize = sf::Vector2i(0, 0);
}

Roof::~Roof() {
	
}


void Roof::generate(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale) {
	_tiles = tiles;
}

void Roof::draw(sf::RenderTarget& target, sf::Vector2i position, float scale) {

}

FlatRoof::FlatRoof() : Roof() {
	_roofOverhangSize = sf::Vector2i(4, 4);
}

FlatRoof::~FlatRoof() {

}


void FlatRoof::generateMask(std::vector<std::vector<int>> tiles) {

	if(tiles.size() <= 0 || tiles[0].size() <= 0)
		return;

	int w = tiles[0].size();
	int h = tiles.size();

	std::vector<std::vector<bool>> solid(h, std::vector<bool>(w, false));

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {

			if (tiles[y][x] > -1)
				solid[y][x] = true;

		}
	}

	int fw = w + 2;
	int fh = h + 2;

	std::vector<std::vector<bool>> outside(fh, std::vector<bool>(fw, false));
	std::queue<sf::Vector2i> q;

	q.push(sf::Vector2i(0, 0));
	outside[0][0] = true;

	const int dx[4] = { 1, -1, 0, 0 };
	const int dy[4] = { 0, 0, 1, -1 };

	while (!q.empty())
	{
		sf::Vector2i p = q.front();
		q.pop();

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = p.x + dx[dir];
			int ny = p.y + dy[dir];

			if (nx < 0 || ny < 0 || nx >= fw || ny >= fh)
				continue;

			if (outside[ny][nx])
				continue;

			int ox = nx - 1;
			int oy = ny - 1;

			bool blocked = false;

			if (ox >= 0 && oy >= 0 && ox < w && oy < h)
				blocked = solid[oy][ox];

			if (blocked)
				continue;

			outside[ny][nx] = true;
			q.push(sf::Vector2i(nx, ny));
		}
	}

	_mask.assign(h, std::vector<int>(w, -1));

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {

			bool isOutside = outside[y + 1][x + 1];

			if (!isOutside)
				_mask[y][x] = 0;   // pole wewnątrz budynku
		}
	}
}

void FlatRoof::generateParts(sf::Vector2i position, float scale) {

	_parts.clear();

	sf::Vector2i size = sf::Vector2i(_mask[0].size(), _mask.size());

	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			int id = _mask[y][x];
			int left = (x > 0) ? _mask[y][x - 1] : -1;
			int right = (x < size.x - 1) ? _mask[y][x + 1] : -1;
			int top = (y - 1 >= 0) ? _mask[y - 1][x] : -1;
			int bottom = (y + 1 < size.y) ? _mask[y + 1][x] : -1;
			if (id > -1) {


				sf::IntRect textureRect(roofset->_groups[id]->roofs[0].get(), sf::Vector2i(32, 32));

				int left = (x > 0) ? _mask[y][x-1] : -1;
				int right = (x < size.x - 1) ? _mask[y][x + 1] : -1;
				int top = (y - 1 >= 0) ? _mask[y - 1][x] : -1;
				int bottom = (y + 1 < size.y) ? _mask[y + 1][x] : -1;

				int topLeft = (x > 0 && y - 1 >= 0) ? _mask[y - 1][x - 1] : -1;
				int topRight = (x < size.x - 1 && y - 1 >= 0) ? _mask[y - 1][x + 1] : -1;
				int bottomLeft = (x > 0 && y + 1 < size.y) ? _mask[y + 1][x - 1] : -1;
				int bottomRight = (x < size.x - 1 && y + 1 < size.y) ? _mask[y + 1][x + 1] : -1;

				int i = 0;
				if (left == -1 && right == -1 && top == -1 && bottom == -1) i = 1;
				else if (left > -1 && right > -1 && top > -1 && bottom > -1) {

					int edgeMask = 0;

					if (topLeft == -1) edgeMask |= 1;
					if (topRight == -1) edgeMask |= 2;
					if (bottomRight == -1) edgeMask |= 4;
					if (bottomLeft == -1) edgeMask |= 8;

					int crossParts[16] = {
						0,		// 0000 - no edges 

						12,		// 0001 - top-left
						13,		// 0010 - top-right
						16,		// 0011 - top-left + top-right

						14,		// 0100 - bottom-right
						20,		// 0101 - top-left + bottom-right
						17,		// 0110 - top-right + bottom-right
						23,		// 0111 - top-left + top-right + bottom-right

						15,		// 1000 - bottom-left
						19,		// 1001 - top-left + bottom-left
						21,		// 1010 - top-right + bottom-left
						22,		// 1011 - top-left + top-right + bottom-left

						18,		// 1100 - bottom-left + bottom-right
						25,		// 1101 - top-left + bottom-left + bottom-right
						24,		// 1110 - top-right + bottom-left + bottom-right
						38		// 1111 - all four edges
					};

					i = crossParts[edgeMask];
				}
				else if (left > -1 && right > -1 && top > -1 && bottom > -1 && topLeft == -1 && topRight == -1 && bottomLeft == -1 && bottomRight == -1) i = 38;

				// H rotated
				else if (topLeft > -1 && top > -1 && topRight == -1 && left > -1 && right > -1 && bottom == -1) i = 30;
				else if (topLeft == -1 && top > -1 && topRight > -1 && left > -1 && right > -1 && bottom == -1) i = 26;
				else if (top == -1 && bottom > -1 && left > -1 && right > -1 && bottomLeft > -1 && bottomRight == -1) i = 28;
				else if (top == -1 && bottom > -1 && left > -1 && right > -1 && bottomLeft == -1 && bottomRight > -1) i = 32;

				// H
				else if (top > -1 && bottom > -1 && left > -1 && right == -1 && topLeft > -1 && bottomLeft == -1) i = 29;
				else if (top > -1 && bottom > -1 && left > -1 && right == -1 && topLeft == -1 && bottomLeft > -1) i = 33;
				else if (top > -1 && bottom > -1 && right > -1 && left == -1 && topRight == -1 && bottomRight > -1) i = 27;
				else if (top > -1 && bottom > -1 && right > -1 && left == -1 && topRight > -1 && bottomRight == -1) i = 31;

				// T przypadki
				else if (top > -1 && left > -1 && right > -1 && bottom == -1 && topLeft == -1 && topRight == -1) i = 34;
				else if (bottom > -1 && left > -1 && right > -1 && top == -1 && bottomLeft == -1 && bottomRight == -1) i = 36;
				else if (top > -1 && bottom > -1 && left > -1 && right == -1 && topLeft == -1 && bottomLeft == -1) i = 37;
				else if (top > -1 && bottom > -1 && right > -1 && left == -1 && topRight == -1 && bottomRight == -1) i = 35;

				else if (top == -1 && bottom == -1) {
					if (left > -1 && right > -1) i = 7;
					if (left == -1 && right > -1) i = 2;
					if (left > -1 && right == -1) i = 3;
				}
				else if (left == -1 && right == -1) {
					if (top > -1 && bottom > -1) i = 6;
					if (top == -1 && bottom > -1) i = 4;
					if (top > -1 && bottom == -1) i = 5;
				}
				else if (top == -1 && bottom > -1) {
					if (left > -1 && right > -1) i = 44;
					if (left == -1 && right > -1) { (bottomRight == -1) ? i = 8 : i = 39; }
					if (left > -1 && right == -1) { (bottomLeft == -1) ? i = 9 : i = 40; };
				}
				else if (top > -1 && bottom == -1) {
					if (left > -1 && right > -1) i = 46;
					if (left == -1 && right > -1) { (topRight == -1) ? i = 10 : i = 41; }
					if (left > -1 && right == -1) { (topLeft == -1) ? i = 11 : i = 42; };
				}
				else if (left == -1) {
					i = 43;
				}
				else if (right == -1) {
					i = 45;
				}

				textureRect.position = roofset->_groups[id]->roofs[i].get();
				std::shared_ptr<FlatRoofPart> part = std::make_shared<FlatRoofPart>(roofset->getPrefab(id), textureRect);
				part->setPosition(sf::Vector2i(position.x + int((float)x * 32.f * scale), position.y + int(((float)y * 32.f - 96.0f) * scale)));
				_parts.push_back(part);
				
			}
			else
				_parts.push_back(nullptr);
		}
	}
}

void FlatRoof::generateOverhang(sf::Vector2i position, float scale) {
	_roofOverhangVertexArray.clear();
	_roofOverhangVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
	
	if (_roofOverhangSize.x <= 0 || _roofOverhangSize.y <= 0)
		return;

	int miniSize = _roofOverhangSize.x;
	int miniCount = 32 / miniSize;
	
	int width = _mask[0].size() * miniCount + 2;
	int height = _mask.size() * miniCount + 2;
	
	std::vector<uint8_t> occupied(width * height, 0);
	std::vector<uint8_t> border(width * height, 0);
	
	auto index = [width](int x, int y) {
		return y * width + x;
	};
	
	for (int y = 0; y < _mask.size(); ++y) {
		for (int x = 0; x < _mask[0].size(); ++x) {
			if (_mask[y][x] < 0)
				continue;
	
			for (int yy = 0; yy < miniCount; ++yy) {
				for (int xx = 0; xx < miniCount; ++xx) {
					occupied[index(
						x * miniCount + xx + 1,
						y * miniCount + yy + 1
					)] = 1;
				}
			}
		}
	}
	
	for (int y = 1; y < height - 1; ++y) {
		for (int x = 1; x < width - 1; ++x) {
	
			if (!occupied[index(x, y)])
				continue;
	
			for (int yy = -1; yy <= 1; ++yy) {
				for (int xx = -1; xx <= 1; ++xx) {
					
					int nx = x + xx;
					int ny = y + yy;
	
					if (!occupied[index(nx, ny)])
						border[index(nx, ny)] = 1;
				}
			}
		}
	}
	
	sf::Vector2i roofPosition(position.x, position.y - int(std::lround((128.f - 32) * scale)));
	
	auto appendTile = [&](int left, int top, int right, int bottom) {
			float ts = static_cast<float>(miniSize);
	
			auto vertex = [](int x, int y, float tx, float ty) {
				return sf::Vertex{
					.position = sf::Vector2f(x, y),
					.texCoords = sf::Vector2f(tx, ty)
				};
			};
	
			_roofOverhangVertexArray.append(vertex(left, top, 0.f, 0.f));
			_roofOverhangVertexArray.append(vertex(right, top, ts, 0.f));
			_roofOverhangVertexArray.append(vertex(right, bottom, ts, ts));
	
			_roofOverhangVertexArray.append(vertex(left, top, 0.f, 0.f));
			_roofOverhangVertexArray.append(vertex(right, bottom, ts, ts));
			_roofOverhangVertexArray.append(vertex(left, bottom, 0.f, ts));
		};
	
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
	
			if (!border[index(x, y)])
				continue;
	
			int miniX = x - 1;
			int miniY = y - 1;
	
			int left = roofPosition.x + int(std::lround(miniX * miniSize * scale));
			int right = roofPosition.x + int(std::lround((miniX + 1) * miniSize * scale));
			int top = roofPosition.y +int(std::lround(miniY * miniSize * scale));
			int bottom = roofPosition.y +int(std::lround((miniY + 1) * miniSize * scale));
	
			appendTile(left, top, right, bottom);
		}
	}
}

void FlatRoof::generateTexture(sf::Vector2i position, float scale) {

	const float tileSize = 32.f * scale;
	const float topPadding = 96.f * scale;
	const float overhangWidth = 4.f * scale;

	sf::Vector2f textureOrigin(
		position.x - overhangWidth,
		position.y - topPadding - overhangWidth
	);

	int width = std::ceil(_mask[0].size() * tileSize + 2.f * overhangWidth) + 1;
	int height = std::ceil(_mask.size() * tileSize + 2.f * overhangWidth) + 1;

	sf::RenderTexture rtex;
	rtex.resize(sf::Vector2u(width, height));
	rtex.clear(sf::Color::Transparent);

	sf::RenderStates states;
	states.texture = roofset->_overhangTexture->_texture.get();
	states.transform.translate(sf::Vector2f(-textureOrigin.x, -textureOrigin.y));
	
	rtex.draw(_roofOverhangVertexArray, states);

	for (auto& part : _parts) {
		if (!part) continue;

		sf::Sprite sprite(*roofset->_texture->_texture);
		sprite.setPosition(sf::Vector2f(part->_position) - textureOrigin);
		sprite.setTextureRect(part->_textureRect);
		sprite.setScale(sf::Vector2f(scale, scale));
		rtex.draw(sprite);
	}

	rtex.display();

	_roofTexture = rtex.getTexture();
}

void FlatRoof::generate(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale) {
	Roof::generate(tiles, position, scale);

	generateMask(tiles);
	generateParts(position, scale);
	generateOverhang(position, scale);
	generateTexture(position, scale);
}

void FlatRoof::draw(sf::RenderTarget& target, sf::Vector2i position, float scale) {

	float topPadding = 96.f * scale;
	float overhangWidth = _roofOverhangSize.x * scale;
	float overhangHeight = _roofOverhangSize.y * scale;

	sf::Sprite sprite(_roofTexture);
	sprite.setPosition(sf::Vector2f(position.x - overhangWidth, position.y - topPadding - overhangHeight));
	target.draw(sprite);
}

GableRoof::GableRoof() : Roof() {

}

GableRoof::~GableRoof() {

}


void GableRoof::generate(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale) {
	Roof::generate(tiles, position, scale);
	
	generateOverhang(tiles, position, scale);
	generateShape(tiles, position, scale);
	generateTexture(tiles, position, scale);
}

void GableRoof::generateOverhang(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale) {
	_roofOverhangSize = sf::Vector2i(16, 12);
}

float GableRoof::getTopOffset(int wallHeight, float scale)
{
	float tileSize = 32.f * scale;
	float roofWidth = _tiles[0].size() * tileSize + _roofOverhangSize.x * 2.f * scale;
	float roofHeight = (roofWidth / 2.f) * (24.f / 32.f);

	float topOffset = wallHeight * tileSize + _roofOverhangSize.y * scale + roofHeight - tileSize;
	return topOffset;
}

void GableRoof::generateShape(
	std::vector<std::vector<int>> tiles,
	sf::Vector2i position,
	float scale)
{
	const float tileSize = 32.f * scale;

	const float overhangX = _roofOverhangSize.x * scale;
	const float overhangY = _roofOverhangSize.y * scale;

	const float roofWidth =
		tiles[0].size() * tileSize + 2.f * overhangX;

	// długość połaci w osi Y
	const float roofDepth =
		(tiles.size() - 1) * tileSize + 2.f * overhangY;

	const float roofHeight =
		(roofWidth / 2.f) * (24.f / 32.f);

	const float centerX = roofWidth / 2.f;

	sf::Color roofColor(127, 15, 15);
	sf::Color frontColor(95, 15, 15);

	_topTriangle.setPointCount(3);
	_topTriangle.setPoint(0, sf::Vector2f(0.f, roofHeight));
	_topTriangle.setPoint(1, sf::Vector2f(roofWidth, roofHeight));
	_topTriangle.setPoint(2, sf::Vector2f(centerX, 0.f));
	_topTriangle.setFillColor(roofColor);
 
	_bottomTriangle.setPointCount(3);
	_bottomTriangle.setPoint(0, sf::Vector2f(0.f, roofHeight + roofDepth));
	_bottomTriangle.setPoint(1, sf::Vector2f(roofWidth, roofHeight + roofDepth));
	_bottomTriangle.setPoint(2, sf::Vector2f(centerX, roofDepth));
	_bottomTriangle.setFillColor(frontColor);

	_rect.setPointCount(4);
	_rect.setPoint(0, _topTriangle.getPoint(0));
	_rect.setPoint(1, _topTriangle.getPoint(1));
	_rect.setPoint(2,_bottomTriangle.getPoint(1));
	_rect.setPoint(3, _bottomTriangle.getPoint(0));
	_rect.setFillColor(roofColor);
}

void GableRoof::generateTexture(
	std::vector<std::vector<int>> tiles,
	sf::Vector2i position,
	float scale)
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

	rtex.draw(_topTriangle);
	rtex.draw(_rect);
	rtex.draw(_bottomTriangle);

	rtex.display();

	_roofTexture = rtex.getTexture();
}


void GableRoof::draw(sf::RenderTarget& target, sf::Vector2i position, float scale) {
	
	float wallsHeight = 3.f * 32.f * scale;
	float overhangX = _roofOverhangSize.x * scale;
	float overhangY = _roofOverhangSize.y * scale;

	float roofWidth = _tiles[0].size() * 32.f * scale + 2.f * overhangX;
	float roofHeight = (roofWidth / 2.f) * (24.f / 32.f);

	sf::Sprite sprite(_roofTexture);
	sprite.setPosition(sf::Vector2f(position.x - overhangX, position.y - wallsHeight - overhangY - roofHeight + 32.f * scale));
	target.draw(sprite);
}