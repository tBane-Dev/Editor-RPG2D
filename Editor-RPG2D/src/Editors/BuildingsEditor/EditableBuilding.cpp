#include "Editors/BuildingsEditor/EditableBuilding.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Cursor.hpp"
#include "DebugLog.hpp"
#include "Wallset.hpp"

namespace BuildingsEditor {

	EditableBuilding::EditableBuilding() : ResizableShape() {
		_state = EditableBuildingStates::Idle;
		_editState = EditableBuildingEditStates::None;
	}

	EditableBuilding::~EditableBuilding() {

	}

	void EditableBuilding::create() {
		sf::Vector2i buildingSize = sf::Vector2i(9 * 32, 9 * 32);

		ResizableShape::resize(buildingSize);
		setColor(sf::Color(79, 79, 79));
		setOutlineColor(sf::Color(47, 47, 47));
		sf::Vector2i rsize = BuildingsEditor::editor->_building_panel->getSize();
		sf::Vector2i rpos = BuildingsEditor::editor->_building_panel->getPosition();
		setPosition(sf::Vector2i(rpos.x + (rsize.x - getSize().x) / 2, rpos.y + (rsize.y - getSize().y) / 2));
		setStep(32);
		setMinSize(sf::Vector2i(7 * 32, 7 * 32));
		setMaxSize(sf::Vector2i(19 * 32, 19 * 32));

		_floorset = textures_manager->getTexture(L"assets\\tex\\floorset.png");

		createFloor();
		generateFloorVertexArray();

		createWalls();
		generateWalls();

	}

	void EditableBuilding::createFloor() {
		_floor.clear();

		_floorSize.x = getSize().x / 16;
		_floorSize.y = getSize().y / 16;

		for (int y = 0; y < _floorSize.y; y++) {
			for (int x = 0; x < _floorSize.x; x++) {
				_floor.push_back(1);
			}
		}
	}

	void EditableBuilding::resizeFloor(int offsetX, int offsetY) {
		int newWidth = _rect.size.x / 16;
		int newHeight = _rect.size.y / 16;

		std::vector<int> newFloor(newWidth * newHeight, 0);

		for (int y = 0; y < newHeight; ++y) {
			for (int x = 0; x < newWidth; ++x) {
				int oldX = x - offsetX;
				int oldY = y - offsetY;

				if (oldX >= 0 && oldX < _floorSize.x &&
					oldY >= 0 && oldY < _floorSize.y) {

					newFloor[y * newWidth + x] = _floor[oldY * _floorSize.x + oldX];
				}
			}
		}

		_floor = newFloor;
		_floorSize = sf::Vector2i(newWidth, newHeight);

		generateFloorVertexArray();
	}

	void EditableBuilding::generateFloorVertexArray() {
		_floorVertexArray.clear();
		_floorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

		float s = 16.f;
		float floorSize = 64.f;
		float a = s * _scale;
		sf::Vector2f p(getPosition());

		for (int y = 0; y < _floorSize.y; y++) {
			for (int x = 0; x < _floorSize.x; x++) {
				int t = _floor[y * _floorSize.x + x];

				float px = p.x + x * s * _scale;
				float py = p.y + y * s * _scale;

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

	void EditableBuilding::createWalls() {
		_wallsSize.x = getSize().x / 32;
		_wallsSize.y = getSize().y / 32;
		int index = 1;

		_walls.clear();

		for (int y = 0; y < _wallsSize.y; y++) {
			for (int x = 0; x < _wallsSize.x; x++) {
				int id = index;
				if (y != _wallsSize.y - 5 && y != _wallsSize.y - 4)
					id = -1;

				if(x==0 || y ==0 || x == _wallsSize.x - 1 || y == _wallsSize.y - 1)
					id = index;

				id = -1;

				_walls.push_back(id);
			}
		}
	}

	void EditableBuilding::resizeWalls(int offsetX, int offsetY) {
		int newWidth = _rect.size.x / 32;
		int newHeight = _rect.size.y / 32;

		std::vector<int> newWalls(newWidth * newHeight, -1);

		for (int y = 0; y < newHeight; y++) {
			for (int x = 0; x < newWidth; x++) {
				int oldX = x - offsetX;
				int oldY = y - offsetY;

				if (oldX >= 0 && oldX < _wallsSize.x &&
					oldY >= 0 && oldY < _wallsSize.y) {

					newWalls[y * newWidth + x] = _walls[oldY * _wallsSize.x + oldX];
				}
			}
		}

		_walls = newWalls;
		_wallsSize = sf::Vector2i(newWidth, newHeight);

		generateWalls();
	}

	void EditableBuilding::generateWalls() {

		_wallsObjects.clear();

		for (int y = 0; y < _wallsSize.y; y++) {
			for (int x = 0; x < _wallsSize.x; x++) {
				int id = _walls[y * _wallsSize.x + x];
				int left = (x > 0) ? _walls[y * _wallsSize.x + (x - 1)] : -1;
				int right = (x < _wallsSize.x - 1) ? _walls[y * _wallsSize.x + (x + 1)] : -1;
				int top = (y - 1 >= 0) ? _walls[(y - 1) * _wallsSize.x + x] : -1;
				int bottom = (y + 1 < _wallsSize.y) ? _walls[(y + 1) * _wallsSize.x + x] : -1;

				if (id > -1) {

					// BOTTOM TEXTURE RECT
					sf::IntRect textureBottomRect(wallset->_groups[id]->walls[49].get(), sf::Vector2i(32, 32));

					if(left == -1 && right == -1)
						textureBottomRect.position = wallset->_groups[id]->walls[47].get();
					else if (left == -1)
						textureBottomRect.position = wallset->_groups[id]->walls[48].get();
					else if (right == -1)
						textureBottomRect.position = wallset->_groups[id]->walls[50].get();
					// TOP TEXTURE RECT
					sf::IntRect textureTopRect(sf::Vector2i(32, id * 32), sf::Vector2i(32, 32));

					int left = (x > 0) ? _walls[y * _wallsSize.x + (x - 1)] : -1;
					int right = (x < _wallsSize.x - 1) ? _walls[y * _wallsSize.x + (x + 1)] : -1;
					int top = (y - 1 >= 0) ? _walls[(y - 1) * _wallsSize.x + x] : -1;
					int bottom = (y + 1 < _wallsSize.y) ? _walls[(y + 1) * _wallsSize.x + x] : -1;

					int topLeft = (x > 0 && y - 1 >= 0) ? _walls[(y - 1) * _wallsSize.x + (x - 1)] : -1;
					int topRight = (x < _wallsSize.x - 1 && y - 1 >= 0) ? _walls[(y - 1) * _wallsSize.x + (x + 1)] : -1;
					int bottomLeft = (x > 0 && y + 1 < _wallsSize.y) ? _walls[(y + 1) * _wallsSize.x + (x - 1)] : -1;
					int bottomRight = (x < _wallsSize.x - 1 && y + 1 < _wallsSize.y) ? _walls[(y + 1) * _wallsSize.x + (x + 1)] : -1;

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

	void EditableBuilding::resize(std::shared_ptr<EdgePoint> edgePoint) {

		if (!edgePoint || _scale <= 0.f)
			return;

		sf::Vector2i oldPosition = _rect.position;
		sf::Vector2i oldSize = _rect.size;

		bool resizeLeft =
			edgePoint == _point_left_top ||
			edgePoint == _point_left ||
			edgePoint == _point_left_bottom;

		bool resizeRight =
			edgePoint == _point_right_top ||
			edgePoint == _point_right ||
			edgePoint == _point_right_bottom;

		bool resizeTop =
			edgePoint == _point_left_top ||
			edgePoint == _point_top ||
			edgePoint == _point_right_top;

		bool resizeBottom =
			edgePoint == _point_left_bottom ||
			edgePoint == _point_bottom ||
			edgePoint == _point_right_bottom;

		sf::Vector2i screenDelta = Cursors::cursor->_position - edgePoint->getPosition();

		auto screenToLogicalStep = [this](int screenDelta) {
			int logicalDelta = int(std::round(float(screenDelta) / _scale));
			return logicalDelta / _step * _step;
		};

		sf::Vector2i logicalDelta(screenToLogicalStep(screenDelta.x), screenToLogicalStep(screenDelta.y));

		sf::Vector2i newPosition = oldPosition;
		sf::Vector2i newSize = oldSize;

		if (resizeLeft) {
			newSize.x = std::clamp(oldSize.x - logicalDelta.x, _minSize.x, _maxSize.x);
			int appliedLogicalDelta = oldSize.x - newSize.x;
			newPosition.x += int(std::round(appliedLogicalDelta * _scale));
		}
		else if (resizeRight) {
			newSize.x = std::clamp(oldSize.x + logicalDelta.x, _minSize.x, _maxSize.x);
		}

		if (resizeTop) {
			newSize.y = std::clamp(oldSize.y - logicalDelta.y, _minSize.y, _maxSize.y);
			int appliedLogicalDelta = oldSize.y - newSize.y;
			newPosition.y += int(std::round(appliedLogicalDelta * _scale));
		}
		else if (resizeBottom) {
			newSize.y = std::clamp(oldSize.y + logicalDelta.y, _minSize.y, _maxSize.y);
		}

		if (newPosition == oldPosition && newSize == oldSize)
			return;

		_rect.position = newPosition;
		_rect.size = newSize;

		int offsetX = 0;
		int offsetY = 0;

		if (resizeLeft)
			offsetX = (newSize.x - oldSize.x) / 16;

		if (resizeTop)
			offsetY = (newSize.y - oldSize.y) / 16;

		resizeFloor(offsetX, offsetY);
		resizeWalls(offsetX/2, offsetY/2);
		setPosition(_rect.position);
	}

	void EditableBuilding::moveFloor(sf::Vector2i offset) {
		for (int i = 0; i < _floorVertexArray.getVertexCount(); i+=1) {
			_floorVertexArray[i].position += sf::Vector2f(offset);
		}
	}

	void EditableBuilding::moveWalls(sf::Vector2i offset) {
		for (auto& wall : _wallsObjects) {
			if (wall) {
				wall->_position += offset;
			}
		}
	}

	void EditableBuilding::editTileUnderCursor() {
		sf::Vector2i cursorPos = Cursors::cursor->_position;
		sf::Vector2i localPos = cursorPos - sf::Vector2i(sf::Vector2f(getPosition()));

		int tileX = localPos.x / int(16.f * _scale);
		int tileY = localPos.y / int(16.f * _scale);

		if (tileX >= 0 && tileX < _floorSize.x &&
			tileY >= 0 && tileY < _floorSize.y) {

			std::shared_ptr<Slot> selectedSlot = BuildingsEditor::editor->_palette->_slots->_selectedSlot;

			if (!selectedSlot) return;

			std::shared_ptr<Floor> floor = std::dynamic_pointer_cast<Floor>(selectedSlot->_object.lock());

			if (!floor) return;

			int index = tileY * _floorSize.x + tileX;

			if (_floor[index] != floor->_id) {
				_floor[index] = floor->_id;
				generateFloorVertexArray();
			}
		}
	}

	void EditableBuilding::addWallUnderCursor() {
		sf::Vector2i cursorPos = Cursors::cursor->_position;
		sf::Vector2i localPos = cursorPos - sf::Vector2i(sf::Vector2f(getPosition()));

		int tileX = localPos.x / int(32.f * _scale);
		int tileY = localPos.y / int(32.f * _scale);

		if (tileX >= 0 && tileX < _wallsSize.x &&
			tileY >= 0 && tileY < _wallsSize.y) {

			std::shared_ptr<Slot> selectedSlot = BuildingsEditor::editor->_palette->_slots->_selectedSlot;
			if (!selectedSlot) return;

			std::shared_ptr<Wall> wall = std::dynamic_pointer_cast<Wall>(selectedSlot->_object.lock());
			if (!wall) return;

			std::shared_ptr<WallPrefab> wallPrefab = std::dynamic_pointer_cast<WallPrefab>(wall->_prefab.lock());
			if (!wallPrefab) return;

			int index = tileY * _wallsSize.x + tileX;
			if (_walls[index] != wallPrefab->_id) {
				_walls[index] = wallPrefab->_id;
				generateWalls();
			}
		}
	}

	void EditableBuilding::removeWallUnderCursor() {
		sf::Vector2i cursorPos = Cursors::cursor->_position;
		sf::Vector2i localPos = cursorPos - sf::Vector2i(sf::Vector2f(getPosition()));

		int tileX = localPos.x / int(32.f * _scale);
		int tileY = localPos.y / int(32.f * _scale);

		if (tileX >= 0 && tileX < _wallsSize.x &&
			tileY >= 0 && tileY < _wallsSize.y) {

			int index = tileY * _wallsSize.x + tileX;
			if (_walls[index] != -1) {
				_walls[index] = -1;
				generateWalls();
			}
		}
	}

	sf::Vector2i EditableBuilding::clampPosition(sf::Vector2i position) {
		int clampOffset = 256;
		int x = std::min(getSize().x / 2, clampOffset);
		int y = std::min(getSize().y / 2, clampOffset);

		sf::IntRect rect = BuildingsEditor::editor->_building_panel->_rect;

		sf::Vector2i p;
		p.x = std::clamp(position.x, rect.position.x - getSize().x + x, rect.position.x + rect.size.x - x);
		p.y = std::clamp(position.y, rect.position.y - getSize().y + y, rect.position.y + rect.size.y - y);
		return p;
	}

	void EditableBuilding::cursorHover() {
		if (BuildingsEditor::editor->_building_panel->_rect.contains(Cursors::cursor->_position)) {
			ResizableShape::cursorHover();
		}
		
	}

	void EditableBuilding::handleEvent(const sf::Event& event) {

		if (!(BuildingsEditor::editor->_building_panel->_building.get() == this && BuildingsEditor::editor->_building_panel->_rect.contains(Cursors::cursor->_position)) && GUI_manager->Element_pressed == nullptr)
			return;

		for (auto& point : _edgePoints) {
			point->handleEvent(event);

			if (point == GUI_manager->Element_pressed) {
				_state = EditableBuildingStates::Resizing;
				return;
			}
		}

		if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
			 
			if (GUI_manager->Element_hovered.get() == this) {
				GUI_manager->Element_pressed = shared_from_this();

				if (_editState == EditableBuildingEditStates::Floor) {
					editTileUnderCursor();
					return;
				}
				if (_editState == EditableBuildingEditStates::GameObject) {
					addWallUnderCursor();
					return;
				}
			}
		}

		if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Middle) {
			if (GUI_manager->Element_hovered.get() == this) {
				_state = EditableBuildingStates::Moving;
				_offset = Cursors::cursor->_position - getPosition();
				GUI_manager->Element_pressed = nullptr;
			}
		}

		if(const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Right) {
			GUI_manager->Element_pressed = shared_from_this();

			if(_editState == EditableBuildingEditStates::Floor) {
				BuildingsEditor::editor->_palette->_slots->selectSlot(-1);
				return;
			}

			if(_editState == EditableBuildingEditStates::GameObject) {
				BuildingsEditor::editor->_palette->_slots->selectSlot(-1);
				return;
			}

			if (_editState == EditableBuildingEditStates::None) {
				removeWallUnderCursor();
				return;
			}
		}

		if(const auto* mm = event.getIf<sf::Event::MouseMoved>(); mm) {
			if (GUI_manager->Element_pressed.get() == this) {
				if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					if (_editState == EditableBuildingEditStates::Floor) {
						editTileUnderCursor();
						return;
					}

					if (_editState == EditableBuildingEditStates::GameObject) {
						addWallUnderCursor();
						return;
					}
				}

				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
					if (_editState == EditableBuildingEditStates::None) {
						removeWallUnderCursor();
						return;	
					}
				}
			}
			
		}

		if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
			if (GUI_manager->Element_pressed.get() == this)
				GUI_manager->Element_pressed = nullptr;
		}

		if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Middle) {
			_state = EditableBuildingStates::Idle;
			_offset = sf::Vector2i(0, 0);
			if (GUI_manager->Element_pressed.get() == this)
				GUI_manager->Element_pressed = nullptr;
		}

		if(const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Right) {
			if (GUI_manager->Element_pressed.get() == this)
				GUI_manager->Element_pressed = nullptr;

			if (_editState == EditableBuildingEditStates::Floor) {
				_editState = EditableBuildingEditStates::None;
			}

			if (_editState == EditableBuildingEditStates::GameObject) {
				_editState = EditableBuildingEditStates::None;
			}

			return;
		}


		if (const auto* mws = event.getIf<sf::Event::MouseWheelScrolled>(); mws) {
			float oldScale = _scale;
			float newScale = std::clamp(_scale + mws->delta * 0.125f, 0.5f, 2.0f);
			sf::Vector2f cursorPosition(Cursors::cursor->_position);
			sf::Vector2f oldPosition(getPosition());
			float scaleFactor = newScale / oldScale;
			sf::Vector2f newPosition = cursorPosition + (oldPosition - cursorPosition) * scaleFactor;
			_scale = newScale;
			setPosition(sf::Vector2i(newPosition));
			generateFloorVertexArray();
			generateEdgePoints();
		}
	}

	void EditableBuilding::update() {
		if (_state == EditableBuildingStates::Moving) {

			sf::Vector2i oldPosition = getPosition();
			sf::Vector2i newPosition = Cursors::cursor->_position - _offset;
			
			if (BuildingsEditor::editor->_building_panel->_building.get() == this) {
				newPosition = clampPosition(newPosition);
			}

			sf::Vector2i delta = newPosition - oldPosition;

			setPosition(newPosition);
			moveFloor(delta);
			moveWalls(delta);
			return;
		}

		if (_state == EditableBuildingStates::Resizing) {
			for (auto& point : _edgePoints) {
				if(point == GUI_manager->Element_pressed) {
					resize(point);
					setPosition(clampPosition(getPosition()));
					return;
				}
			}

			return;
		}

		for (auto& point : _edgePoints) {
			point->update();
		}
	}

	void EditableBuilding::drawOnlyShape() {
		ResizableShape::drawOnlyRect();
	}

	void EditableBuilding::drawOnlyFloor() {
		Main::render_window->draw(_floorVertexArray, sf::RenderStates(_floorset->_texture.get()));
	}

	void EditableBuilding::drawOnlyWalls() {
		for (int y = 0; y < _wallsSize.y; y++) {
			for (int x = 0; x < _wallsSize.x; x++) {
				int index = y * _wallsSize.x + x;
				if (index < _wallsObjects.size()) {
					std::shared_ptr<Wall> wall = _wallsObjects[index];
					if (wall) {
						wall->setPosition(getPosition() + sf::Vector2i((float)x * 32.f * _scale, (float)y * 32.f * _scale));
						wall->draw(_scale);
					}
				}
			}
		}
	}

	void EditableBuilding::drawOnlyEdgePoints() {
		ResizableShape::drawOnlyEdgePoints();
	}

	void EditableBuilding::draw() {

		if (BuildingsEditor::editor->_building_panel->_building.get() == this) {
			sf::FloatRect fr;
			fr.size = sf::Vector2f(BuildingsEditor::editor->_building_panel->_rect.size);
			fr.position = sf::Vector2f(BuildingsEditor::editor->_building_panel->_rect.position);

			sf::View view(fr);

			sf::FloatRect vp(
				sf::Vector2f(
					fr.position.x / GUI_manager->_view.getSize().x,
					fr.position.y / GUI_manager->_view.getSize().y
				),

				sf::Vector2f(
					fr.size.x / GUI_manager->_view.getSize().x,
					fr.size.y / GUI_manager->_view.getSize().y
				)
			);

			view.setViewport(vp);
			Main::render_window->setView(view);
		}
		

		drawOnlyShape();
		drawOnlyFloor();
		drawOnlyWalls();
		drawOnlyEdgePoints();

		

	}
}
