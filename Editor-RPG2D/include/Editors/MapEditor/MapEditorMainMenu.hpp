#pragma once
#include "Components/MainMenu/MainMenu.hpp"
#include "Controls/Option.hpp"
#include "Controls/OptionWithIcon.hpp"
#include "Controls/OptionWithCheckbox.hpp"

class MapEditorMainMenu : public MainMenu {
public:

	std::shared_ptr<MenuButton> file;
	std::shared_ptr<MenuButton> render;
	std::shared_ptr<MenuButton> tools;
	std::shared_ptr<MenuButton> settings;

	// FILE
	std::shared_ptr<OptionWithIcon> _file_new_project;
	std::shared_ptr<OptionWithIcon> _file_open_project;
	std::shared_ptr<OptionWithIcon> _file_save_project;

	// RENDER
	std::shared_ptr<OptionWithCheckbox> _render_chunks_info;
	std::shared_ptr<OptionWithCheckbox> _render_sprites_outline;
	std::shared_ptr<OptionWithCheckbox> _render_colliders;
	std::shared_ptr<OptionWithCheckbox> _render_paths;
	std::shared_ptr<OptionWithCheckbox> _render_meshes;
	
	MapEditorMainMenu();
	~MapEditorMainMenu();
};