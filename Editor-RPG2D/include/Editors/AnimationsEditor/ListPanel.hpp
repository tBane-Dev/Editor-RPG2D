#pragma once
#include "Components/Panel.hpp"
#include "Controls/TextInput.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"
#include "Controls/Scrollbar.hpp"

namespace AnimationsEditor {

	class ListPanelItem : public Button {
	public:
		std::unique_ptr<sf::Text> _text;

		ListPanelItem(sf::Vector2i size);
		~ListPanelItem();

		void setName(std::wstring name);
		virtual void setPosition(sf::Vector2i position);
		virtual void cursorHover();
		virtual void handleEvent(const sf::Event& event);
		virtual void update();
		virtual void draw();
	};

	class ListPanel : public Components::Panel {
	public:

		std::unique_ptr<sf::Text> _title;
		sf::IntRect _list_rect;
		std::vector<std::shared_ptr<ListPanelItem>> _items;

		std::shared_ptr<ListPanelItem> _selectedItem;
		int _selectedItemIndex = -1;

		std::shared_ptr<Scrollbar> _scrollbar;

		ListPanel(sf::Vector2i margin);
		~ListPanel();

		void selectItem(int index);
		void loadList();
		void loadScrollbar();
		void loadAll();
		sf::FloatRect getListRect();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};
}
