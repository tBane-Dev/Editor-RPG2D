#pragma once
#include "Objects/Building/Building.hpp"
#include "Wallset.hpp"
#include "Roofset.hpp"
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
	
	_name = name;
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

}

BuildingPrefab::~BuildingPrefab() {

}

Building::Building(std::weak_ptr<GameObject> prefab) : GameObjectOnMap(prefab) {
	_type = ObjectType::Building;

	std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(prefab.lock());

	_renderOutsideLook = true;
	
}

Building::~Building() {

}

void Building::generate() {
	generateFloorVertexArray();
	generateWalls();
	generateRoofs();
	generateCollider();
	generateMesh();
}

void Building::setPosition(sf::Vector2i position) {
	
	sf::Vector2i delta = position - getPosition();

	GameObjectOnMap::setPosition(position);

	float scale = 1.0f;
	if (BuildingsEditor::editor && BuildingsEditor::editor->_building_panel->_building->_building.get() == this) {
		scale = BuildingsEditor::editor->_building_panel->_building->_scale;
	}

	generateFloorVertexArray(scale);

	for(auto& wall : _wallsObjects) {
		if (wall)
			wall->setPosition(wall->getPosition() + delta);
	}

	generateRoofs(scale);
	generateCollider(scale);
}

void Building::generateFloorVertexArray(float scale) {
	_floorVertexArray.clear();
	_floorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

	std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());
	if (!buildingPrefab) return;


	float s = 16.f;
	float floorSize = 64.f;
	float a = s * scale;
	
	for (int y = 0; y < buildingPrefab->_floor.size(); y++) {
		for (int x = 0; x < buildingPrefab->_floor[0].size(); x++) {
			int t = buildingPrefab->_floor[y][x];

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

void Building::generateWalls(float scale, bool renderOutsideLook) {

	_wallsObjects.clear();

	std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());
	if (!buildingPrefab) return;

	sf::Vector2i size = sf::Vector2i(buildingPrefab->_walls[0].size(), buildingPrefab->_walls.size());
	std::vector<std::vector<int>>& walls = buildingPrefab->_walls;

	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {

			int id = walls[y][x];
			int left = (x > 0) ? walls[y][x-1] : -1;
			int right = (x < size.x - 1) ? walls[y][x+1] : -1;
			int top = (y - 1 >= 0) ? walls[y-1][x] : -1;
			int bottom = (y + 1 < size.y) ? walls[y+1][x] : -1;

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
					
				int left = (x > 0) ? walls[y][x-1] : -1;
				int right = (x < size.x - 1) ? walls[y][x+1] : -1;
				int top = (y - 1 >= 0) ? walls[y-1][x] : -1;
				int bottom = (y + 1 < size.y) ? walls[y+1][x] : -1;

				int topLeft = (x > 0 && y - 1 >= 0) ? walls[y-1][x-1] : -1;
				int topRight = (x < size.x - 1 && y - 1 >= 0) ? walls[y-1][x+1] : -1;
				int bottomLeft = (x > 0 && y + 1 < size.y) ? walls[y+1][x-1] : -1;
				int bottomRight = (x < size.x - 1 && y + 1 < size.y) ? walls[y+1][x+1] : -1;

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
								
				_wallsObjects.push_back(std::make_shared<Wall>(wallset->getPrefab(id), std::dynamic_pointer_cast<Building>(shared_from_this()), textureBottomRect, textureTopRect, 3));
				_wallsObjects.back()->setPosition(getPosition() + sf::Vector2i((float)x * 32.f * scale, (float)y * 32.f * scale));
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

void Building::generateRoofs(float scale) {

	std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());
	
	if (!bp)
		return;

	_roof = std::make_shared<FlatRoof>();
	_roof->generate(bp->_walls, _position, scale);
}

void Building::generateCollider(float scale) {
	std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());

	if (!bp)
		return;

	int border = 8;

	int x = -float(border) * scale;
	int y = -float(border) * scale;

	int w = (float)(bp->_floor[0].size() * 16 + 2 * border) * scale;
	int h = (float)(bp->_floor.size() * 16 + 2 * border) * scale;

	_prefab.lock()->_collider = std::make_shared<RectangularCollider>(x, y, w, h);

}

void Building::generateMesh(float scale) {

	std::vector<std::vector<bool>> mask;

	std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());

	if (!bp) return;

	// create mash using floor size
	for (auto& row : bp->_floor)
		mask.push_back(std::vector<bool>(row.size(), false));

	// fill mask with floor
	for (int y = 0; y < bp->_floor.size(); y+=1) {
		for(int x = 0; x < bp->_floor[0].size(); x+=1) {
			if (bp->_floor[y][x] > 0)
				mask[y][x] = true;
		}
	}

	// fill mask with walls
	for (int y = 0; y < bp->_walls.size(); y += 1) {
		for (int x = 0; x < bp->_walls[0].size(); x += 1) {
			if (bp->_walls[y][x] > -1) {
				mask[y*2][x*2] = true;
				mask[y*2][x*2+1] = true;
				mask[y*2+1][x*2] = true;
				mask[y*2+1][x*2+1] = true;
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
			while( y + height < mask.size()) {

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

	for(auto& rect : rects) {

		std::shared_ptr<Shape> shape = std::make_shared<Shape>();
		shape->addPoint(sf::Vector2i(rect.position.x * 16, rect.position.y * 16), -1, scale);
		shape->addPoint(sf::Vector2i((rect.position.x + rect.size.x) * 16, rect.position.y * 16), -1, scale);
		shape->addPoint(sf::Vector2i((rect.position.x + rect.size.x) * 16, (rect.position.y + rect.size.y) * 16), -1, scale);
		shape->addPoint(sf::Vector2i(rect.position.x * 16, (rect.position.y + rect.size.y) * 16), -1, scale);
		mesh->addShape(shape);
	}
	
	bp->_mesh = mesh;
}

void Building::addWallsToGameObjects() {

	for (auto& wall : _wallsObjects) {
		if (wall != nullptr) { // because some walls can be nullptr if they are not present in the prefab
			MapEditor::editor->_game_objects->addGameObject(wall);
		}
	}
}

void Building::removeWallsFromGameObjects() {
	for (auto& wall : _wallsObjects) {
		if(wall != nullptr) { // because some walls can be nullptr if they are not present in the prefab
			MapEditor::editor->_game_objects->removeGameObject(wall);
		}
	}
}

void Building::drawOnlyCollider() {
	if (_prefab.expired()) return;
	
	std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());
	
	if (!bp) return;

	if (MapEditor::editor && MapEditor::editor->_main_menu->_render_colliders->_checkbox->_value == 1) {
		_prefab.lock()->getCollider()->draw(getPosition());
	}

}

void Building::drawOnlyFloor(sf::RenderTarget& target, sf::Vector2i position) {

	std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());
	if (!bp) return;

	sf::RenderStates rs;
	rs.texture = bp->_floorset->_texture.get();
	rs.transform.translate(sf::Vector2f(position));

	target.draw(_floorVertexArray, rs);
}

void Building::drawOnlyWalls(sf::RenderTarget& target, sf::Vector2i position, float scale) {

	std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());
	if (!bp) return;

	for (int y = 0; y < bp->_walls.size(); y++) {
		for (int x = 0; x < bp->_walls[0].size(); x++) {
			int index = y * bp->_walls[0].size() + x;
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

void Building::drawOnlyRoof(sf::RenderTarget& target, sf::Vector2i position, float scale) {

	if (!_roof)
		return;

	if (_renderOutsideLook) {
		_roof->draw(target, position, scale);
	}
	
	// draw the mesh
	// std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(_prefab.lock());
	// bp->getMesh()->draw(position, sf::Color::Red);

	if(MapEditor::editor) {
		if(_isSelected)
			drawFrame(sf::Color(255, 30, 45));
		else if(MapEditor::editor->_main_menu->_render_sprites_outline->_checkbox->_value == 1)
			drawFrame();
	}
}

void Building::cursorHover() {

	if (_prefab.expired())
		return;

	if (MapEditor::editor && Main::editor_manager->get_back() == MapEditor::editor) {
		
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
	// NOTHING
}