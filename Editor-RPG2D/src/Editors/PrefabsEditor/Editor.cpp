#include "Editors/PrefabsEditor/Editor.hpp"
#include "Editors/PrefabsEditor/Palette.hpp"
#include "DebugLog.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "WindowsManager.hpp"

namespace PrefabsEditor {

	Editor::Editor() {
		_margin = sf::Vector2i(32, 64);

		_title = std::make_unique<sf::Text>(basicFont, L"Prefabs Editor", 28);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(32, 48));

		_object = std::weak_ptr<GameObject>();
		_animator = nullptr;
		_collider = nullptr;
		_mesh = nullptr;
	}

	Editor::~Editor() {

	}

	void Editor::init() {
		createMainMenu();
		createPalette();
		createMainPanel();
		createAnimator();
		createAnimationPanel();
		createColliderPanel();
		createMeshPanel();
		
	}

	void Editor::createMainMenu() {
		_main_menu = std::make_shared<MainMenu>();
	}

	void Editor::createPalette() {
		_palette = std::make_shared<Palette>();
		// TO-DO - must be 
		//_object = _palette->_slots->_selectedSlot ? std::dynamic_pointer_cast<GameObject>(_palette->_slots->_selectedSlot->_object) : nullptr;
	}

	void Editor::createMainPanel() {
		_main_panel = std::make_shared<MainPanel>(_margin);
	}

	void Editor::createAnimator() {
		if (!_object.expired())
			_animator = std::make_shared<Animator>(_object.lock()->getAnimations(), 0.2f);
	}

	void Editor::createAnimationPanel() {
		_animation_panel = std::make_shared<AnimationPanel>(_margin);
	}

	void Editor::createColliderPanel() {
		_collider_panel = std::make_shared<ColliderPanel>(_margin);
	}

	void Editor::createMeshPanel() {
		_mesh_panel = std::make_shared<MeshPanel>(_margin);
	}

	void Editor::cursorHover() {

		if(Main::windows_manager->get_back())
			return;

		if (_main_menu->_state != Components::MainMenuStates::Closed) {
			_main_menu->cursorHover();
			return;
		}

		
		_animation_panel->cursorHover();
		_collider_panel->cursorHover();
		_main_panel->cursorHover();
		_mesh_panel->cursorHover();
		_palette->cursorHover();
		_main_menu->cursorHover();

	}

	void Editor::handleEvent(const sf::Event& event) {

		if (Main::windows_manager->get_back())
			return;

		_main_menu->handleEvent(event);

		if (_main_menu->_state != Components::MainMenuStates::Closed) {
			return;
		}

		_palette->handleEvent(event);

		_main_panel->handleEvent(event);
		_animation_panel->handleEvent(event);
		_collider_panel->handleEvent(event);
		_mesh_panel->handleEvent(event);

	}

	void Editor::update() {

		_main_menu->update();
		_palette->update();

		_main_panel->update();
		_animation_panel->update();
		_collider_panel->update();
		_mesh_panel->update();

	}

	void Editor::draw() {

		GUI_manager->setView();

		Main::render_window->draw(*_title);

		_main_panel->draw();
		_animation_panel->draw();
		_collider_panel->draw();
		_mesh_panel->draw();

		_palette->draw();
		_main_menu->draw();
	}

	std::shared_ptr<Editor> editor = nullptr;
}