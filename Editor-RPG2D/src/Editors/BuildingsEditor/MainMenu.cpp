#include "Editors/BuildingsEditor/MainMenu.hpp"
#include "Components/MainMenu/MenuButton.hpp"
#include "Controls/OptionWithIcon.hpp"
#include "EditorsManager.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"

namespace BuildingsEditor {

	MainMenu::MainMenu() : Components::MainMenu() {
		_file = std::make_shared<Components::MenuButton>(std::make_shared<MainMenu>(*this), L"File");
		_render = std::make_shared<Components::MenuButton>(std::make_shared<MainMenu>(*this), L"Render");
		_settings = std::make_shared<Components::MenuButton>(std::make_shared<MainMenu>(*this), L"Settings");

		_menu_boxes.push_back(_file);
		_menu_boxes.push_back(_render);
		_menu_boxes.push_back(_settings);


		for (auto& mb : _menu_boxes) {
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
			Main::editor_manager->pop_back();
			BuildingsEditor::editor = nullptr;
			};

		_file_exit = std::make_shared<OptionWithIcon>(
			L"Exit",
			textures_manager->getTexture(L"assets\\tex\\main_menu\\exit.png"), // TO-DO
			textures_manager->getTexture(L"assets\\tex\\main_menu\\exit.png") // TO-DO
		);

		_file->addOption(_file_back_to_map_editor);
		_file->addOption(_file_exit);


		// RENDER

		_render_grid = std::make_shared<OptionWithCheckbox>(L"Grid", textures_manager->getTexture(L"assets\\tex\\unchecked.png"), textures_manager->getTexture(L"assets\\tex\\unchecked_hover.png"));
		_render_grid->addValue(textures_manager->getTexture(L"assets\\tex\\checked.png"), textures_manager->getTexture(L"assets\\tex\\checked_hover.png"));
		_render_grid->_checkbox->setValue(1);

		_render->addOption(_render_grid);

		// POSITIONING

		setPosition(sf::Vector2i(0, 0));
	}

	MainMenu::~MainMenu() {

	}

	void MainMenu::handleEvent(const sf::Event& event) {
		Components::MainMenu::handleEvent(event);

	}
}