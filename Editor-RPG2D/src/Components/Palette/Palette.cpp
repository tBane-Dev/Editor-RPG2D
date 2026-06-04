#include "Components/Palette/Palette.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Objects/Monster.hpp"
#include "PrefabsManager.hpp"
#include "RenderWindow.hpp"
#include "Cursor.hpp"
#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "DebugLog.hpp"


namespace Components {
	Palette::Palette() : Element() {
		sf::Vector2i size;
		size.x = 600;
		size.y = Main::render_window->getSize().y - MapEditor::editor->_main_menu->getSize().y;
		_rect = sf::IntRect(sf::Vector2i(Main::render_window->getSize().x - size.x, MapEditor::editor->_main_menu->getSize().y), size);

		_categories = std::make_shared<Categories>();
		_tools = std::make_shared<Tools>();
		_slots = std::make_shared<Slots>();

		_minBrushSize = 0;
		_maxBrushSize = 5;
		_brushSize = 2;

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr,
			ObjectType::None,
			[this]() {
				loadAll(ObjectType::None);
			}
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr, 
			ObjectType::None,
			[this]() {
				loadAll(ObjectType::None);
			}
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr, 
			ObjectType::None,
			[this]() {
				loadAll(ObjectType::None);
			}
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr, 
			ObjectType::None,
			[this]() {
				loadAll(ObjectType::None);
			}
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr,
			ObjectType::None,
			[this]() {
				loadAll(ObjectType::None);
			}
		);

		setPosition(sf::Vector2i(_rect.position));

		// set the active group
		loadAll(ObjectType::None); // TO-DO - must be - selectCategory
		_tools->setTool(nullptr, ToolType::None);
	}

	Palette::~Palette() {

	}


	sf::Vector2i Palette::getSize() {
		return _rect.size;
	}

	void Palette::loadAll(ObjectType type) {
		_categories->setCategory(type);

		sf::Vector2i slotsPosition = sf::Vector2i(_rect.position.x, _categories->getPosition().y + _categories->getSize().y);
		if (_categories->_selectedType == ObjectType::Terrain) {
			slotsPosition.y += _tools->getSize().y + 16;
		}
		_slots->setPosition(slotsPosition);
		_slots->setCategory(type);

		
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
		if (MapEditor::editor->_main_menu->_tool_palette->_checkbox->_value != 0)
			return;

		if (_rect.contains(Main::cursor->_position)) {
			GUI_manager->Element_hovered = shared_from_this();
		}

		_categories->cursorHover();

		if (_categories->_selectedType == ObjectType::Terrain)
			_tools->cursorHover();

		_slots->cursorHover();

	}

	void Palette::handleEvent(const sf::Event& event) {

		if (MapEditor::editor->_main_menu->_tool_palette->_checkbox->_value != 0)
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

		if (MapEditor::editor->_main_menu->_tool_palette->_checkbox->_value == 0) {
			sf::RectangleShape rect(sf::Vector2f(_rect.size));
			rect.setFillColor(sf::Color(31, 31, 31));
			rect.setPosition(sf::Vector2f(_rect.position));
			Main::render_window->draw(rect);

			_categories->draw();
			if (_categories->_selectedType == ObjectType::Terrain)
				_tools->draw();
			_slots->draw();

			GUI_manager->setView();
		}




	}

}