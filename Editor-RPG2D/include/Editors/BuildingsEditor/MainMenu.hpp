#pragma once
#include "Components/MainMenu/MainMenu.hpp"
#include "Controls/OptionWithIcon.hpp"
#include "Controls/OptionWithCheckbox.hpp"

namespace BuildingsEditor {
	class MainMenu : public Components::MainMenu {
	public:

		std::shared_ptr<Components::MenuButton> _file;
		std::shared_ptr<Components::MenuButton> _render;
		std::shared_ptr<Components::MenuButton> _settings;

		// FILE
		std::shared_ptr<OptionWithIcon> _file_back_to_map_editor;
		std::shared_ptr<OptionWithIcon> _file_exit;

		// RENDER
		std::shared_ptr<OptionWithCheckbox>	_render_grid;

		MainMenu();
		~MainMenu();

		void handleEvent(const sf::Event& event);
	};
}