#include "Editors/BuildingsEditor/EditableBuilding.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Cursor.hpp"
#include "DebugLog.hpp"

namespace BuildingsEditor {

	EditableBuilding::EditableBuilding() : ResizableShape() {
		_state = EditableBuildingStates::Idle;
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
		int newWidth = getSize().x / int(16.f * _scale);
		int newHeight = getSize().y / int(16.f * _scale);

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

		for (int y = 0; y < _floorSize.y; ++y) {
			for (int x = 0; x < _floorSize.x; ++x) {
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

		sf::Vector2i screenDelta = Main::cursor->_position - edgePoint->getPosition();

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
		setPosition(_rect.position);
	}

	void EditableBuilding::moveFloor(sf::Vector2i offset) {
		for (int i = 0; i < _floorVertexArray.getVertexCount(); i+=1) {
			_floorVertexArray[i].position += sf::Vector2f(offset);
		}
	}

	void EditableBuilding::cursorHover() {
		ResizableShape::cursorHover();
	}

	void EditableBuilding::handleEvent(const sf::Event& event) {
		for (auto& point : _edgePoints) {
			point->handleEvent(event);

			if (point == GUI_manager->Element_pressed) {
				_state = EditableBuildingStates::Resizing;
				return;
			}
		}

		if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
			if (BuildingsEditor::editor->_palette->_categories->_selectedCategory->_type == ObjectType::Floor && BuildingsEditor::editor->_palette->_slots->_selectedSlotId >= 0) {
				if (GUI_manager->Element_hovered.get() == this) {
					_state = EditableBuildingStates::EditingFloor;
					GUI_manager->Element_pressed = shared_from_this();
					return;
				}
			}
		}

		if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Middle) {
			if (GUI_manager->Element_hovered.get() == this) {
				_state = EditableBuildingStates::Moving;
				_offset = Main::cursor->_position - getPosition();
				GUI_manager->Element_pressed = nullptr;
			}
		}

		if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
			_state = EditableBuildingStates::Idle;
			if (GUI_manager->Element_pressed.get() == this)
				GUI_manager->Element_pressed = nullptr;
		}

		if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Middle) {
			_state = EditableBuildingStates::Idle;
			_offset = sf::Vector2i(0, 0);
			if (GUI_manager->Element_pressed.get() == this)
				GUI_manager->Element_pressed = nullptr;
		}

		if (const auto* mws = event.getIf<sf::Event::MouseWheelScrolled>(); mws) {
			float oldScale = _scale;
			float newScale = std::clamp(_scale + mws->delta * 0.04f, 0.1f, 4.0f);
			sf::Vector2f cursorPosition(Main::cursor->_position);
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
			sf::Vector2i newPosition = Main::cursor->_position - _offset;
			sf::Vector2i delta = newPosition - oldPosition;

			setPosition(newPosition);
			moveFloor(delta);

			return;
		}

		if (_state == EditableBuildingStates::Resizing) {
			for (auto& point : _edgePoints) {
				if(point == GUI_manager->Element_pressed) {
					resize(point);
					return;
				}
			}

			return;
		}

		if(_state == EditableBuildingStates::EditingFloor) {
			if (GUI_manager->Element_pressed.get() == this) {
				sf::Vector2i cursorPos = Main::cursor->_position;
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

	void EditableBuilding::drawOnlyEdgePoints() {
		ResizableShape::drawOnlyEdgePoints();
	}

	void EditableBuilding::draw() {
		drawOnlyShape();
		drawOnlyFloor();
		drawOnlyEdgePoints();

	}
}
