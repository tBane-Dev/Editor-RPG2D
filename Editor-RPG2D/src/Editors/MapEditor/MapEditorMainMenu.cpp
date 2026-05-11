#include "Editors/MapEditor/MapEditorMainMenu.hpp"
#include "Components/MainMenu/MenuButton.hpp"

MapEditorMainMenu::MapEditorMainMenu() : MainMenu() {
	file = std::make_shared<MenuButton>(std::make_shared<MainMenu>(*this), L"File");
	render = std::make_shared<MenuButton>(std::make_shared<MainMenu>(*this), L"Render");
	tools = std::make_shared<MenuButton>(std::make_shared<MainMenu>(*this), L"Tools");
	settings = std::make_shared<MenuButton>(std::make_shared<MainMenu>(*this), L"Settings");
	
	_menu_boxes.push_back(file);
	_menu_boxes.push_back(render);
	_menu_boxes.push_back(tools);
	_menu_boxes.push_back(settings);

	for(auto& mb : _menu_boxes) {
		mb->_onclick_func = [this, mb]() {
			hideMenu();
			openMenuButton(mb);
			};
	}

	// FILE

	_file_new_project = std::make_shared<OptionWithIcon>(
		L"New project", 
		textures_manager->getTexture(L"assets\\tex\\main_menu\\new_project.png"), 
		textures_manager->getTexture(L"assets\\tex\\main_menu\\new_project.png"), 
		L"Ctrl+N"
		);

	_file_open_project = std::make_shared<OptionWithIcon>(
		L"Open project",
		textures_manager->getTexture(L"assets\\tex\\main_menu\\open_project.png"),
		textures_manager->getTexture(L"assets\\tex\\main_menu\\open_project.png"),
		L"Ctrl+O"
	);

	_file_save_project = std::make_shared<OptionWithIcon>(
		L"Save project", 
		textures_manager->getTexture(L"assets\\tex\\main_menu\\save_project.png"),
		textures_manager->getTexture(L"assets\\tex\\main_menu\\save_project.png"),
		L"Ctrl+S"
	);

	file->addOption(_file_new_project);
	file->addOption(_file_open_project);
	file->addOption(_file_save_project);

	// RENDER

	_render_chunks_info = std::make_shared<OptionWithCheckbox>(L"Chunks info", textures_manager->getTexture(L"assets\\tex\\unchecked.png"), textures_manager->getTexture(L"assets\\tex\\unchecked_hover.png"));
	_render_chunks_info->addValue(textures_manager->getTexture(L"assets\\tex\\checked.png"), textures_manager->getTexture(L"assets\\tex\\checked_hover.png"));
	
	_render_sprites_outline = std::make_shared<OptionWithCheckbox>(L"Sprites outline", textures_manager->getTexture(L"assets\\tex\\unchecked.png"), textures_manager->getTexture(L"assets\\tex\\unchecked_hover.png"));
	_render_sprites_outline->addValue(textures_manager->getTexture(L"assets\\tex\\checked.png"), textures_manager->getTexture(L"assets\\tex\\checked_hover.png"));

	_render_colliders = std::make_shared<OptionWithCheckbox>(L"Colliders", textures_manager->getTexture(L"assets\\tex\\unchecked.png"), textures_manager->getTexture(L"assets\\tex\\unchecked_hover.png"));
	_render_colliders->addValue(textures_manager->getTexture(L"assets\\tex\\checked.png"), textures_manager->getTexture(L"assets\\tex\\checked_hover.png"));

	_render_paths = std::make_shared<OptionWithCheckbox>(L"Paths", textures_manager->getTexture(L"assets\\tex\\unchecked.png"), textures_manager->getTexture(L"assets\\tex\\unchecked_hover.png"));
	_render_paths->addValue(textures_manager->getTexture(L"assets\\tex\\checked.png"), textures_manager->getTexture(L"assets\\tex\\checked_hover.png"));

	_render_meshes = std::make_shared<OptionWithCheckbox>(L"Meshes", textures_manager->getTexture(L"assets\\tex\\unchecked.png"), textures_manager->getTexture(L"assets\\tex\\unchecked_hover.png"));
	_render_meshes->addValue(textures_manager->getTexture(L"assets\\tex\\checked.png"), textures_manager->getTexture(L"assets\\tex\\checked_hover.png"));

	render->addOption(_render_chunks_info);
	render->addOption(_render_sprites_outline);
	render->addOption(_render_colliders);
	render->addOption(_render_paths);
	render->addOption(_render_meshes);

	// POSITIONING

	setPosition(sf::Vector2i(0, 0));
}

MapEditorMainMenu::~MapEditorMainMenu() {
	
}