#pragma once
#include "Objects/Building/Building.hpp"
#include "Objects/Building/Roof1.hpp"
#include "Objects/Building/Roof2.hpp"
#include "Wallset.hpp"
#include "EditorsManager.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/BuildingPanel.hpp"
#include <queue>
#include "DebugLog.hpp"

std::shared_ptr<Texture> BuildingPrefab::_floorset = nullptr;	

BuildingPrefab::BuildingPrefab(std::wstring name, sf::Vector2i size) : GameObject(name) {

	if(_floorset == nullptr)
		_floorset = textures_manager->getTexture(L"assets\\tex\\floorset.png");

	_type = ObjectType::Building;

	// create floor
	_floor.clear();;
	for (int y = 0; y < size.y * 2; y += 1) {
		_floor.push_back(std::vector<int>(size.x*2, 1));
	}

	// create empty walls
	_walls.clear();
	for (int y = 0; y < size.y; y += 1) {
		_walls.push_back(std::vector<int>(size.x, -1));
	}

	// create outline walls
	for (int y = 0; y < size.y; ++y) {
		for (int x = 0; x < size.x; ++x) {
			if (x == 0 || y == 0 || x == _walls[0].size() - 1 || y == _walls.size() - 1) {
				if (!(y == _walls.size() - 1 && x == _walls[0].size() / 2 || y == _walls.size() - 1 && x == _walls[0].size() / 2 - 1)) {
					_walls[y][x] = 1;
				}
			}
		}
	}

	_collider = std::make_shared<RectangularCollider>(0, 0, _floor[0].size(), _floor.size());
}

BuildingPrefab::BuildingPrefab(std::wstring name, const BuildingPrefab& other) : GameObject(name) {
	
	_type = ObjectType::Building;

	// copy the floor
	_floor.clear();;
	for (int y = 0; y < other._floor.size(); y += 1) {
		_floor.push_back(std::vector<int>());
		for (int x = 0; x < other._floor[0].size(); x += 1) {
			_floor[y].push_back(other._floor[y][x]);
		}
	}

	// copy the walls
	_walls.clear();;
	for (int y = 0; y < other._walls.size(); y += 1) {
		_walls.push_back(std::vector<int>(other._walls[0].size(), -1));
		for (int x = 0; x < other._walls[0].size(); x += 1) {
			_walls[y][x] = other._walls[y][x];
		}
	}

	_wallHeight = other._wallHeight;

	if (std::dynamic_pointer_cast<Roof1>(other._roof)) {
		_roof = std::make_shared<Roof1>(other._roof->_type, other._roof->_wallHeight);
	}
	else if (std::dynamic_pointer_cast<Roof2>(other._roof)) {
		_roof = std::make_shared<Roof2>(other._roof->_type, other._roof->_wallHeight);
	}

}

BuildingPrefab::~BuildingPrefab() {

}

void BuildingPrefab::generate(sf::Vector2i position, float scale, std::shared_ptr<Building> building) {
	generateFloorVertexArray(scale);

	generateWalls(position, scale, building);

	if (building) {
		building->_wallsObjects.clear();
		for (auto& wall : _wallsObjects) {
			if (wall) {
				std::shared_ptr<Wall> newWall = std::make_shared<Wall>(wall->_prefab, wall->_building, wall->_textureBottomRect, wall->_textureTopRect, wall->_height);
				newWall->setPosition(sf::Vector2i(wall->_position.x, wall->_position.y));
				building->_wallsObjects.push_back(newWall);
			}
			else
				building->_wallsObjects.push_back(nullptr);
		}
	}

	generateSkelet(position, scale, building);

	generateRoofs(position, scale);
	generateCollider(scale);
	generateMesh(scale);
	generatePreviewTextures();
}

void BuildingPrefab::generateFloorVertexArray(float scale) {
	_floorVertexArray.clear();
	_floorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

	float s = 16.f;
	float floorSize = 64.f;
	float a = s * scale;

	for (int y = 0; y < _floor.size(); y++) {
		for (int x = 0; x < _floor[0].size(); x++) {
			int t = _floor[y][x];

			float px = x * s * scale;
			float py = y * s * scale;

			float tx = t * floorSize + (x % 4) * s;
			float ty = (y % 4) * s;

			if (tx < 0) tx = 0;
			if (ty < 0) ty = 0;

			_floorVertexArray.append(sf::Vertex(sf::Vector2f(px, py), sf::Color::White, sf::Vector2f(tx, ty)));
			_floorVertexArray.append(sf::Vertex(sf::Vector2f(px + a, py), sf::Color::White, sf::Vector2f(tx + s, ty)));
			_floorVertexArray.append(sf::Vertex(sf::Vector2f(px + a, py + a), sf::Color::White, sf::Vector2f(tx + s, ty + s)));

			_floorVertexArray.append(sf::Vertex(sf::Vector2f(px, py), sf::Color::White, sf::Vector2f(tx, ty)));
			_floorVertexArray.append(sf::Vertex(sf::Vector2f(px + a, py + a), sf::Color::White, sf::Vector2f(tx + s, ty + s)));
			_floorVertexArray.append(sf::Vertex(sf::Vector2f(px, py + a), sf::Color::White, sf::Vector2f(tx, ty + s)));
		}
	}
}

void BuildingPrefab::generateWalls(sf::Vector2i position, float scale, std::shared_ptr<Building> building) {

	_wallsObjects.clear();


	sf::Vector2i size = sf::Vector2i(_walls[0].size(), _walls.size());
	std::vector<std::vector<int>>& walls = _walls;

	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {

			int id = walls[y][x];
			int left = (x > 0) ? walls[y][x - 1] : -1;
			int right = (x < size.x - 1) ? walls[y][x + 1] : -1;
			int top = (y - 1 >= 0) ? walls[y - 1][x] : -1;
			int bottom = (y + 1 < size.y) ? walls[y + 1][x] : -1;

			if (id > -1) {

				// BOTTOM TEXTURE RECT
				sf::IntRect textureBottomRect(wallset->_groups[id]->walls[49].get(), sf::Vector2i(32, 32));

				if (left == -1 && right == -1)
					textureBottomRect.position = wallset->_groups[id]->walls[47].get();
				else if (left == -1)
					textureBottomRect.position = wallset->_groups[id]->walls[48].get();
				else if (right == -1)
					textureBottomRect.position = wallset->_groups[id]->walls[50].get();
				// TOP TEXTURE RECT

				sf::IntRect textureTopRect(textureBottomRect.position, sf::Vector2i(32, 32));

				int left = (x > 0) ? walls[y][x - 1] : -1;
				int right = (x < size.x - 1) ? walls[y][x + 1] : -1;
				int top = (y - 1 >= 0) ? walls[y - 1][x] : -1;
				int bottom = (y + 1 < size.y) ? walls[y + 1][x] : -1;

				int topLeft = (x > 0 && y - 1 >= 0) ? walls[y - 1][x - 1] : -1;
				int topRight = (x < size.x - 1 && y - 1 >= 0) ? walls[y - 1][x + 1] : -1;
				int bottomLeft = (x > 0 && y + 1 < size.y) ? walls[y + 1][x - 1] : -1;
				int bottomRight = (x < size.x - 1 && y + 1 < size.y) ? walls[y + 1][x + 1] : -1;

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

				textureTopRect.position = wallset->_groups[id]->walls[i].get();

				_wallsObjects.push_back(std::make_shared<Wall>(wallset->getPrefab(id), building, textureBottomRect, textureTopRect, _wallHeight));
				_wallsObjects.back()->setPosition(position + sf::Vector2i((float)x * 32.f * scale, (float)y * 32.f * scale));
			}
			else
				_wallsObjects.push_back(nullptr);
		}
	}
}

void BuildingPrefab::generateSkelet(sf::Vector2i position, float scale, std::shared_ptr<Building> building) {

	if (!building) return;

	std::vector<sf::IntRect> rects;

	std::shared_ptr<BuildingPrefab> prefab = std::dynamic_pointer_cast<BuildingPrefab>(building->_prefab.lock());
	int height = prefab->_walls.size();
	int width = prefab->_walls[0].size();

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

			if (prefab->_walls[y][x] == 0) {
				++y;
				continue;
			}

			int startY = y;

			while (y < height && prefab->_walls[y][x] != 0)
				++y;

			runs.push_back({ startY, y - startY });
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
				rects.push_back(it->rect);
				it = active.erase(it);
			}
			else {
				++it;
			}
		}
	}

	for (auto& a : active)
		rects.push_back(a.rect);

	_skeletObjects.clear();
	for(auto & rect : rects) {
		_skeletObjects.push_back(std::make_shared<Skelet>(skeletset->getSkelet(0), sf::IntRect(sf::Vector2i(rect.position.x * 32, rect.position.y * 32), sf::Vector2i(rect.size.x * 32, _wallHeight * 32))));
	}
}

void BuildingPrefab::generateRoofs(sf::Vector2i position, float scale) {

	if (!_roof)
		return;

	_roof->generate(_walls, position, scale);

}

void BuildingPrefab::generateCollider(float scale) {
	int border = 8;

	int x = -float(border) * scale;
	int y = -float(border) * scale;

	int w = (float)(_floor[0].size() * 16 + 2 * border) * scale;
	int h = (float)(_floor.size() * 16 + 2 * border) * scale;

	_collider = std::make_shared<RectangularCollider>(x, y, w, h);

}

void BuildingPrefab::generateMesh(float scale) {

	std::vector<std::vector<bool>> mask;


	// create mash using floor size
	for (auto& row : _floor)
		mask.push_back(std::vector<bool>(row.size(), false));

	// fill mask with floor
	for (int y = 0; y < _floor.size(); y += 1) {
		for (int x = 0; x < _floor[0].size(); x += 1) {
			if (_floor[y][x] > 0)
				mask[y][x] = true;
		}
	}

	// fill mask with walls
	for (int y = 0; y < _walls.size(); y += 1) {
		for (int x = 0; x < _walls[0].size(); x += 1) {
			if (_walls[y][x] > -1) {
				mask[y * 2][x * 2] = true;
				mask[y * 2][x * 2 + 1] = true;
				mask[y * 2 + 1][x * 2] = true;
				mask[y * 2 + 1][x * 2 + 1] = true;
			}
		}
	}

	/*
	// draw mask
	for(int y=0;y<mask.size();y+=1){
		for(int x=0;x<mask[0].size();x+=1){
			std::wcout << (mask[y][x] ? L"1" : L"0");
		}
		std::wcout << std::endl;
	}
	*/

	// divide mask into rectangles
	std::vector<sf::IntRect> rects;

	for (int y = 0; y < mask.size(); y += 1) {
		for (int x = 0; x < mask[y].size(); x += 1) {
			if (!mask[y][x])
				continue;

			int width = 0;
			while (x + width < mask[y].size() && mask[y][x + width]) {
				width += 1;
			}

			int height = 0;
			while (y + height < mask.size()) {

				bool fullRow = true;

				for (int xx = x; xx < x + width; xx++) {
					if (!mask[y + height][xx]) {
						fullRow = false;
						break;
					}
				}

				if (!fullRow)
					break;

				height += 1;
			}

			rects.push_back(sf::IntRect(sf::Vector2i(x, y), sf::Vector2i(width, height)));

			// unselect the rectangle in the mask
			for (int yy = y; yy < y + height; yy++) {
				for (int xx = x; xx < x + width; xx++) {
					mask[yy][xx] = false;
				}
			}
		}
	}

	/*
	// draw debug rectangles
	for(auto& rect : rects) {
		std::wcout << L"Rect: " << rect.position.x << L"," << rect.position.y << L" - " << rect.size.x << L"x" << rect.size.y << std::endl;
	}
	*/

	// Add rectangles to mesh
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(2.0f, 1.0f);

	for (auto& rect : rects) {

		std::shared_ptr<Shape> shape = std::make_shared<Shape>();
		shape->addPoint(sf::Vector2i(rect.position.x * 16, rect.position.y * 16), -1, scale);
		shape->addPoint(sf::Vector2i((rect.position.x + rect.size.x) * 16, rect.position.y * 16), -1, scale);
		shape->addPoint(sf::Vector2i((rect.position.x + rect.size.x) * 16, (rect.position.y + rect.size.y) * 16), -1, scale);
		shape->addPoint(sf::Vector2i(rect.position.x * 16, (rect.position.y + rect.size.y) * 16), -1, scale);
		mesh->addShape(shape);
	}

	_mesh = mesh;
}

void BuildingPrefab::drawOnlyCollider(sf::RenderTarget& target, sf::Vector2i position) {

	if (MapEditor::editor && MapEditor::editor->_main_menu->_render_colliders->_checkbox->_value == 1) {
		getCollider()->draw(position);
	}

}

void BuildingPrefab::drawOnlyFloor(sf::RenderTarget& target, sf::Vector2i position) {

	sf::RenderStates rs;
	rs.texture = _floorset->_texture.get();
	rs.transform.translate(sf::Vector2f(position));

	target.draw(_floorVertexArray, rs);
}

void BuildingPrefab::drawOnlyWalls(sf::RenderTarget& target, sf::Vector2i position, float scale, int drawType) {

	if (drawType == -1) {
		for (int y = 0; y < _walls.size(); y++) {
			for (int x = 0; x < _walls[0].size(); x++) {
				int index = y * _walls[0].size() + x;
				if (index < _wallsObjects.size()) {
					std::shared_ptr<Wall> wall = _wallsObjects[index];
					if (wall) {
						wall->setPosition(position + sf::Vector2i((float)x * 32.f * scale, (float)y * 32.f * scale));
						wall->draw(target, scale);
					}
				}
			}
		}
	}

	else {

		for (int y = 0; y < _walls.size(); y++) {
			for (int x = 0; x < _walls[0].size(); x++) {
				int index = y * _walls[0].size() + x;
				if (index < _wallsObjects.size()) {
					std::shared_ptr<Wall> wall = _wallsObjects[index];
					if (wall) {
						wall->setPosition(position + sf::Vector2i((float)x * 32.f * scale, (float)y * 32.f * scale));
						wall->draw(target, scale, drawType);
					}
				}
			}
		}
	}
}

void BuildingPrefab::drawOnlySkelet(sf::RenderTarget& target, sf::Vector2i position, float scale, int drawType) {
	for(auto& skelet : _skeletObjects) {
		if (skelet) {
			skelet->setPosition(position);
			skelet->draw(target, scale, drawType);
		}
	}
}

void BuildingPrefab::drawOnlyRoof(sf::RenderTarget& target, sf::Vector2i position, float scale, std::shared_ptr<Building> building) {

	if (!_roof)
		return;

	if (building->_renderOutsideLook) {
		_roof->draw(target, position, scale);
	}

	// draw the mesh
	// std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());
	// bp->getMesh()->draw(position, sf::Color::Red);

}

void BuildingPrefab::generatePreviewTexture(std::shared_ptr<sf::Texture>& texture, bool drawOutside) {

	const float scale = 1.0f;

	sf::Vector2i overhang = (_roof)? _roof->_roofOverhangSize : sf::Vector2i(0, 0);
	

	sf::FloatRect floorBounds = _floorVertexArray.getBounds();

	sf::Vector2i floorSize(
		(int)std::ceil(floorBounds.size.x),
		(int)std::ceil(floorBounds.size.y)
	);

	sf::RenderTexture resultTexture = sf::RenderTexture();

	if (auto flatRoof = std::dynamic_pointer_cast<Roof1>(_roof)) {

		int topOffset = _wallHeight * 32.0f;
		int width = floorSize.x + overhang.x * 2;
		int height = floorSize.y + topOffset + overhang.y;

		resultTexture.resize(sf::Vector2u(width, height));
		resultTexture.clear(sf::Color::Transparent);

		sf::Vector2i buildingPosition(overhang.x, topOffset + overhang.y);

		drawOnlyFloor(resultTexture, buildingPosition);

		if (!drawOutside) {
			drawOnlyWalls(resultTexture, buildingPosition, scale, 1);
			drawOnlySkelet(resultTexture, buildingPosition + sf::Vector2i(0, floorSize.y), scale, 1);
		}
		else {
			drawOnlyWalls(resultTexture, buildingPosition, scale, 2);
			drawOnlySkelet(resultTexture, buildingPosition + sf::Vector2i(0, floorSize.y), scale, 2);

			if (_roof) {
				_roof->draw(resultTexture, buildingPosition, scale);
			}
		}

		
	}

	if (auto gableRoof = std::dynamic_pointer_cast<Roof2>(_roof)) {

		int topOffset = gableRoof->getTopOffset(scale);
		int width = floorSize.x + gableRoof->_roofOverhangSize.x * 2.f;
		int height = floorSize.y + topOffset;

		resultTexture.resize(sf::Vector2u(width, height));
		resultTexture.clear(sf::Color::Transparent);

		sf::Vector2i buildingPosition(gableRoof->_roofOverhangSize.x, topOffset);

		drawOnlyFloor(resultTexture, buildingPosition);

		if (!drawOutside) {
			drawOnlyWalls(resultTexture, buildingPosition, scale, 1);
			drawOnlySkelet(resultTexture, buildingPosition + sf::Vector2i(0, floorSize.y), scale, 1);
		}
		else {
			drawOnlyWalls(resultTexture, buildingPosition, scale, 2);
			drawOnlySkelet(resultTexture, buildingPosition + sf::Vector2i(0, floorSize.y), scale, 2);
			
			if (_roof) {
				_roof->draw(resultTexture, buildingPosition, scale);
			}
		}
	}
	

	resultTexture.display();

	texture = std::make_shared<sf::Texture>(resultTexture.getTexture());
}

void BuildingPrefab::generatePreviewTextures() {
	generatePreviewTexture(_insideTexture, false);
	generatePreviewTexture(_outsideTexture, true);
}

std::shared_ptr<sf::Texture> BuildingPrefab::getPreviewInsideTexture() {
	return _insideTexture;
}

std::shared_ptr<sf::Texture> BuildingPrefab::getPreviewOutsideTexture() {
	return _outsideTexture;
}

Building::Building(std::weak_ptr<GameObject> prefab) : GameObjectOnMap(prefab) {
	_type = ObjectType::Building;

	std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(prefab.lock());

	_renderOutsideLook = true;
	
}

Building::~Building() {

}

void Building::generate(sf::Vector2i position) {

	std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());

	if (buildingPrefab) {
		buildingPrefab->generate(position, 1.0f, std::dynamic_pointer_cast<Building>(shared_from_this()));
	}
		

}

void Building::setPosition(sf::Vector2i position) {
	
	

	sf::Vector2i delta = position - getPosition();

	GameObjectOnMap::setPosition(position);

	std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());
	if (!buildingPrefab) return;

	float scale = 1.0f;
	if (BuildingsEditor::editor && BuildingsEditor::editor->_building_panel->_building->_building.get() == this) {
		scale = BuildingsEditor::editor->_building_panel->_building->_scale;
	}

	buildingPrefab->generateFloorVertexArray(scale);

	for(auto& wall : _wallsObjects) {
		if (wall)
			wall->setPosition(wall->getPosition() + delta);
	}

	buildingPrefab->generateRoofs(_position, scale);
	buildingPrefab->generateCollider(scale);
}

void Building::loadPrefab(std::shared_ptr<BuildingPrefab> buildingPrefab) {
	_prefab = buildingPrefab;
	setPosition(getPosition());
}

void Building::addWallsToGameObjects() {

	for (auto& wall : _wallsObjects) {

		if (!wall)
			continue;

		std::shared_ptr<Chunk> chunk = MapEditor::editor->_map->getChunkByGlobalPosition(_position);

		if (chunk) {
			chunk->addGameObjectOnMap(wall);

			MapEditor::editor->_game_objects->addGameObject(wall);
		}
	}
}

void Building::removeWallsFromGameObjects() {

	for (auto& wall : _wallsObjects) {

		if (!wall)
			continue;

		std::shared_ptr<Chunk> chunk = MapEditor::editor->_map->getChunkByGlobalPosition(_position);

		if (chunk)
			chunk->removeGameObjectOnMap(wall);

		MapEditor::editor->_game_objects->removeGameObject(wall);
	}
}

void Building::addWallsToVisibleGameObjects() {

	for (auto& wall : _wallsObjects) {

		if (!wall)
			continue;

		MapEditor::editor->_game_objects->_visibleGameObjectsOnMap.push_back(wall);
	}
}

void Building::cursorHover() {

	if (_prefab.expired())
		return;

	if (MapEditor::editor && Main::editor_manager->get_back() == MapEditor::editor) {
		
		if (!MapEditor::editor->_cursor_on_map->_object.expired())
			return;

		std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(_prefab.lock()->getMesh());

		if (!mesh) {
			// if mesh is not present, use collider instead
			std::shared_ptr<RectangularCollider> collider = std::dynamic_pointer_cast<RectangularCollider>(_prefab.lock()->getCollider());
			if (!collider) return;
			if (collider->cursorHover(MapEditor::editor->_cursor_on_map->_globalPosition, getPosition())) {
				MapEditor::editor->_game_objects->_hoveredGameObjectOnMap = shared_from_this();
			}
			return;
		}

		if(mesh->isPointInside(MapEditor::editor->_cursor_on_map->_globalPosition, getPosition())) {
			MapEditor::editor->_game_objects->_hoveredGameObjectOnMap = shared_from_this();
		}
	}

}

void Building::update() {
	

	if (MapEditor::editor && Main::editor_manager->get_back() == MapEditor::editor) {
		
		_renderOutsideLook = true;

		if (MapEditor::editor->_game_objects->_hoveredGameObjectOnMap.lock() == shared_from_this()) {
			_renderOutsideLook = false;
		}
	}

	if (BuildingsEditor::editor && Main::editor_manager->get_back() == BuildingsEditor::editor) {

		if (BuildingsEditor::editor->_main_menu->_render_walls_look->_checkbox->_value == 2) {
			_renderOutsideLook = true;
		}
		else {
			_renderOutsideLook = false;
		}
	}

	GameObjectOnMap::update();
}

void Building::draw() {
	if (MapEditor::editor) {
		if (_isSelected)
			drawFrame(sf::Color(255, 30, 45));
		else if (MapEditor::editor->_main_menu->_render_sprites_outline->_checkbox->_value == 1)
			drawFrame();
	}
}