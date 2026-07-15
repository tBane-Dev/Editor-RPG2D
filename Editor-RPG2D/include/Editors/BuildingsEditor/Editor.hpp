#pragma once
#include "EditorsManager.hpp"
#include "Editors/BuildingsEditor/MainMenu.hpp"
#include "Editors/BuildingsEditor/Palette.hpp"
#include "Editors/BuildingsEditor/BuildingsListPanel.hpp"
#include "Editors/BuildingsEditor/ActionsPanel.hpp"
#include "Editors/BuildingsEditor/NamePanel.hpp"
#include "Editors/BuildingsEditor/BuildingPanel.hpp"

namespace BuildingsEditor {

	class Editor : public Main::Editor {
	public:

		std::unique_ptr<sf::Text> _title;
		sf::Vector2i _margin;

		std::shared_ptr<MainMenu> _main_menu;
		std::shared_ptr<Palette> _palette;
		std::shared_ptr<BuildingsListPanel> _list_panel;
		std::shared_ptr<ActionsPanel> _actions_panel;
		std::shared_ptr<NamePanel> _name_panel;
		std::shared_ptr<BuildingPanel> _building_panel;

		Editor();
		~Editor();

		void init();
		void createMainMenu();
		void createPalette();
		void createListPanel();
		void createActionsPanel();
		void createNamePanel();
		void createBuildingPanel();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};

	extern std::shared_ptr<Editor> editor;

}