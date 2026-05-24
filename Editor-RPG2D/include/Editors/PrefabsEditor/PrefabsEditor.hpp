#pragma once
#include "Editors/Editor.hpp"
#include "Editors/PrefabsEditor/PrefabsEditorMainMenu.hpp"
#include "Components/Palette/Palette.hpp"
#include "Editors/PrefabsEditor/MainPanel.hpp"
#include "Editors/PrefabsEditor/AnimationPanel.hpp"
#include "Editors/PrefabsEditor/ColliderPanel.hpp"
#include "Editors/PrefabsEditor/MeshPanel.hpp"
#include "PrefabsManager.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"

class PrefabsEditor : public Editor {
public:

	std::unique_ptr<sf::Text> _title;
	sf::Vector2i _margin;

	std::shared_ptr<PrefabsEditorMainMenu> _main_menu;
	std::shared_ptr<Palette> _palette;

	std::shared_ptr<GameObject> _object;

	std::shared_ptr<MainPanel> _main_panel;
	std::shared_ptr<AnimationPanel> _animation_panel;
	std::shared_ptr<ColliderPanel> _collider_panel;
	std::shared_ptr<MeshPanel> _mesh_panel;
	
	PrefabsEditor();
	~PrefabsEditor();

	void init();
	void createMainMenu();
	void createPalette();
	void createMainPanel();
	void createAnimationPanel();
	void createColliderPanel();
	void createMeshPanel();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};

extern std::shared_ptr<PrefabsEditor> prefabs_editor;