#include "Editors/AnimationsEditor/ListPanel.hpp"
#include "Editors/AnimationsEditor/Editor.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"
#include "Objects/Monster.hpp"
#include "PrefabsManager.hpp"

namespace AnimationsEditor {
	ListPanelItem::ListPanelItem(sf::Vector2i size) : Button() {
		_rect.size = size;
		_text = std::make_unique<sf::Text>(basicFont, L"", 20);
		_text->setFillColor(button_text_color);
	}

	ListPanelItem::~ListPanelItem() {

	}

	void ListPanelItem::setName(std::wstring name) {
		_text->setString(name);
	}

	void ListPanelItem::setPosition(sf::Vector2i position) {
		Button::setPosition(position);
		sf::Vector2f textPosition;
		textPosition.x = (float)_rect.position.x + 16.f;
		textPosition.y = (float)_rect.position.y + (_rect.size.y - basic_text_rect_height) / 2.f;
		_text->setPosition(textPosition);
	}


	void ListPanelItem::cursorHover() {
		if(_text->getString() != L"")
			Button::cursorHover();
	}

	void ListPanelItem::handleEvent(const sf::Event& event) {
		if (_text->getString() != L"")
			Button::handleEvent(event);
	}

	void ListPanelItem::update() {
		if (_text->getString() != L"")
			Button::update();
	}

	void ListPanelItem::draw() {
		Button::draw();
		Main::render_window->draw(*_text);
	}

	ListPanel::ListPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 600), sf::Vector2i(margin.x, AnimationsEditor::editor->_main_menu->getSize().y + margin.y)) {

		// title
		_title = std::make_unique<sf::Text>(basicFont, L"Animations List", 20);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));
		
		// list background rect
		{
			int x = _rect.position.x + 16;
			int y = _rect.position.y + 48;
			int w = _rect.size.x - 32 - 32;
			int h = _rect.size.y - 64;
			_list_rect = sf::IntRect(sf::Vector2i(x,y), sf::Vector2i(w, h));
		}

		// items
		{
			int border = 2;
			int w = _list_rect.size.x - 2*border;
			int h = basic_text_rect_height;
			int items_count = std::ceil((float)_list_rect.size.y / (float)h) + 1;
			for (int i = 0; i < items_count; i++) {
				
				std::shared_ptr<ListPanelItem> item = std::make_shared<ListPanelItem>(sf::Vector2i(w, h));
				item->setRectColors(
					button_idle_color, button_hover_color, button_press_color, 
					button_select_idle_color, button_select_hover_color, button_select_press_color, 
					button_inactive_color,
					2, 
					sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent, sf::Color(0, 0, 0), sf::Color(0, 0, 0));

				item->setPosition(sf::Vector2i(_list_rect.position.x+border, _list_rect.position.y + border + i * (h)));
				_items.push_back(item);
			}

			
		}

		loadList();
		loadScrollbar();
	}

	ListPanel::~ListPanel() {

	}

	void ListPanel::selectItem(int index) {
		
		if(_selectedItem)
			_selectedItem->setSelect(false);

		_selectedItem = nullptr;
		_selectedItemIndex = -1;

		if(index >= 0 && index < animations_manager->getAnimationsCount()) {
			
			_selectedItemIndex = index;

			int localIndex = index - _scrollbar->getValue() / basic_text_rect_height;

			if (localIndex >= 0 && localIndex < _items.size()) {
				_selectedItem = _items[localIndex];
				_selectedItem->setSelect(true);
			}
		}
	}

	void ListPanel::loadList() {

		for (int i = 0; i < _items.size(); i++) {

			_items[i]->setName(L"");

			sf::Vector2i position;
			position.x = _list_rect.position.x;
			position.y = _list_rect.position.y + (i)*basic_text_rect_height;
			
			if(_scrollbar) 
				position.y -= _scrollbar->getValue() % basic_text_rect_height;

			_items[i]->setPosition(position);
		}

		int max_id = (animations_manager->getAnimationsCount() < _items.size()) ? animations_manager->getAnimationsCount() : _items.size();

		for (int i = 0; i < max_id; i++) {
			std::shared_ptr<ListPanelItem> item = _items[i];

			int index = i;
			if (_scrollbar) index += _scrollbar->getValue() / basic_text_rect_height;

			if (index < animations_manager->getAnimationsCount()) {
				item->setName(animations_manager->getAnimations(index)->_path);
				item->_onclick_func = [this, index, item]() {

					selectItem(index);

					editor->_animations = std::make_shared<Animations>(*animations_manager->getAnimations(index));

					editor->_animator = std::make_shared<Animator>(editor->_animations, 0.2f);
					editor->_animation_name_panel->loadAnimations();;
					editor->_sprite_sheet_panel->loadAnimations();
					editor->_preview_panel->loadAnimations();
					};
			}
			else {
				item->setName(L"");
				item->_onclick_func = {};
			}



		}
	}

	void ListPanel::loadScrollbar() {

		int x = _list_rect.position.x + _list_rect.size.x;
		int y = _list_rect.position.y;
		int w = 32;
		int h = _list_rect.size.y;

		int min = 0;
		int max = animations_manager->getAnimationsCount() * basic_text_rect_height - _list_rect.size.y;
		if (max < 0) max = 0;
		int slider_size = _list_rect.size.y;
		int value = 0;

		if (_scrollbar) {
			value = _scrollbar->getValue();

			if (value < min) value = min;
			if (value > max) value = max;
		}

		_scrollbar = std::make_shared<Scrollbar>(x, y, w, h, min, max, slider_size, value);
		_scrollbar->setScrollArea(std::make_shared<sf::IntRect>(_list_rect), basic_text_rect_height/4);
		_scrollbar->_func = [this]() {
			loadList();
			selectItem(_selectedItemIndex);
			};
	}

	void ListPanel::loadAll() {
		
		loadScrollbar();
		loadList();
	}

	sf::FloatRect ListPanel::getListRect() {

		return sf::FloatRect(
			sf::Vector2f((float)_list_rect.position.x, (float)_list_rect.position.y),
			sf::Vector2f((float)_list_rect.size.x, (float)(_list_rect.size.y))
		);
	}


	void ListPanel::cursorHover() {
		Components::Panel::cursorHover();
		_scrollbar->cursorHover();

		for(auto& item : _items) {
			item->cursorHover();
		}
	}

	void ListPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);
		_scrollbar->handleEvent(event);

		for (auto& item : _items) {
			item->handleEvent(event);
		}
	}

	void ListPanel::update() {
		Components::Panel::update();
		_scrollbar->update();

		for (auto& item : _items) {
			item->update();
		}
	}

	void ListPanel::draw() {

		Components::Panel::draw();

		Main::render_window->draw(*_title);

		// Draw list background
		int border = 2;
		sf::Vector2f rectSize = sf::Vector2f(_list_rect.size.x - 2*border, _list_rect.size.y - 2*border);

		sf::RectangleShape list_bg(rectSize);
		list_bg.setPosition(sf::Vector2f(_list_rect.position.x + border, _list_rect.position.y + border));
		list_bg.setFillColor(sf::Color(23,23,23));
		list_bg.setOutlineThickness((float)border);
		list_bg.setOutlineColor(sf::Color(0, 0, 0));
		Main::render_window->draw(list_bg);

		_scrollbar->draw();

		sf::FloatRect slotsRect = getListRect();
		sf::View view(slotsRect);

		sf::FloatRect vp(
			sf::Vector2f(
				slotsRect.position.x / GUI_manager->_view.getSize().x,
				slotsRect.position.y / GUI_manager->_view.getSize().y
			),

			sf::Vector2f(
				slotsRect.size.x / GUI_manager->_view.getSize().x,
				slotsRect.size.y / GUI_manager->_view.getSize().y
			)
		);

		view.setViewport(vp);
		Main::render_window->setView(view);

		for (auto& item : _items) {
			item->draw();
		}

		

	}
}