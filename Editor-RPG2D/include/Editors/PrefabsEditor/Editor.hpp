#pragma once
#include "EditorsManager.hpp"
#include "Editors/PrefabsEditor/MainMenu.hpp"
#include "Editors/PrefabsEditor/Palette.hpp"
#include "Editors/PrefabsEditor/MainPanel.hpp"
#include "Editors/PrefabsEditor/AnimationPanel.hpp"
#include "Editors/PrefabsEditor/ColliderPanel.hpp"
#include "Editors/PrefabsEditor/MeshPanel.hpp"
#include "PrefabsManager.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"
#include "Objects/Mesh.hpp"

namespace PrefabsEditor {

	class Editor : public Main::Editor {
	public:

		std::unique_ptr<sf::Text> _title;
		sf::Vector2i _margin;

		std::shared_ptr<MainMenu> _main_menu;
		std::shared_ptr<Palette> _palette;

		std::weak_ptr<GameObject> _object;
		std::shared_ptr<Animator> _animator;
		std::shared_ptr<Collider> _collider;
		std::shared_ptr<Mesh> _mesh;

		std::shared_ptr<MainPanel> _main_panel;
		std::shared_ptr<AnimationPanel> _animation_panel;
		std::shared_ptr<ColliderPanel> _collider_panel;
		std::shared_ptr<MeshPanel> _mesh_panel;

		Editor();
		~Editor();

		void init();
		void createMainMenu();
		void createPalette();
		void createMainPanel();
		void createAnimator();
		void createAnimationPanel();
		void createColliderPanel();
		void createMeshPanel();


		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};

	extern std::shared_ptr<Editor> editor;

}