#include "Editors/BuildingsEditor/EditableBuilding.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Cursor.hpp"
#include "DebugLog.hpp"
#include "Wallset.hpp"
#include "Roofset.hpp"

namespace BuildingsEditor {

	EditableBuilding::EditableBuilding() {
		_state = EditableBuildingStates::Idle;
		_building = nullptr;
	}

	EditableBuilding::~EditableBuilding() {

	}

	void EditableBuilding::create(std::shared_ptr<BuildingPrefab> prefab) {
		if (!prefab) return;

		_building = std::make_shared<Building>(prefab);
		_building->generate();

		sf::Vector2i buildingSize = sf::Vector2i(prefab->_wallsSize.x * 32,prefab->_wallsSize.y * 32);

		ResizableShape::resize(buildingSize);

		setColor(sf::Color(79, 79, 79));
		setOutlineColor(sf::Color(47, 47, 47));

		setStep(32);
		setMinSize(sf::Vector2i(8 * 32, 8 * 32));
		setMaxSize(sf::Vector2i(24 * 32, 24 * 32));

		sf::Vector2i panelSize = BuildingsEditor::editor->_building_panel->getSize();
		sf::Vector2i panelPosition = BuildingsEditor::editor->_building_panel->getPosition();

		setPosition(sf::Vector2i(
			panelPosition.x + (panelSize.x - getSize().x) / 2,
			panelPosition.y + (panelSize.y - getSize().y) / 2
		));

		// setPosition generuje podłogę i obiekty ścian.
		_building->setPosition(getPosition());
	}

	void EditableBuilding::resizeFloor(int offsetX, int offsetY) {

		std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(_building->_prefab.lock());
		if (!bp) return;

		int newWidth = _rect.size.x / 16;
		int newHeight = _rect.size.y / 16;

		std::vector<int> newFloor(newWidth * newHeight, 0);

		for (int y = 0; y < newHeight; ++y) {
			for (int x = 0; x < newWidth; ++x) {
				int oldX = x - offsetX;
				int oldY = y - offsetY;

				if (oldX >= 0 && oldX < bp->_floorSize.x &&
					oldY >= 0 && oldY < bp->_floorSize.y) {

					newFloor[y * newWidth + x] = bp->_floor[oldY * bp->_floorSize.x + oldX];
				}
			}
		}

		bp->_floor = newFloor;
		bp->_floorSize = sf::Vector2i(newWidth, newHeight);

		_building->generateFloorVertexArray();
	}

	void EditableBuilding::resizeWalls(int offsetX, int offsetY) {

		std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(_building->_prefab.lock());
		if (!bp) return;

		int newWidth = _rect.size.x / 32;
		int newHeight = _rect.size.y / 32;

		std::vector<int> newWalls(newWidth * newHeight, -1);

		for (int y = 0; y < newHeight; y++) {
			for (int x = 0; x < newWidth; x++) {
				int oldX = x - offsetX;
				int oldY = y - offsetY;

				if (oldX >= 0 && oldX < bp->_wallsSize.x &&
					oldY >= 0 && oldY < bp->_wallsSize.y) {

					newWalls[y * newWidth + x] = bp->_walls[oldY * bp->_wallsSize.x + oldX];
				}
			}
		}

		bp->_walls = newWalls;
		bp->_wallsSize = sf::Vector2i(newWidth, newHeight);

		_building->generateWalls();
	}

	void EditableBuilding::resizeRoof() {

		std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(_building->_prefab.lock());
		if (!bp) return;

		_building->generateRoofs(_scale);
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
		resizeRoof();
		_building->generateCollider(_scale);
		setPosition(_rect.position);
		_building->setPosition(_rect.position);
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

		sf::IntRect rect = sf::IntRect(BuildingsEditor::editor->_building_panel->getPosition(), BuildingsEditor::editor->_building_panel->getSize());
		if (rect.contains(BuildingsEditor::editor->_building_panel->_cursorOnBuilding->_globalPosition)) {
			ResizableShape::cursorHover();
		}

		_building->cursorHover();
		
	}

	void EditableBuilding::handleEvent(const sf::Event& event) {

		sf::IntRect rect = sf::IntRect(BuildingsEditor::editor->_building_panel->getPosition(), BuildingsEditor::editor->_building_panel->getSize());

		if (!(BuildingsEditor::editor->_building_panel->_building.get() == this && rect.contains(BuildingsEditor::editor->_building_panel->_cursorOnBuilding->_globalPosition)) && GUI_manager->Element_pressed == nullptr)
			return;

		if (_state == EditableBuildingStates::Resizing) {
			_state = EditableBuildingStates::Idle;
		}

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
		
				//if (_editState == EditableBuildingEditStates::Floor) {
				//	editTileUnderCursor();
				//	return;
				//}
				//if (_editState == EditableBuildingEditStates::GameObject) {
				//	addWallUnderCursor();
				//	return;
				//}
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
			GUI_manager->Element_pressed = ResizableShape::shared_from_this();

			//if(_editState == EditableBuildingEditStates::Floor) {
			//	BuildingsEditor::editor->_palette->_slots->selectSlot(-1);
			//	return;
			//}
			//
			//if(_editState == EditableBuildingEditStates::GameObject) {
			//	BuildingsEditor::editor->_palette->_slots->selectSlot(-1);
			//	return;
			//}
			//
			//if (_editState == EditableBuildingEditStates::None) {
			//	removeWallUnderCursor();
			//	return;
			//}
		}

		if(const auto* mm = event.getIf<sf::Event::MouseMoved>(); mm) {
			if (GUI_manager->Element_pressed.get() == this) {
				//if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				//	if (_editState == EditableBuildingEditStates::Floor) {
				//		editTileUnderCursor();
				//		return;
				//	}
				//
				//	if (_editState == EditableBuildingEditStates::GameObject) {
				//		addWallUnderCursor();
				//		return;
				//	}
				//}
				//
				//if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
				//	if (_editState == EditableBuildingEditStates::None) {
				//		removeWallUnderCursor();
				//		return;	
				//	}
				//}
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

			return;
		}


		if (const auto* mws = event.getIf<sf::Event::MouseWheelScrolled>(); mws) {
			float oldScale = _scale;
			float newScale = std::clamp(_scale + mws->delta * 0.125f, 0.5f, 2.0f);
			sf::Vector2f cursorPosition(Cursors::cursor->_position);
			sf::Vector2f oldPosition(ResizableShape::getPosition());
			float scaleFactor = newScale / oldScale;
			sf::Vector2f newPosition = cursorPosition + (oldPosition - cursorPosition) * scaleFactor;
			_scale = newScale;
			setPosition(sf::Vector2i(newPosition));
			_building->setPosition(sf::Vector2i(newPosition));

			_building->generateFloorVertexArray(_scale);
			_building->generateWalls(_scale);
			_building->generateRoofs(_scale);

			generateEdgePoints();
		}
	}

	void EditableBuilding::update() {
		if (_state == EditableBuildingStates::Moving) {

			sf::Vector2i oldPosition = ResizableShape::getPosition();
			sf::Vector2i newPosition = Cursors::cursor->_position - _offset;
			
			if (BuildingsEditor::editor->_building_panel->_building.get() == this) {
				newPosition = clampPosition(newPosition);
			}

			sf::Vector2i delta = newPosition - oldPosition;

			setPosition(newPosition);
			_building->setPosition(newPosition);
			_building->generateFloorVertexArray(_scale);
			_building->generateWalls(_scale);
			_building->generateRoofs(_scale);
			_building->generateCollider(_scale);
			return;
		}
		 
		if (_state == EditableBuildingStates::Resizing) {
			std::wcout << L"EditableBuilding::update() - _state == Resizing" << std::endl;
			for (auto& point : _edgePoints) {
				if(point == GUI_manager->Element_pressed) {
					resize(point);
					sf::Vector2i newPos = clampPosition(ResizableShape::getPosition());
					ResizableShape::setPosition(newPos);
					_building->update();
					return;
				}
			}

			for (auto& point : _edgePoints) {
				point->update();
			}

			return;
		}

		std::wcout << L"EditableBuilding::update() - _state == Idle" << std::endl;
		_building->update();
	}

	void EditableBuilding::drawOnlyShape() {
		ResizableShape::drawOnlyRect();
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
		_building->drawOnlyFloor();
		_building->drawOnlyWalls(_scale);
		_building->drawOnlyRoof(_scale);
		drawOnlyEdgePoints();

		

	}
}
