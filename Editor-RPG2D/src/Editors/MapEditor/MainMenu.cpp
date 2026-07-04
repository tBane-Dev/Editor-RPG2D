#include "Editors/MapEditor/MainMenu.hpp"
#include "Components/MainMenu/MenuButton.hpp"
#include "Editors/AnimationsEditor/Editor.hpp"
#include "Editors/PrefabsEditor/Editor.hpp"
#include "WindowsManager.hpp" // TO-DO - to delete
#include "Windows/FileDialog/FileDialog.hpp" // TO-DO - to delete
#include "DebugLog.hpp"

#include <fstream>
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/MapEditor/Map/Map.hpp"

namespace MapEditor {

	MainMenu::MainMenu() : Components::MainMenu() {
		_file = std::make_shared<Components::MenuButton>(std::make_shared<Components::MainMenu>(*this), L"File");
		_render = std::make_shared<Components::MenuButton>(std::make_shared<Components::MainMenu>(*this), L"Render");
		_tools = std::make_shared<Components::MenuButton>(std::make_shared<Components::MainMenu>(*this), L"Tools");
		_settings = std::make_shared<Components::MenuButton>(std::make_shared<Components::MainMenu>(*this), L"Settings");

		_menu_boxes.push_back(_file);
		_menu_boxes.push_back(_render);
		_menu_boxes.push_back(_tools);
		_menu_boxes.push_back(_settings);

		for (auto& mb : _menu_boxes) {
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

		_file_open_project->_onclick_func = [this]() {
			closeMenu();
			std::shared_ptr<FileDialog> load_window = std::make_shared<FileDialog>(L"Open Project");
			std::function<void()> load_function = [load_window]() {
				std::ifstream loader;
				loader.open(load_window->getPathFile(), std::ios::in | std::ios::binary);
				animations_manager->load(loader);
				prefabs_manager->load(loader);
				MapEditor::editor->_map->load(loader);
				MapEditor::editor->_game_objects->load(loader);
				loader.close();

				MapEditor::editor->_palette->_slots->updateObjects();
			};

			load_window->setFunction(load_function);
			Main::windows_manager->push_back(load_window);
			};

		_file_save_project = std::make_shared<OptionWithIcon>(
			L"Save project",
			textures_manager->getTexture(L"assets\\tex\\main_menu\\save_project.png"),
			textures_manager->getTexture(L"assets\\tex\\main_menu\\save_project.png"),
			L"Ctrl+S"
		);

		_file_save_project->_onclick_func = [this]() {
			closeMenu();
			std::shared_ptr<FileDialog> save_window = std::make_shared<FileDialog>(L"Save Project");
			std::function<void()> save_function = [save_window]() {
				std::ofstream saver;
				saver.open(save_window->getPathFile(), std::ios::out | std::ios::trunc | std::ios::binary);
				animations_manager->save(saver);
				prefabs_manager->save(saver);
				MapEditor::editor->_map->save(saver);
				MapEditor::editor->_game_objects->save(saver);
				saver.close();

			};
			save_window->setFunction(save_function);
			Main::windows_manager->push_back(save_window);
		};

		_file_exit = std::make_shared<OptionWithIcon>(
			L"Exit",
			textures_manager->getTexture(L"assets\\tex\\main_menu\\exit.png"), // TO-DO
			textures_manager->getTexture(L"assets\\tex\\main_menu\\exit.png")  // TO-DO
		);

		_file->addOption(_file_new_project);
		_file->addOption(_file_open_project);
		_file->addOption(_file_save_project);
		_file->addOption(_file_exit);

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

		_tool_animations_editor = std::make_shared<OptionWithIcon>(
			L"Animations editor",
			nullptr,
			nullptr
		);
		_tool_animations_editor->_onclick_func = [this]() {
			if (AnimationsEditor::editor == nullptr) {
				closeMenu();
				AnimationsEditor::editor = std::make_shared<AnimationsEditor::Editor>();
				AnimationsEditor::editor->init();
				Main::editor_manager->push_back(AnimationsEditor::editor);
			}
			};

		_tool_prefabs_editor = std::make_shared<OptionWithIcon>(
			L"Prefabs editor",
			nullptr,
			nullptr
		);
		_tool_prefabs_editor->_onclick_func = [this]() {
			if (PrefabsEditor::editor == nullptr) {
				closeMenu();
				PrefabsEditor::editor = std::make_shared<PrefabsEditor::Editor>();
				PrefabsEditor::editor->init();
				Main::editor_manager->push_back(PrefabsEditor::editor);
			}
			};

		_tools->addOption(_tool_palette);
		_tools->addOption(_tool_animations_editor);
		_tools->addOption(_tool_prefabs_editor);

		// POSITIONING

		setPosition(sf::Vector2i(0, 0));
	}

	MainMenu::~MainMenu() {

	}

	void MainMenu::handleEvent(const sf::Event& event) {



		if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp) {
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
			if (kp->code == sf::Keyboard::Key::B) {
				_tool_palette->_checkbox->_value = (_tool_palette->_checkbox->_value == 0) ? 1 : 0;
				return;
			}
		}

		Components::MainMenu::handleEvent(event);

	}
}