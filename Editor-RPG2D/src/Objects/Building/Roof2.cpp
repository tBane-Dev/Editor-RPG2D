#include "Objects/Building/Roof2.hpp"
#include "RenderWindow.hpp"
#include <queue>
#include <iostream>
#include "DebugLog.hpp"
#include "TexturesManager.hpp"

Roof2::Roof2(int type, int wallHeight) : Roof(type, wallHeight) {

}

Roof2::~Roof2() {

}

void Roof2::generateOverhang(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale) {
	//_roofOverhangSize = sf::Vector2i(16, 12);
	_roofOverhangSize = sf::Vector2i(0, 0);
}

void Roof2::generateMask(std::vector<std::vector<int>> tiles) {

	if (tiles.size() <= 0 || tiles[0].size() <= 0)
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



void Roof2::generateRects()
{
    _rects.clear();

    if (_mask.empty() || _mask[0].empty())
        return;

    int height = _mask.size();
    int width = _mask[0].size();

    struct VerticalRun {
        int y;
        int height;
    };

    struct ActiveRect {
        sf::IntRect rect;
        bool continued = false;
    };

    std::vector<ActiveRect> active;

    for (int x = 0; x < width; ++x) {

        std::vector<VerticalRun> runs;

        int y = 0;

        while (y < height) {

            if (_mask[y][x] == -1) {
                ++y;
                continue;
            }
			
            int startY = y;

            while (y < height && _mask[y][x] != -1)
                ++y;

            runs.push_back({startY, y - startY});
        }

        for (auto& a : active)
            a.continued = false;

        for (auto& run : runs) {

            bool found = false;

            for (auto& a : active) {

                if (a.rect.position.y == run.y && a.rect.size.y == run.height && a.rect.position.x + a.rect.size.x == x) {
                    ++a.rect.size.x;

                    a.continued = true;
                    found = true;

                    break;
                }
            }

            if (!found) {
                ActiveRect a;
                a.rect.position = { x, run.y };
                a.rect.size = { 1, run.height };
                a.continued = true;

                active.push_back(a);
            }
        }

        for (auto it = active.begin(); it != active.end();) {
            if (!it->continued) {
                _rects.push_back(it->rect);
                it = active.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    for (auto& a : active)
        _rects.push_back(a.rect);
}

void Roof2::generateShape(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale)
{
	float tileSize = 32.f * scale;

	//sf::Color roofColor(127, 15, 15);
	//sf::Color frontColor(95, 15, 15);

	sf::Color roofColor = sf::Color::Transparent;
	sf::Color frontColor = sf::Color::Transparent;


	_topTriangle.resize(_rects.size());
	_bottomTriangle.resize(_rects.size());
	_rect.resize(_rects.size());

	int i = 0;

	for (auto& rect : _rects) {

		float roofWidth = rect.size.x * tileSize;

		// długość połaci w osi Y
		float roofDepth = (rect.size.y - 1) * tileSize;

		float roofHeight = (roofWidth / 2.f/* + 20.f*/);
		float centerX = roofWidth / 2.f;

		sf::Vector2f rectPosition;
		rectPosition.x = (float)rect.position.x * tileSize;
		float bottomBaseY = rect.position.y * tileSize + getRoofHeight(scale) + roofDepth;
		rectPosition.y = bottomBaseY - roofHeight - roofDepth;

		_topTriangle[i].setPointCount(3);
		_topTriangle[i].setPoint(0, rectPosition + sf::Vector2f(0.f, roofHeight));
		_topTriangle[i].setPoint(1, rectPosition + sf::Vector2f(roofWidth, roofHeight));
		_topTriangle[i].setPoint(2, rectPosition + sf::Vector2f(centerX, 0.f));
		_topTriangle[i].setFillColor(roofColor);

		_bottomTriangle[i].setPointCount(3);
		_bottomTriangle[i].setPoint(0, rectPosition + sf::Vector2f(0.f, roofHeight + roofDepth));
		_bottomTriangle[i].setPoint(1, rectPosition + sf::Vector2f(roofWidth, roofHeight + roofDepth));
		_bottomTriangle[i].setPoint(2, rectPosition + sf::Vector2f(centerX, roofDepth));
		_bottomTriangle[i].setFillColor(frontColor);

		_rect[i].setPointCount(4);
		_rect[i].setPoint(0, _topTriangle[i].getPoint(0));
		_rect[i].setPoint(1, _topTriangle[i].getPoint(1));
		_rect[i].setPoint(2, _bottomTriangle[i].getPoint(1));
		_rect[i].setPoint(3, _bottomTriangle[i].getPoint(0));
		_rect[i].setFillColor(roofColor);
		
		i += 1;
	}
	
}

void Roof2::generateTexture(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale)
{
	float tileSize = 32.f * scale;

	float roofWidth = tiles[0].size() * tileSize;
	float roofDepth = (tiles.size() - 1) * tileSize;
	float roofHeight = getRoofHeight(scale);

	int width = roofWidth;
	int height = roofHeight + roofDepth;

	sf::RenderTexture rtex;
	rtex.resize(sf::Vector2u(width, height));
	rtex.clear(sf::Color::Transparent);

	sf::Sprite tileLeft(*textures_manager->getTexture(L"assets\\tex\\roof2.png")->_texture);
	tileLeft.setTextureRect(sf::IntRect(sf::Vector2i(0, 64 * _type), sf::Vector2i(32, 64)));

	sf::Sprite tileRight(*textures_manager->getTexture(L"assets\\tex\\roof2.png")->_texture);
	tileRight.setTextureRect(sf::IntRect(sf::Vector2i(32, 64 * _type), sf::Vector2i(32, 64)));

	sf::Sprite halfTileLeft(*textures_manager->getTexture(L"assets\\tex\\roof2.png")->_texture);
	halfTileLeft.setTextureRect(sf::IntRect(sf::Vector2i(64, 64 * _type), sf::Vector2i(32, 64)));

	sf::Sprite halfTileRight(*textures_manager->getTexture(L"assets\\tex\\roof2.png")->_texture);
	halfTileRight.setTextureRect(sf::IntRect(sf::Vector2i(96, 64 * _type), sf::Vector2i(32, 64)));

	sf::Sprite middleHalfTileLeft(*textures_manager->getTexture(L"assets\\tex\\roof2.png")->_texture);
	middleHalfTileLeft.setTextureRect(sf::IntRect(sf::Vector2i(128, 64 * _type), sf::Vector2i(32, 64)));

	sf::Sprite middleHalfTileRight(*textures_manager->getTexture(L"assets\\tex\\roof2.png")->_texture);
	middleHalfTileRight.setTextureRect(sf::IntRect(sf::Vector2i(160, 64 * _type), sf::Vector2i(32, 64)));

	for (size_t k = 0; k < _rects.size(); k++) {
		const auto& rectTiles = _rects[k];
		auto bounds = _rect[k].getLocalBounds();

		int cols = rectTiles.size.x / 2;
		int rows = rectTiles.size.y - 1;
		bool oddWidth = rectTiles.size.x % 2 != 0;

		for (int i = 0; i < rows; i++) {

			float offsetX = float(i % 2) * 16.f * scale;
			float offsetY = float(i % 2) * 16.f * scale;

			if (i % 2 == 1) {
				float boundaryY = bounds.position.y + float(i) * 32.f * scale - 16.f * scale;
				halfTileLeft.setPosition(sf::Vector2f(bounds.position.x - 16.f * scale, boundaryY));
				rtex.draw(halfTileLeft);
				halfTileRight.setPosition(sf::Vector2f(bounds.position.x + bounds.size.x - 16.f * scale, boundaryY));
				rtex.draw(halfTileRight);
			}

			int drawCols = cols - i % 2;

			if (oddWidth && i % 2 == 1)
				drawCols++;

			for (int j = 0; j < drawCols; j++) {

				tileLeft.setPosition(sf::Vector2f(
					bounds.position.x + float(j) * 32.f * scale + offsetX,
					bounds.position.y + float(i - j) * 32.f * scale - 32.f * scale - offsetY
				));

				rtex.draw(tileLeft);

				tileRight.setPosition(sf::Vector2f(
					bounds.position.x + bounds.size.x - float(j + 1) * 32.f * scale - offsetX,
					bounds.position.y + float(i - j) * 32.f * scale - 32.f * scale - offsetY
				));

				rtex.draw(tileRight);
			}

			if (!oddWidth && i % 2 == 1) {

				int jMid = cols - 1;

				middleHalfTileLeft.setPosition(sf::Vector2f(
					bounds.position.x + float(jMid) * 32.f * scale + offsetX,
					bounds.position.y + float(i - jMid) * 32.f * scale - 32.f * scale - offsetY
				));

				rtex.draw(middleHalfTileLeft);

				middleHalfTileRight.setPosition(sf::Vector2f(
					bounds.position.x + bounds.size.x - float(jMid + 1) * 32.f * scale - offsetX,
					bounds.position.y + float(i - jMid) * 32.f * scale - 32.f * scale - offsetY
				));

				rtex.draw(middleHalfTileRight);
			}

			if (oddWidth && i % 2 == 0) {

				int jMid = cols;
				float middleX = bounds.position.x + bounds.size.x / 2.f - 16.f * scale;
				float middleY = bounds.position.y + float(i - jMid) * 32.f * scale - 32.f * scale;

				middleHalfTileLeft.setPosition(sf::Vector2f(middleX, middleY));
				middleHalfTileRight.setPosition(sf::Vector2f(middleX, middleY));

				rtex.draw(middleHalfTileLeft);
				rtex.draw(middleHalfTileRight);
			}

		}
	}
	rtex.display();

	_texture = rtex.getTexture();
}

int Roof2::getRoofHeight(float scale)
{
	float tileSize = 32.f * scale;
	float overhangX = _roofOverhangSize.x * scale;

	float maxRoofHeight = 0.f;

	for (const auto& rect : _rects) {

		float roofWidth = rect.size.x * tileSize + 2.f * overhangX;
		float roofHeight = (roofWidth / 2.f/* + 20.f*/);
		maxRoofHeight = std::max(maxRoofHeight, roofHeight);
	}

	return maxRoofHeight;
}

int Roof2::getTopOffset(float scale)
{
	float tileSize = 32.f * scale;
	float roofHeight = getRoofHeight(scale);
	float topOffset =
		(float)(_wallHeight) * tileSize +
		_roofOverhangSize.y * scale +
		roofHeight -
		tileSize;

	return topOffset;
}

void Roof2::generate(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale) {
    Roof::generate(tiles, position, scale);

	generateMask(tiles);
	generateRects();

    generateOverhang(tiles, position, scale);
    generateShape(tiles, position, scale);
    generateTexture(tiles, position, scale);
}

void Roof2::draw(sf::RenderTarget& target, sf::Vector2i position, float scale) {
	
	float wallsHeight = float(_wallHeight) * 32.f * scale;

	float roofHeight = getRoofHeight(scale);

	sf::Sprite sprite(_texture);
	sprite.setPosition(sf::Vector2f(position.x, position.y - wallsHeight - roofHeight + 32.f * scale));
	target.draw(sprite);
}