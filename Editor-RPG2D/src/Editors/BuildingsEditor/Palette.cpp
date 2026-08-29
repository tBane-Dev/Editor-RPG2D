#include "Editors/BuildingsEditor/Palette.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "DebugLog.hpp"

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

		_tools = std::make_shared<ToolsTerrain>();
		_slots = std::make_shared<Slots>();


		// set the active group
		loadAll(ObjectType::Wall);
		//_slots->setCategory(ObjectType::None);
	}

	Palette::~Palette() {

	}

	void Palette::addTools() {
		
		_tools = nullptr;

		if (_categories->_selectedType == ObjectType::Floor) {
			std::shared_ptr<ToolsTerrain> t = std::make_shared<ToolsTerrain>();
			_tools = t;

			t->addTool(
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\cursor.png"),
				[this, t]() { t->setTool(t->_tools[0], ToolTerrainType::None); }
			);

			t->addTool(
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\circle.png"),
				[this, t]() {
					t->setTool(t->_tools[1], ToolTerrainType::Circle);
					if (_slots->_selectedSlot == nullptr) {
						if (_slots->_slots.size() > 0) {
							_slots->selectSlot(1);
							BuildingsEditor::editor->_building_panel->_cursorOnBuilding->_object = _slots->_slots[1]->_object;
						}
					}
				}
			);

			t->addTool(
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\rect.png"),
				[this, t]() { t->setTool(
					t->_tools[2], ToolTerrainType::Rect);

			if (_slots->_selectedSlot == nullptr) {
				if (_slots->_slots.size() > 0) {
					_slots->selectSlot(1);
					BuildingsEditor::editor->_building_panel->_cursorOnBuilding->_object = _slots->_slots[1]->_object;
				}
			}
				}
			);

			t->addTool(
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\decrease.png"),
				[this, t]() { BuildingsEditor::editor->_palette->_brushSize = std::clamp(BuildingsEditor::editor->_palette->_brushSize - 1, BuildingsEditor::editor->_palette->_minBrushSize, BuildingsEditor::editor->_palette->_maxBrushSize); }
			);

			t->addTool(
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\increase.png"),
				[this, t]() { BuildingsEditor::editor->_palette->_brushSize = std::clamp(BuildingsEditor::editor->_palette->_brushSize + 1, BuildingsEditor::editor->_palette->_minBrushSize, BuildingsEditor::editor->_palette->_maxBrushSize); }
			);
		}
	}

	void Palette::loadAll(ObjectType type) {
		_categories->setCategory(type);
		addTools();
		

		if(auto tools = std::dynamic_pointer_cast<ToolsTerrain>(_tools); tools) {
			if (tools->_toolType == ToolTerrainType::None) {
				tools->setTool(tools->_tools[0], ToolTerrainType::None);
			}
		}

		_slots->setCategory(type);

		if (_categories->_selectedType == ObjectType::Floor) {
			_slots->setFunction(
				[this](std::shared_ptr<Slot> slot, int selectedSlotId) {
					if (auto tools = std::dynamic_pointer_cast<ToolsTerrain>(_tools); !( tools && (tools->_toolType == ToolTerrainType::Circle || tools->_toolType == ToolTerrainType::Rect)))
						tools->setTool(tools->_tools[1], ToolTerrainType::Circle);
					BuildingsEditor::editor->_building_panel->_cursorOnBuilding->_object = slot->_object;
					_slots->selectSlot(selectedSlotId);
				}
			);
		}
		else {
			_slots->setFunction(
				[this](std::shared_ptr<Slot> slot, int selectedSlotId) {
					BuildingsEditor::editor->_building_panel->_cursorOnBuilding->_object = slot->_object;
					_slots->selectSlot(selectedSlotId);
				}
			);
		}

		setPosition(getPosition());
	}

	void Palette::draw() {
		Components::Palette::draw();
	}
}
