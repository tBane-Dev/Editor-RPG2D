#include "Editors/BuildingsEditor/BuildingShape.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Cursor.hpp"
#include "DebugLog.hpp"

namespace BuildingsEditor {

	BuildingShape::BuildingShape() : ResizableShape() {
		_state = BuildingEditStates::Idle;
	}

	BuildingShape::~BuildingShape() {

	}

	void BuildingShape::create() {
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
	}

	void BuildingShape::createFloor() {
		_floor.clear();

		_floorSize.x = getSize().x / 16;
		_floorSize.y = getSize().y / 16;

		for (int y = 0; y < _floorSize.y; y++) {
			for (int x = 0; x < _floorSize.x; x++) {
				_floor.push_back(1);
			}
		}
	}

	void BuildingShape::resizeFloor(int offsetX, int offsetY) {
		int newWidth = getSize().x / 16;
		int newHeight = getSize().y / 16;

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

	void BuildingShape::generateFloorVertexArray() {
		_floorVertexArray.clear();
		_floorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

		float s = 16.f;
		float floorSize = 64.f;

		sf::Vector2f p(getPosition());

		for (int y = 0; y < _floorSize.y; ++y) {
			for (int x = 0; x < _floorSize.x; ++x) {
				int t = _floor[y * _floorSize.x + x];

				float px = p.x + x * s;
				float py = p.y + y * s;

				float tx = t * floorSize + (x % 4) * s;
				float ty = (y % 4) * s;

				if (tx < 0) tx = 0;
				if (ty < 0) ty = 0;

				_floorVertexArray.append(sf::Vertex(sf::Vector2f(px, py), sf::Color::White, sf::Vector2f(tx, ty)));
				_floorVertexArray.append(sf::Vertex(sf::Vector2f(px + s, py), sf::Color::White, sf::Vector2f(tx + s, ty)));
				_floorVertexArray.append(sf::Vertex(sf::Vector2f(px + s, py + s), sf::Color::White, sf::Vector2f(tx + s, ty + s)));

				_floorVertexArray.append(sf::Vertex(sf::Vector2f(px, py), sf::Color::White, sf::Vector2f(tx, ty)));
				_floorVertexArray.append(sf::Vertex(sf::Vector2f(px + s, py + s), sf::Color::White, sf::Vector2f(tx + s, ty + s)));
				_floorVertexArray.append(sf::Vertex(sf::Vector2f(px, py + s), sf::Color::White, sf::Vector2f(tx, ty + s)));
			}
		}
	}

	void BuildingShape::resize(std::shared_ptr<EdgePoint> edgePoint) {

		if (!edgePoint)
			return;

		sf::Vector2i oldPosition = getPosition();

		sf::Vector2i p = Main::cursor->_position - edgePoint->getPosition();
		p = p / _step * _step;

		int minX = _point_left->getPosition().x;
		int minY = _point_top->getPosition().y;
		int maxX = _point_right->getPosition().x;
		int maxY = _point_bottom->getPosition().y;

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

		if (resizeLeft)
			minX += p.x;
		else if (resizeRight)
			maxX += p.x;

		if (resizeTop)
			minY += p.y;
		else if (resizeBottom)
			maxY += p.y;

		if (resizeLeft) {
			int width = std::clamp(maxX - minX, _minSize.x, _maxSize.x);
			minX = maxX - width;
		}
		else if (resizeRight) {
			int width = std::clamp(maxX - minX, _minSize.x, _maxSize.x);
			maxX = minX + width;
		}

		if (resizeTop) {
			int height = std::clamp(maxY - minY, _minSize.y, _maxSize.y);
			minY = maxY - height;
		}
		else if (resizeBottom) {
			int height = std::clamp(maxY - minY, _minSize.y, _maxSize.y);
			maxY = minY + height;
		}

		_point_left_top->setPosition(sf::Vector2i(minX, minY));
		_point_top->setPosition(sf::Vector2i((minX + maxX) / 2, minY));
		_point_right_top->setPosition(sf::Vector2i(maxX, minY));

		_point_left->setPosition(sf::Vector2i(minX, (minY + maxY) / 2));
		_point_right->setPosition(sf::Vector2i(maxX, (minY + maxY) / 2));

		_point_left_bottom->setPosition(sf::Vector2i(minX, maxY));
		_point_bottom->setPosition(sf::Vector2i((minX + maxX) / 2, maxY));
		_point_right_bottom->setPosition(sf::Vector2i(maxX, maxY));

		_rect = sf::IntRect(sf::Vector2i(minX, minY), sf::Vector2i(maxX - minX, maxY - minY));

		int offsetX = 0;
		int offsetY = 0;

		if (resizeLeft)
			offsetX = (oldPosition.x - minX) / 16;

		if (resizeTop)
			offsetY = (oldPosition.y - minY) / 16;

		resizeFloor(offsetX, offsetY);
	}

	void BuildingShape::moveFloor(sf::Vector2i offset) {
		for (int i = 0; i < _floorVertexArray.getVertexCount(); i+=1) {
			_floorVertexArray[i].position += sf::Vector2f(offset);
		}
	}

	void BuildingShape::cursorHover() {
		ResizableShape::cursorHover();
	}

	void BuildingShape::handleEvent(const sf::Event& event) {
		for (auto& point : _edgePoints) {
			point->handleEvent(event);

			if (point == GUI_manager->Element_pressed) {
				_state = BuildingEditStates::Resizing;
				return;
			}
		}

		if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
			if (BuildingsEditor::editor->_palette->_categories->_selectedCategory->_type == ObjectType::Floor && BuildingsEditor::editor->_palette->_slots->_selectedSlotId > 0) {
				if (GUI_manager->Element_hovered.get() == this) {
					_state = BuildingEditStates::EditingFloor;
					GUI_manager->Element_pressed = shared_from_this();
					return;
				}
			}
		}

		if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Middle) {
			if (GUI_manager->Element_hovered.get() == this) {
				_state = BuildingEditStates::Moving;
				_offset = Main::cursor->_position - getPosition();
				GUI_manager->Element_pressed = nullptr;
			}
		}

		if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
			_state = BuildingEditStates::Idle;
			if (GUI_manager->Element_pressed.get() == this)
				GUI_manager->Element_pressed = nullptr;
		}

		if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Middle) {
			_state = BuildingEditStates::Idle;
			_offset = sf::Vector2i(0, 0);
			if (GUI_manager->Element_pressed.get() == this)
				GUI_manager->Element_pressed = nullptr;
		}
	}

	void BuildingShape::update() {
		if (_state == BuildingEditStates::Moving) {

			sf::Vector2i oldPosition = getPosition();
			sf::Vector2i newPosition = Main::cursor->_position - _offset;
			sf::Vector2i delta = newPosition - oldPosition;

			setPosition(newPosition);
			moveFloor(delta);

			return;
		}

		if (_state == BuildingEditStates::Resizing) {
			for (auto& point : _edgePoints) {
				if(point == GUI_manager->Element_pressed) {
					resize(point);
					return;
				}
			}

			return;
		}

		if(_state == BuildingEditStates::EditingFloor) {
			if (GUI_manager->Element_pressed.get() == this) {
				sf::Vector2i cursorPos = Main::cursor->_position;
				sf::Vector2i localPos = cursorPos - getPosition();

				int tileX = localPos.x / 16;
				int tileY = localPos.y / 16;

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
		}

		for (auto& point : _edgePoints) {
			point->update();
		}
	}

	void BuildingShape::drawOnlyShape() {
		ResizableShape::drawOnlyRect();
	}

	void BuildingShape::drawOnlyFloor() {
		Main::render_window->draw(_floorVertexArray, sf::RenderStates(_floorset->_texture.get()));
	}

	void BuildingShape::drawOnlyEdgePoints() {
		ResizableShape::drawOnlyEdgePoints();
	}

	void BuildingShape::draw() {
		drawOnlyShape();
		drawOnlyFloor();
		drawOnlyEdgePoints();

	}
}
