#include "Components/Palette/Palette.hpp"
#include "Editors/MapEditor/MapEditor.hpp"
#include "Editors/MapEditor/Objects/Monster.hpp"
#include "PrefabsManager.hpp"
#include "Window.hpp"
#include <Cursor.hpp>
#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "DebugLog.hpp"


Palette::Palette() : Element() {
	sf::Vector2i size;
	size.x = 600;
	size.y = window->getSize().y - map_editor->_main_menu->getSize().y;
	_rect = sf::IntRect(sf::Vector2i(window->getSize().x - size.x, map_editor->_main_menu->getSize().y), size);

	_categories = std::make_shared<Categories>();
	
	_categories->addCategory(
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\terrain.png"),
		[this]() { 
			loadAll(_categories->_categories[0], ObjectType::Terrain); // TO-DO - must be - selectCategory
		}
	);

	_categories->addCategory(
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\natures.png"),
		[this]() { loadAll(_categories->_categories[1], ObjectType::Nature); } // TO-DO - must be - selectCategory
	);


	_categories->addCategory(
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\monsters.png"),
		[this]() { loadAll(_categories->_categories[2], ObjectType::Monster); } // TO-DO - must be - selectCategory
	);

	_categories->addCategory(
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\monsters.png"),
		[this]() { loadAll(_categories->_categories[3], ObjectType::Monster); } // TO-DO - must be - selectCategory
	);

	_categories->addCategory(
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\monsters.png"),
		[this]() { loadAll(_categories->_categories[4], ObjectType::Monster); } // TO-DO - must be - selectCategory
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
				if(_slots->_slots.size() > 2) {
					_slots->selectSlot(2);
					map_editor->_cursor_on_map->_object = _slots->_slots[2]->_object;
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
				if (_slots->_slots.size() > 2) {
					_slots->selectSlot(2);
					map_editor->_cursor_on_map->_object = _slots->_slots[2]->_object;
				}
			}
		}
	);

	_tools->addTool(
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\decrease.png"),
		[this]() { map_editor->_palette->_brushSize = std::clamp(map_editor->_palette->_brushSize - 1, map_editor->_palette->_minBrushSize, map_editor->_palette->_maxBrushSize); }
	);

	_tools->addTool(
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\increase.png"),
		[this]() { map_editor->_palette->_brushSize = std::clamp(map_editor->_palette->_brushSize + 1, map_editor->_palette->_minBrushSize, map_editor->_palette->_maxBrushSize); }
	);

	_slots = std::make_shared<Slots>();

	_minBrushSize = 0;
	_maxBrushSize = 5;
	_brushSize = 1;
	
	setPosition(sf::Vector2i(_rect.position));


	// set the active group
	loadAll(_categories->_categories[0], ObjectType::Terrain); // TO-DO - must be - selectCategory
	_tools->setTool(_tools->_tools[0], ToolType::None);
	
	//_slots->setCategory(ObjectType::Terrain);
}

Palette::~Palette() {

}


sf::Vector2i Palette::getSize() {
	return _rect.size;
}

void Palette::loadAll(std::shared_ptr<CategoryButton> category, ObjectType type) {
	_categories->setCategory(category, type);

	sf::Vector2i slotsPosition = sf::Vector2i(_rect.position.x, _categories->getPosition().y + _categories->getSize().y);
	if(_categories->_selectedType == ObjectType::Terrain) {
		slotsPosition.y += _tools->getSize().y + 16;
	}
	_slots->setPosition(slotsPosition);
	_slots->setCategory(type);

	if(_categories->_selectedType == ObjectType::Terrain) {
		_slots->setFunction(
			[this](std::shared_ptr<Slot> slot, int selectedSlotId) {
				if (!(_tools->_toolType == ToolType::Circle || _tools->_toolType == ToolType::Rect))
					_tools->setTool(_tools->_tools[1], ToolType::Circle);
				map_editor->_cursor_on_map->_object = slot->_object;
				_slots->selectSlot(selectedSlotId);

			}
		);
	}
	else {
		_slots->setFunction(
			[this](std::shared_ptr<Slot> slot, int selectedSlotId) {
				map_editor->_cursor_on_map->_object = slot->_object;
				_slots->selectSlot(selectedSlotId);
			}
		);
	}
}

void Palette::setPosition(sf::Vector2i position) {

	_rect.position = position;

	int margin = 16;

	_categories->setPosition(position);
	
	_tools->setPosition(position + sf::Vector2i(0, _categories->getSize().y + margin));
	
	if (_categories->_selectedCategory != nullptr && _categories->_selectedType == ObjectType::Terrain)
		_slots->setPosition(position + sf::Vector2i(0, _categories->getSize().y + margin + _tools->getSize().y + margin));
	else
		_slots->setPosition(position + sf::Vector2i(0, _categories->getSize().y + margin));

	
}

void Palette::cursorHover() {
	if (map_editor->_main_menu->_tool_palette->_checkbox->_value != 0)
		return;

	if (_rect.contains(cursor->_position)) {
		GUI_manager->Element_hovered = shared_from_this();
	}
	
	_categories->cursorHover();
	
	if (_categories->_selectedType == ObjectType::Terrain)
		_tools->cursorHover();

	_slots->cursorHover();

}

void Palette::handleEvent(const sf::Event& event) {

	if (map_editor->_main_menu->_tool_palette->_checkbox->_value != 0)
		return;

	if (GUI_manager->Element_hovered.get() == this) {
		if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
			GUI_manager->Element_pressed = shared_from_this();
		}
	}

	if (GUI_manager->Element_pressed.get() == this) {
		if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
			GUI_manager->Element_pressed = nullptr;
		}
	}

	_categories->handleEvent(event);

	if (_categories->_selectedType == ObjectType::Terrain)
		_tools->handleEvent(event);

	_slots->handleEvent(event);

}

void Palette::update() {

	_categories->update();
	_tools->update();
	_slots->update();

}

void Palette::draw() {

	GUI_manager->setView();

	if (map_editor->_main_menu->_tool_palette->_checkbox->_value == 0) {
		sf::RectangleShape rect(sf::Vector2f(_rect.size));
		rect.setFillColor(sf::Color(31, 31, 31));
		rect.setPosition(sf::Vector2f(_rect.position));
		window->draw(rect);

		_categories->draw();
		if (_categories->_selectedType == ObjectType::Terrain)
			_tools->draw();
		_slots->draw();

		GUI_manager->setView();
	}

	
	

}
