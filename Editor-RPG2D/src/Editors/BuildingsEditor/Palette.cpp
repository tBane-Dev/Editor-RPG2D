#include "Editors/BuildingsEditor/Palette.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"

namespace BuildingsEditor {

	Palette::Palette() : Components::Palette() {

		_categories = std::make_shared<Categories>();

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\walls.png"),
			ObjectType::Wall,
			[this]() { loadAll(ObjectType::Wall); }
		);


		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\floors.png"),
			ObjectType::Floor,
			[this]() { loadAll(ObjectType::Floor); }
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\doors.png"),
			ObjectType::Door,
			[this]() { loadAll(ObjectType::Door); }
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\windows.png"),
			ObjectType::Window,
			[this]() { loadAll(ObjectType::Window); }
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\wall_mounted.png"),
			ObjectType::WallMounted,
			[this]() { loadAll(ObjectType::WallMounted); }
		);

		_tools = std::make_shared<Tools>();

		_tools->addTool(
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\cursor.png"),
			[this]() { _tools->setTool(_tools->_tools[0], ToolType::None); }
		);

		_tools->addTool(
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\circle.png"),
			[this]() {
				_tools->setTool(_tools->_tools[1], ToolType::Circle);
				if (_slots->_selectedSlot == nullptr) {
					if (_slots->_slots.size() > 0) {
						_slots->selectSlot(1);
						BuildingsEditor::editor->_building_panel->_cursorOnBuilding->_object = _slots->_slots[1]->_object;
					}
				}
			}
		);

		_tools->addTool(
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\rect.png"),
			[this]() { _tools->setTool(
				_tools->_tools[2], ToolType::Rect);

		if (_slots->_selectedSlot == nullptr) {
			if (_slots->_slots.size() > 0) {
				_slots->selectSlot(1);
				BuildingsEditor::editor->_building_panel->_cursorOnBuilding->_object = _slots->_slots[1]->_object;
			}
		}
			}
		);

		_tools->addTool(
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\decrease.png"),
			[this]() { BuildingsEditor::editor->_palette->_brushSize = std::clamp(BuildingsEditor::editor->_palette->_brushSize - 1, BuildingsEditor::editor->_palette->_minBrushSize, BuildingsEditor::editor->_palette->_maxBrushSize); }
		);

		_tools->addTool(
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\increase.png"),
			[this]() { BuildingsEditor::editor->_palette->_brushSize = std::clamp(BuildingsEditor::editor->_palette->_brushSize + 1, BuildingsEditor::editor->_palette->_minBrushSize, BuildingsEditor::editor->_palette->_maxBrushSize); }
		);

		_slots = std::make_shared<Slots>();

		setPosition(sf::Vector2i(_rect.position));

		// set the active group
		loadAll(ObjectType::Wall);
		_tools->setTool(nullptr, ToolType::None);
		//_slots->setCategory(ObjectType::None);
	}

	Palette::~Palette() {

	}

	void Palette::loadAll(ObjectType type) {
		_categories->setCategory(type);

		sf::Vector2i slotsPosition = sf::Vector2i(_rect.position.x, _categories->getPosition().y + _categories->getSize().y);
		if (_categories->_selectedType == ObjectType::Terrain || _categories->_selectedType == ObjectType::Floor) {
			slotsPosition.y += _tools->getSize().y + 16;
		}
		_slots->setPosition(slotsPosition);
		_slots->setCategory(type);

		if (_categories->_selectedType == ObjectType::Floor) {
			_slots->setFunction(
				[this](std::shared_ptr<Slot> slot, int selectedSlotId) {
					if (!(_tools->_toolType == ToolType::Circle || _tools->_toolType == ToolType::Rect))
						_tools->setTool(_tools->_tools[1], ToolType::Circle);
					BuildingsEditor::editor->_building_panel->_building->_editState = EditableBuildingEditStates::Floor;
					BuildingsEditor::editor->_building_panel->_cursorOnBuilding->_object = slot->_object;
					_slots->selectSlot(selectedSlotId);
				}
			);
		}
		else {
			_slots->setFunction(
				[this](std::shared_ptr<Slot> slot, int selectedSlotId) {
					BuildingsEditor::editor->_building_panel->_building->_editState = EditableBuildingEditStates::GameObject;
					_slots->selectSlot(selectedSlotId);
				}
			);
		}
	}

	void Palette::draw() {
		Components::Palette::draw();
	}
}
