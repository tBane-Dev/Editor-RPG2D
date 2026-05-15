#pragma once
#include "Editors/Editor.hpp"
#include "Components/MainMenu/MainMenu.hpp"
#include "Components/Palette/Palette.hpp"

class PrefabsEditor : public Editor {
public:
	std::shared_ptr<MainMenu> _main_menu;
	std::shared_ptr<Palette> _palette;

	PrefabsEditor();
	~PrefabsEditor();

	void createMainMenu();
	void createPalette();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};

extern std::shared_ptr<PrefabsEditor> prefabs_editor;