#pragma once
#include "Components/MainMenu/MainMenu.hpp"
#include "Controls/Option.hpp"
#include "Controls/OptionWithIcon.hpp"
#include "Controls/OptionWithCheckbox.hpp"

namespace MapEditor {
	class MainMenu : public Components::MainMenu {
	public:

		std::shared_ptr<Components::MenuButton> _file;
		std::shared_ptr<Components::MenuButton> _render;
		std::shared_ptr<Components::MenuButton> _tools;
		std::shared_ptr<Components::MenuButton> _settings;

		// FILE
		std::shared_ptr<OptionWithIcon> _file_new_project;
		std::shared_ptr<OptionWithIcon> _file_open_project;
		std::shared_ptr<OptionWithIcon> _file_save_project;
		std::shared_ptr<OptionWithIcon> _file_exit;

		// RENDER
		std::shared_ptr<OptionWithCheckbox>	_render_chunks_info;
		std::shared_ptr<OptionWithCheckbox> _render_sprites_outline;
		std::shared_ptr<OptionWithCheckbox> _render_colliders;
		std::shared_ptr<OptionWithCheckbox> _render_paths;
		std::shared_ptr<OptionWithCheckbox> _render_meshes;

		// TOOLS
		std::shared_ptr<OptionWithCheckbox> _tool_palette;
		std::shared_ptr<OptionWithIcon> _tool_animations_editor;
		std::shared_ptr<OptionWithIcon> _tool_prefabs_editor;
		std::shared_ptr<OptionWithIcon> _tool_buildings_editor;

		MainMenu();
		~MainMenu();

		void handleEvent(const sf::Event& event);
	};
}