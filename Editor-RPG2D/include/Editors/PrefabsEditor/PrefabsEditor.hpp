#pragma once
#include "Editors/Editor.hpp"
#include "Editors/PrefabsEditor/PrefabsEditorMainMenu.hpp"
#include "Components/Palette/Palette.hpp"
#include "Editors/PrefabsEditor/MainPanel.hpp"
#include "Editors/PrefabsEditor/PreviewPanel.hpp"
#include "Editors/PrefabsEditor/ColliderPanel.hpp"
#include "PrefabsManager.hpp"

class PrefabsEditor : public Editor {
public:

	std::unique_ptr<sf::Text> _title;
	sf::Vector2i _margin;

	std::shared_ptr<PrefabsEditorMainMenu> _main_menu;
	std::shared_ptr<Palette> _palette;

	std::shared_ptr<GameObject> _object;

	std::shared_ptr<MainPanel> _main_panel;
	std::shared_ptr<PreviewPanel> _preview_panel;
	std::shared_ptr<ColliderPanel> _collider_panel;
	

	PrefabsEditor();
	~PrefabsEditor();

	void createMainMenu();
	void createPalette();
	void createMainPanel();
	void createPreviewPanel();
	void createColliderPanel();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};

extern std::shared_ptr<PrefabsEditor> prefabs_editor;