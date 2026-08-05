#include "Objects/Building/Roof.hpp"
#include "Roofset.hpp"
#include "RenderWindow.hpp"
#include <queue>
#include "DebugLog.hpp"


Roof::Roof() {
	_size = sf::Vector2i(0, 0);
	_tiles.clear();
}

Roof::~Roof() {
	
}


void Roof::generate(sf::Vector2i size, std::vector<int> tiles, sf::Vector2i position, float scale) {
	_size = size;
	_tiles = tiles;
}

void Roof::draw(sf::RenderTarget& target, float scale) {

}

FlatRoof::FlatRoof() : Roof() {
	
}

FlatRoof::~FlatRoof() {

}


void FlatRoof::generateMask(std::vector<int> tiles) {

	if(_size.x <= 0 || _size.y <= 0)
		return;

	int w = _size.x;
	int h = _size.y;

	std::vector<bool> solid(w * h, false);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {

			if (tiles[y * w + x] > -1)
				solid[y * w + x] = true;

		}
	}

	int fw = w + 2;
	int fh = h + 2;

	std::vector<bool> outside(fw * fh, false);
	std::queue<sf::Vector2i> q;

	q.push(sf::Vector2i(0, 0));
	outside[0] = true;

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

			if (outside[ny * fw + nx])
				continue;

			int ox = nx - 1;
			int oy = ny - 1;

			bool blocked = false;

			if (ox >= 0 && oy >= 0 && ox < w && oy < h)
				blocked = solid[oy * w + ox];

			if (blocked)
				continue;

			outside[ny * fw + nx] = true;
			q.push(sf::Vector2i(nx, ny));
		}
	}

	_mask.assign(w * h, -1);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {

			bool isOutside = outside[(y + 1) * fw + (x + 1)];

			if (!isOutside)
				_mask[y * w + x] = 0;   // pole wewnątrz budynku
		}
	}
}

void FlatRoof::generateParts(sf::Vector2i position, float scale) {

	_parts.clear();

	for (int y = 0; y < _size.y; y++) {
		for (int x = 0; x < _size.x; x++) {

			int id = _mask[y * _size.x + x];
			int left = (x > 0) ? _mask[y * _size.x + (x - 1)] : -1;
			int right = (x < _size.x - 1) ? _mask[y * _size.x + (x + 1)] : -1;
			int top = (y - 1 >= 0) ? _mask[(y - 1) * _size.x + x] : -1;
			int bottom = (y + 1 < _size.y) ? _mask[(y + 1) * _size.x + x] : -1;

			if (id > -1) {


				sf::IntRect textureRect(roofset->_groups[id]->roofs[0].get(), sf::Vector2i(32, 32));

				int left = (x > 0) ? _mask[y * _size.x + (x - 1)] : -1;
				int right = (x < _size.x - 1) ? _mask[y * _size.x + (x + 1)] : -1;
				int top = (y - 1 >= 0) ? _mask[(y - 1) * _size.x + x] : -1;
				int bottom = (y + 1 < _size.y) ? _mask[(y + 1) * _size.x + x] : -1;

				int topLeft = (x > 0 && y - 1 >= 0) ? _mask[(y - 1) * _size.x + (x - 1)] : -1;
				int topRight = (x < _size.x - 1 && y - 1 >= 0) ? _mask[(y - 1) * _size.x + (x + 1)] : -1;
				int bottomLeft = (x > 0 && y + 1 < _size.y) ? _mask[(y + 1) * _size.x + (x - 1)] : -1;
				int bottomRight = (x < _size.x - 1 && y + 1 < _size.y) ? _mask[(y + 1) * _size.x + (x + 1)] : -1;

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
	
	int miniSize = 4;
	int miniCount = 32 / miniSize;
	
	int width = _size.x * miniCount + 2;
	int height = _size.y * miniCount + 2;
	
	std::vector<uint8_t> occupied(width * height, 0);
	std::vector<uint8_t> border(width * height, 0);
	
	auto index = [width](int x, int y) {
		return y * width + x;
	};
	
	for (int y = 0; y < _size.y; ++y) {
		for (int x = 0; x < _size.x; ++x) {
			if (_mask[y * _size.x + x] < 0)
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

void FlatRoof::generate(sf::Vector2i size, std::vector<int> tiles, sf::Vector2i position, float scale) {
	Roof::generate(size, tiles, position, scale);

	generateMask(tiles);
	generateParts(position, scale);
	generateOverhang(position, scale);
}

void FlatRoof::draw(sf::RenderTarget& target, float scale) {

	// overhang
	sf::RenderStates roofOverhangStates;
	roofOverhangStates.texture = roofset->_overhangTexture->_texture.get();
	target.draw(_roofOverhangVertexArray, roofOverhangStates);

	// parts of roof
	for (auto& part : _parts) {
		if (part) {
			part->draw(target, scale);
		}
	}
}

GableRoof::GableRoof() : Roof() {

}

GableRoof::~GableRoof() {

}


void GableRoof::generate(sf::Vector2i size, std::vector<int> tiles, sf::Vector2i position, float scale) {
	Roof::generate(size, tiles, position, scale);
}

void GableRoof::draw(sf::RenderTarget& target, float scale) {

}