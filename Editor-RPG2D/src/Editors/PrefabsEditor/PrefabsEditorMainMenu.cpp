#include "Editors/PrefabsEditor/PrefabsEditorMainMenu.hpp"
#include "Components/MainMenu/MenuButton.hpp"
#include "Controls/OptionWithIcon.hpp"
#include "Editors/Editor.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"

PrefabsEditorMainMenu::PrefabsEditorMainMenu() : MainMenu() {
	_file = std::make_shared<MenuButton>(std::make_shared<MainMenu>(*this), L"File");
	_tools = std::make_shared<MenuButton>(std::make_shared<MainMenu>(*this), L"Tools");
	_settings = std::make_shared<MenuButton>(std::make_shared<MainMenu>(*this), L"Settings");
	
	_menu_boxes.push_back(_file);
	_menu_boxes.push_back(_tools);	
	_menu_boxes.push_back(_settings);


	for(auto& mb : _menu_boxes) {
		mb->_onclick_func = [this, mb]() {
			hideMenu();
			openMenuButton(mb);
			};
	}

	// FILE

	_file_back_to_map_editor = std::make_shared<OptionWithIcon>(
		L"Back to map editor", 
		textures_manager->getTexture(L"assets\\tex\\main_menu\\new_project.png"), // TO-DO
		textures_manager->getTexture(L"assets\\tex\\main_menu\\new_project.png"), // TO-DO
		L"Esc"
	);
	
	_file_back_to_map_editor->_onclick_func = []() {
		editor_manager->pop_back();
		prefabs_editor = nullptr;
		};

	_file_exit = std::make_shared<OptionWithIcon>(
		L"Exit",
		textures_manager->getTexture(L"assets\\tex\\main_menu\\exit.png"), // TO-DO
		textures_manager->getTexture(L"assets\\tex\\main_menu\\exit.png") // TO-DO
	);

	_file->addOption(_file_back_to_map_editor);
	_file->addOption(_file_exit);

	// POSITIONING

	setPosition(sf::Vector2i(0, 0));
}

PrefabsEditorMainMenu::~PrefabsEditorMainMenu() {
	
}

void PrefabsEditorMainMenu::handleEvent(const sf::Event& event) {
	MainMenu::handleEvent(event);
}