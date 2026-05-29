#pragma once
#include "Components/MainMenu/MainMenu.hpp"
#include "Controls/OptionWithIcon.hpp"

namespace PrefabsEditor {
	class MainMenu : public Components::MainMenu {
	public:

		std::shared_ptr<Components::MenuButton> _file;
		std::shared_ptr<Components::MenuButton> _tools;
		std::shared_ptr<Components::MenuButton> _settings;

		std::shared_ptr<OptionWithIcon> _file_back_to_map_editor;
		std::shared_ptr<OptionWithIcon> _file_exit;

		MainMenu();
		~MainMenu();

		void handleEvent(const sf::Event& event);
	};
}