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

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr,
			ObjectType::None,
			[this]() { loadAll(ObjectType::None); } // TO-DO - must be - selectCategory
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
						_slots->selectSlot(0);
						MapEditor::editor->_cursor_on_map->_object = _slots->_slots[1]->_object;
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
						_slots->selectSlot(0);
						MapEditor::editor->_cursor_on_map->_object = _slots->_slots[1]->_object;
					}
				}
			}
		);

		_tools->addTool(
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\decrease.png"),
			[this]() { MapEditor::editor->_palette->_brushSize = std::clamp(MapEditor::editor->_palette->_brushSize - 1, MapEditor::editor->_palette->_minBrushSize, MapEditor::editor->_palette->_maxBrushSize); }
		);

		_tools->addTool(
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\tools\\increase.png"),
			[this]() { MapEditor::editor->_palette->_brushSize = std::clamp(MapEditor::editor->_palette->_brushSize + 1, MapEditor::editor->_palette->_minBrushSize, MapEditor::editor->_palette->_maxBrushSize); }
		);

		_slots = std::make_shared<Slots>();

		setPosition(sf::Vector2i(_rect.position));

		// set the active group
		loadAll(ObjectType::Terrain); // TO-DO - must be - selectCategory
		_tools->setTool(_tools->_tools[0], ToolType::None);

		//_slots->setCategory(ObjectType::Terrain);
	}

	Palette::~Palette() {

	}

	void Palette::loadAll(ObjectType type) {
		_categories->setCategory(type);

		sf::Vector2i slotsPosition = sf::Vector2i(_rect.position.x, _categories->getPosition().y + _categories->getSize().y);
		if (_categories->_selectedType == ObjectType::Terrain) {
			slotsPosition.y += _tools->getSize().y + 16;
		}
		_slots->setPosition(slotsPosition);
		_slots->setCategory(type);

		if (_categories->_selectedType == ObjectType::Terrain) {
			_slots->setFunction(
				[this](std::shared_ptr<Slot> slot, int selectedSlotId) {
					if (!(_tools->_toolType == ToolType::Circle || _tools->_toolType == ToolType::Rect))
						_tools->setTool(_tools->_tools[1], ToolType::Circle);
					MapEditor::editor->_cursor_on_map->_object = slot->_object;
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
	}
}
