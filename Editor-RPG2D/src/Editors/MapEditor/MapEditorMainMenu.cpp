#include "Editors/MapEditor/MapEditorMainMenu.hpp"
#include "Components/MainMenu/MenuButton.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"

MapEditorMainMenu::MapEditorMainMenu() : MainMenu() {
	_file = std::make_shared<MenuButton>(std::make_shared<MainMenu>(*this), L"File");
	_render = std::make_shared<MenuButton>(std::make_shared<MainMenu>(*this), L"Render");
	_tools = std::make_shared<MenuButton>(std::make_shared<MainMenu>(*this), L"Tools");
	_settings = std::make_shared<MenuButton>(std::make_shared<MainMenu>(*this), L"Settings");
	
	_menu_boxes.push_back(_file);
	_menu_boxes.push_back(_render);
	_menu_boxes.push_back(_tools);
	_menu_boxes.push_back(_settings);

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

	_file->addOption(_file_new_project);
	_file->addOption(_file_open_project);
	_file->addOption(_file_save_project);

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

	_render->addOption(_render_chunks_info);
	_render->addOption(_render_sprites_outline);
	_render->addOption(_render_colliders);
	_render->addOption(_render_paths);
	_render->addOption(_render_meshes);

	// TOOLS

	_tool_palette = std::make_shared<OptionWithCheckbox>(L"Palette", textures_manager->getTexture(L"assets\\tex\\checked.png"), textures_manager->getTexture(L"assets\\tex\\checked_hover.png"));
	_tool_palette->addValue(textures_manager->getTexture(L"assets\\tex\\unchecked.png"), textures_manager->getTexture(L"assets\\tex\\unchecked_hover.png"));

	_tool_prefabs_editor = std::make_shared<OptionWithIcon>(
		L"Prefabs editor",
		nullptr,
		nullptr
	);
	_tool_prefabs_editor->_onclick_func = []() {
		if (prefabs_editor == nullptr) {
			prefabs_editor = std::make_shared<PrefabsEditor>();
			prefabs_editor->createMainMenu();
			prefabs_editor->createPalette();
			prefabs_editor->createMainPanel();
			prefabs_editor->createPreviewPanel();
			prefabs_editor->createColliderPanel();
			editor_manager->push_back(prefabs_editor);
		}
		};

	_tools->addOption(_tool_palette);
	_tools->addOption(_tool_prefabs_editor);

	// POSITIONING

	setPosition(sf::Vector2i(0, 0));
}

MapEditorMainMenu::~MapEditorMainMenu() {
	
}

void MapEditorMainMenu::handleEvent(const sf::Event& event) {
	
	

	if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp) {
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
		if (kp->code == sf::Keyboard::Key::B) {
			_tool_palette->_checkbox->_value = (_tool_palette->_checkbox->_value == 0) ? 1 : 0;
			return;
		}
	}

	MainMenu::handleEvent(event);

}