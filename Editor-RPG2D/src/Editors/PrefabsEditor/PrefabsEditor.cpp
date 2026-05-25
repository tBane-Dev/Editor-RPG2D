#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "DebugLog.hpp"
#include "Theme.hpp"
#include "Window.hpp"

PrefabsEditor::PrefabsEditor() {
	_margin = sf::Vector2i(32, 64);

	_title = std::make_unique<sf::Text>(basicFont, L"Prefabs Editor", 28);
	_title->setFillColor(basic_text_color);
	_title->setPosition(sf::Vector2f(32, 48));

}

PrefabsEditor::~PrefabsEditor() {
	
}

void PrefabsEditor::init() {
	createMainMenu();
	createPalette();
	createMainPanel();
	createAnimator();
	createAnimationPanel();
	createColliderPanel();
	createMeshPanel();
}

void PrefabsEditor::createMainMenu() {
	_main_menu = std::make_shared<PrefabsEditorMainMenu>();
}

void PrefabsEditor::createPalette() {
	_palette = std::make_shared<Palette>();
	_object = _palette->_slots->_selectedSlot ? std::dynamic_pointer_cast<GameObject>(_palette->_slots->_selectedSlot->_object) : nullptr;
}

void PrefabsEditor::createMainPanel() {
	_main_panel = std::make_shared<MainPanel>(_margin);
}

void PrefabsEditor::createAnimator() {
	if(_object)
		_animator = std::make_shared<Animator>(_object->getAnimations(), 0.2f);
}

void PrefabsEditor::createAnimationPanel() {
	_animation_panel = std::make_shared<AnimationPanel>(_margin);
}

void PrefabsEditor::createColliderPanel() {
	_collider_panel = std::make_shared<ColliderPanel>(_margin);
}

void PrefabsEditor::createMeshPanel() {
	_mesh_panel = std::make_shared<MeshPanel>(_margin);
}

void PrefabsEditor::cursorHover() {
	_main_menu->cursorHover();
	_palette->cursorHover();

	_main_panel->cursorHover();
	_animation_panel->cursorHover();
	_collider_panel->cursorHover();
	_mesh_panel->cursorHover();

}

void PrefabsEditor::handleEvent(const sf::Event& event) {
	_main_menu->handleEvent(event);
	_palette->handleEvent(event);

	_main_panel->handleEvent(event);
	_animation_panel->handleEvent(event);
	_collider_panel->handleEvent(event);
	_mesh_panel->handleEvent(event);

}

void PrefabsEditor::update() {

	_main_menu->update();
	_palette->update();

	_main_panel->update();
	_animation_panel->update();
	_collider_panel->update();
	_mesh_panel->update();

}

void PrefabsEditor::draw() {
	
	GUI_manager->setView();

	window->draw(*_title);

	_main_panel->draw();
	_animation_panel->draw();
	_collider_panel->draw();
	_mesh_panel->draw();

	_palette->draw();
	_main_menu->draw();
}

std::shared_ptr<PrefabsEditor> prefabs_editor = nullptr;