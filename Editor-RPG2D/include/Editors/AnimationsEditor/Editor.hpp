#pragma once
#include "Editors/Editor.hpp"
#include "Editors/AnimationsEditor/MainMenu.hpp"
#include "Components/Palette/Palette.hpp"

namespace AnimationsEditor {
	class Editor : public Main::Editor {
	public:

		std::unique_ptr<sf::Text> _title;
		sf::Vector2i _margin;

		std::shared_ptr<MainMenu> _main_menu;

		Editor();
		~Editor();

		void init();
		void createMainMenu();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};

	extern std::shared_ptr<Editor> editor;
}