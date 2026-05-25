#pragma once
#include "Editors/Editor.hpp"
#include "Editors/AnimationsEditor/AnimationsEditorMainMenu.hpp"
#include "Components/Palette/Palette.hpp"

class AnimationsEditor : public Editor {
public:

	std::unique_ptr<sf::Text> _title;
	sf::Vector2i _margin;

	std::shared_ptr<AnimationsEditorMainMenu> _main_menu;
	
	AnimationsEditor();
	~AnimationsEditor();

	void init();
	void createMainMenu();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};

extern std::shared_ptr<AnimationsEditor> animations_editor;