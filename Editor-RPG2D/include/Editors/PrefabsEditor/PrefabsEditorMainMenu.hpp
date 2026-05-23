#pragma once
#include "Components/MainMenu/MainMenu.hpp"
#include "Controls/OptionWithIcon.hpp"

class PrefabsEditorMainMenu : public MainMenu {
public:

	std::shared_ptr<MenuButton> _file;
	std::shared_ptr<MenuButton> _tools;
	std::shared_ptr<MenuButton> _settings;

	std::shared_ptr<OptionWithIcon> _file_back_to_map_editor;
	std::shared_ptr<OptionWithIcon> _file_exit;

	PrefabsEditorMainMenu();
	~PrefabsEditorMainMenu();

	void handleEvent(const sf::Event& event);
};