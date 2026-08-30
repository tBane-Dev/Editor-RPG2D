#include "Editors/MapEditor/Palette.hpp"
#include "Editors/MapEditor/Editor.hpp"

namespace MapEditor {

	Palette::Palette() : Components::Palette() {

		_categories = std::make_shared<Categories>();

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\terrain.png"),
			ObjectType::Terrain,
			[this]() {
				loadAll(ObjectType::Terrain); // TO-DO - must be - selectCategory
			}
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\buildings.png"),
			ObjectType::Building,
			[this]() { loadAll(ObjectType::Building); } // TO-DO - must be - selectCategory
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\natures.png"),
			ObjectType::Nature,
			[this]() { loadAll(ObjectType::Nature); } // TO-DO - must be - selectCategory
		);


		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\monsters.png"),
			ObjectType::Monster,
			[this]() { loadAll(ObjectType::Monster); } // TO-DO - must be - selectCategory
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr,
			ObjectType::None,
			[this]() { loadAll(ObjectType::None); } // TO-DO - must be - selectCategory
		);

		_slots = std::make_shared<Slots>();

		// set the active group
		loadAll(ObjectType::Terrain); // TO-DO - must be - selectCategory

		std::shared_ptr<ToolsTerrain> t = std::dynamic_pointer_cast<ToolsTerrain>(_tools);
		t->setTool(t->_tools[0], ToolTerrainType::None);
		//_slots->setCategory(ObjectType::Terrain);
	}

	Palette::~Palette() {

	}

	void Palette::addTools() {

		_tools = nullptr;

		if (!_categories || !_categories->_selectedCategory) return;

		if(_categories->_selectedType == ObjectType::Terrain) {
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
							MapEditor::editor->_cursor_on_map->_object = _slots->_slots[1]->_object;
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
					MapEditor::editor->_cursor_on_map->_object = _slots->_slots[1]->_object;
				}
			}
				}
			);

			t->addTool(
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\decrease.png"),
				[this, t]() { MapEditor::editor->_palette->_brushSize = std::clamp(MapEditor::editor->_palette->_brushSize - 1, MapEditor::editor->_palette->_minBrushSize, MapEditor::editor->_palette->_maxBrushSize); }
			);

			t->addTool(
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\increase.png"),
				[this, t]() { MapEditor::editor->_palette->_brushSize = std::clamp(MapEditor::editor->_palette->_brushSize + 1, MapEditor::editor->_palette->_minBrushSize, MapEditor::editor->_palette->_maxBrushSize); }
			);

			t->setTool(t->_tools[0], ToolTerrainType::None);
		}

		if (_categories->_selectedType == ObjectType::Building) {
			std::shared_ptr<ToolsBuilding> t = std::make_shared<ToolsBuilding>();
			_tools = t;	
		}
	}

	void Palette::loadAll(ObjectType type) {
		_categories->setCategory(type);
		addTools();
		

		if (auto tools = std::dynamic_pointer_cast<ToolsTerrain>(_tools); tools) {
			if (tools->_toolType == ToolTerrainType::None) {
				tools->setTool(tools->_tools[0], ToolTerrainType::None);
			}
		}

		if (auto tools = std::dynamic_pointer_cast<ToolsBuilding>(_tools); tools) {
			// TO-DO
		}

		_slots->setCategory(type);

		if (_categories->_selectedType == ObjectType::Terrain) {

			_slots->setFunction(
				[this](std::shared_ptr<Slot> slot, int selectedSlotId) {

					if (auto tools = std::dynamic_pointer_cast<ToolsTerrain>(_tools); !(tools && (tools->_toolType == ToolTerrainType::Circle || tools->_toolType == ToolTerrainType::Rect))) {
						if (tools)
							tools->setTool(tools->_tools[1], ToolTerrainType::Circle);
					}

					MapEditor::editor->_cursor_on_map->_object = slot->_object;
					_slots->selectSlot(selectedSlotId);
				}
			);
		}
		else if (_categories->_selectedType == ObjectType::Building) {

			_slots->setFunction(
				[this](std::shared_ptr<Slot> slot, int selectedSlotId) {

					MapEditor::editor->_cursor_on_map->_object = slot->_object;
					std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(MapEditor::editor->_cursor_on_map->_object.lock());

					if (buildingPrefab) {
						buildingPrefab->generate(MapEditor::editor->_cursor_on_map->_globalPosition, 1.0f, nullptr);
					}

					_slots->selectSlot(selectedSlotId);
				}
			);
		}
		else {

			_slots->setFunction(
				[this](std::shared_ptr<Slot> slot, int selectedSlotId) {

					MapEditor::editor->_cursor_on_map->_object = slot->_object;
					_slots->selectSlot(selectedSlotId);
				}
			);
		}

		setPosition(getPosition());
	}

	void Palette::draw() {
		if (MapEditor::editor->_main_menu->_tool_palette->_checkbox->_value == 0) {
			Components::Palette::draw();
		}
	}
}
