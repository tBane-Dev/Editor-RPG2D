#pragma once
#include "EditorsManager.hpp"
#include "Editors/BuildingsEditor/MainMenu.hpp"
#include "Editors/BuildingsEditor/Palette.hpp"
#include "Editors/BuildingsEditor/BuildingsListPanel.hpp"

namespace BuildingsEditor {

	class Editor : public Main::Editor {
	public:

		std::unique_ptr<sf::Text> _title;
		sf::Vector2i _margin;

		std::shared_ptr<MainMenu> _main_menu;
		std::shared_ptr<Palette> _palette;
		std::shared_ptr<BuildingsListPanel> _list_panel;

		Editor();
		~Editor();

		void init();
		void createMainMenu();
		void createPalette();
		void createListPanel();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};

	extern std::shared_ptr<Editor> editor;

}