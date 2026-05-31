#pragma once
#include "Editors/Editor.hpp"
#include "Editors/AnimationsEditor/MainMenu.hpp"
#include "Components/Panel.hpp"
#include "AnimationsManager.hpp"
#include "Animator.hpp"
#include "Editors/AnimationsEditor/NamePanel.hpp"
#include "Editors/AnimationsEditor/ListPanel.hpp"
#include "Editors/AnimationsEditor/ActionsPanel.hpp"
#include "Editors/AnimationsEditor/SpriteSheetPanel.hpp"
#include "Editors/AnimationsEditor/PreviewPanel.hpp"

namespace AnimationsEditor {
	class Editor : public Main::Editor {
	public:

		std::unique_ptr<sf::Text> _title;
		sf::Vector2i _margin;

		std::shared_ptr<MainMenu> _main_menu;

		std::shared_ptr<Animations> _animations;
		std::shared_ptr<Animator> _animator;

		std::shared_ptr<NamePanel> _animation_name_panel;
		std::shared_ptr<ListPanel> _list_panel;
		std::shared_ptr<ActionsPanel> _actions_panel;
		std::shared_ptr<SpriteSheetPanel> _sprite_sheet_panel;
		std::shared_ptr<PreviewPanel> _preview_panel;

		Editor();
		~Editor();

		void init();
		void createMainMenu();
		void createListPanel();
		void createActionsPanel();
		void createNamePanel();
		void createSpriteSheetPanel();
		void createPreviewPanel();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};

	extern std::shared_ptr<Editor> editor;
}