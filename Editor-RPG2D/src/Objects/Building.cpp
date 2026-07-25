#pragma once
#include "Objects/Building.hpp"
#include "Wallset.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/BuildingPanel.hpp"

std::shared_ptr<Texture> BuildingPrefab::_floorset = nullptr;	

BuildingPrefab::BuildingPrefab(std::wstring name, sf::Vector2i size) : GameObject(name) {

	if(_floorset == nullptr)
		_floorset = textures_manager->getTexture(L"assets\\tex\\floorset.png");

	_type = ObjectType::Building;

	// create floor
	_floor.clear();;
	_floorSize = sf::Vector2i(size.x*2, size.y*2);
	for (int y = 0; y < _floorSize.y; y += 1) {
		for (int x = 0; x < _floorSize.x; x += 1) {
			_floor.push_back(1);
		}
	}

	// create empty walls
	_walls.clear();;
	_wallsSize = size;
	for (int y = 0; y < _wallsSize.y; y += 1) {
		for (int x = 0; x < _wallsSize.x; x += 1) {
			_walls.push_back(-1);
		}
	}

	// create outline walls
	for (int y = 0; y < _wallsSize.y; ++y) {
		for (int x = 0; x < _wallsSize.x; ++x) {
			if (x == 0 || y == 0 || x == _wallsSize.x - 1 || y == _wallsSize.y - 1) {
				if (!(y == _wallsSize.y - 1 && x == _wallsSize.x / 2)) {
					_walls[y * _wallsSize.x + x] = 1;
				}
			}
		}
	}
}

BuildingPrefab::BuildingPrefab(std::wstring name, const BuildingPrefab& other) : GameObject(name) {
	
	_name = name;
	_type = ObjectType::Building;

	// copy the floor
	_floor.clear();;
	_floorSize = other._floorSize;
	for (int y = 0; y < _floorSize.y; y += 1) {
		for (int x = 0; x < _floorSize.x; x += 1) {
			_floor.push_back(other._floor[y * _floorSize.x + x]);
		}
	}

	// copy the walls
	_walls.clear();;
	_wallsSize = other._wallsSize;
	for (int y = 0; y < _wallsSize.y; y += 1) {
		for (int x = 0; x < _wallsSize.x; x += 1) {
			_walls.push_back(other._walls[y*_wallsSize.x + x]);
		}
	}

}

BuildingPrefab::~BuildingPrefab() {

}

Building::Building(std::weak_ptr<GameObject> prefab) : GameObjectOnMap(prefab) {
	_type = ObjectType::Building;
}

Building::~Building() {

}

void Building::setPosition(sf::Vector2i position) {
	GameObjectOnMap::setPosition(position);

	float scale = 1.0f;
	if (BuildingsEditor::editor->_building_panel->_building->_building.get() == this) {
		scale = BuildingsEditor::editor->_building_panel->_building->_scale;
	}

	generateFloorVertexArray(scale);
	generateWalls(scale);;
}

void Building::generateFloorVertexArray(float scale) {
	_floorVertexArray.clear();
	_floorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

	std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());
	if (!buildingPrefab) return;


	float s = 16.f;
	float floorSize = 64.f;
	float a = s * scale;
	sf::Vector2f p(getPosition());
	
	for (int y = 0; y < buildingPrefab->_floorSize.y; y++) {
		for (int x = 0; x < buildingPrefab->_floorSize.x; x++) {
			int t = buildingPrefab->_floor[y * buildingPrefab->_floorSize.x + x];

			float px = p.x + x * s * scale;
			float py = p.y + y * s * scale;

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

void Building::generateWalls(float scale) {

	_wallsObjects.clear();

	std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());
	if (!buildingPrefab) return;

	sf::Vector2i size = buildingPrefab->_wallsSize;
	std::vector<int>& walls = buildingPrefab->_walls;
	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {

			int id = walls[y * size.x + x];
			int left = (x > 0) ? walls[y * size.x + (x - 1)] : -1;
			int right = (x < size.x - 1) ? walls[y * size.x + (x + 1)] : -1;
			int top = (y - 1 >= 0) ? walls[(y - 1) * size.x + x] : -1;
			int bottom = (y + 1 < size.y) ? walls[(y + 1) * size.x + x] : -1;

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
				sf::IntRect textureTopRect(sf::Vector2i(32, id * 32), sf::Vector2i(32, 32));

				int left = (x > 0) ? walls[y * size.x + (x - 1)] : -1;
				int right = (x < size.x - 1) ? walls[y * size.x + (x + 1)] : -1;
				int top = (y - 1 >= 0) ? walls[(y - 1) * size.x + x] : -1;
				int bottom = (y + 1 < size.y) ? walls[(y + 1) * size.x + x] : -1;

				int topLeft = (x > 0 && y - 1 >= 0) ? walls[(y - 1) * size.x + (x - 1)] : -1;
				int topRight = (x < size.x - 1 && y - 1 >= 0) ? walls[(y - 1) * size.x + (x + 1)] : -1;
				int bottomLeft = (x > 0 && y + 1 < size.y) ? walls[(y + 1) * size.x + (x - 1)] : -1;
				int bottomRight = (x < size.x - 1 && y + 1 < size.y) ? walls[(y + 1) * size.x + (x + 1)] : -1;

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

				// CROSS with three edges

				// CROSS with two edges

				// CROSS with one edge

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
				_wallsObjects.push_back(std::make_shared<Wall>(wallset->getPrefab(id), textureBottomRect, textureTopRect));
			}
			else
				_wallsObjects.push_back(nullptr);
		}
	}
}

void Building::loadPrefab(std::shared_ptr<BuildingPrefab> buildingPrefab) {
	_prefab = buildingPrefab;
	setPosition(getPosition());
}