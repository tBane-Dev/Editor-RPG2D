#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "DebugLog.hpp"
#include "Theme.hpp"
#include "Window.hpp"

PrefabsEditor::PrefabsEditor() {
	_margin = sf::Vector2i(32, 64);

	_title = std::make_unique<sf::Text>(basicFont, L"Prefabs Editor", 28);
	_title->setFillColor(basic_text_color);
	_title->setPosition(sf::Vector2f(32, 48));

	_object = prefabs_manager->getPrefab(L"golem");
}

PrefabsEditor::~PrefabsEditor() {
	
}

void PrefabsEditor::init() {
	createMainMenu();
	createPalette();
	createMainPanel();
	createAnimationPanel();
	createColliderPanel();
	createMeshPanel();
	createButtons();
}

void PrefabsEditor::createMainMenu() {
	_main_menu = std::make_shared<PrefabsEditorMainMenu>();
}

void PrefabsEditor::createPalette() {
	_palette = std::make_shared<Palette>();
}

void PrefabsEditor::createMainPanel() {
	_main_panel = std::make_shared<MainPanel>(_margin);
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

void PrefabsEditor::createButtons() {

	sf::Vector2i startPosition;
	startPosition.x = _main_panel->getPosition().x;
	startPosition.y = _margin.y + _main_panel->getPosition().y + _main_panel->getSize().y - 32 - 16;
	int distance_between_buttons = 32;

	_add_prefab = std::make_shared<ButtonWithTextAndSprite>(
		L"Add prefab",
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\bottomButton.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\bottomButton_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\bottomButton_press.png"),
		sf::Vector2i(startPosition.x, startPosition.y)
	);

	_duplicate_prefab = std::make_shared<ButtonWithTextAndSprite>(
		L"Duplicate prefab",
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\bottomButton.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\bottomButton_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\bottomButton_press.png"),
		sf::Vector2i(startPosition.x + _add_prefab->getSize().x + distance_between_buttons, startPosition.y)
	);

	_remove_prefab = std::make_shared<ButtonWithTextAndSprite>(
		L"Remove prefab",
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\bottomButton.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\bottomButton_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\bottomButton_press.png"),
		sf::Vector2i(_duplicate_prefab->getPosition().x + _duplicate_prefab->getSize().x + distance_between_buttons, startPosition.y)
	);
}

void PrefabsEditor::cursorHover() {
	_main_menu->cursorHover();
	_palette->cursorHover();

	_main_panel->cursorHover();
	_animation_panel->cursorHover();
	_collider_panel->cursorHover();
	_mesh_panel->cursorHover();

	_add_prefab->cursorHover();
	_duplicate_prefab->cursorHover();
	_remove_prefab->cursorHover();
}

void PrefabsEditor::handleEvent(const sf::Event& event) {
	_main_menu->handleEvent(event);
	_palette->handleEvent(event);

	_main_panel->handleEvent(event);
	_animation_panel->handleEvent(event);
	_collider_panel->handleEvent(event);
	_mesh_panel->handleEvent(event);

	_add_prefab->handleEvent(event);
	_duplicate_prefab->handleEvent(event);
	_remove_prefab->handleEvent(event);
}

void PrefabsEditor::update() {

	_main_menu->update();
	_palette->update();

	_main_panel->update();
	_animation_panel->update();
	_collider_panel->update();
	_mesh_panel->update();

	_add_prefab->update();
	_duplicate_prefab->update();
	_remove_prefab->update();
}

void PrefabsEditor::draw() {
	
	GUI_manager->setView();

	window->draw(*_title);

	_main_panel->draw();
	_animation_panel->draw();
	_collider_panel->draw();
	_mesh_panel->draw();

	_add_prefab->draw();
	_duplicate_prefab->draw();
	_remove_prefab->draw();

	_palette->draw();
	_main_menu->draw();
}

std::shared_ptr<PrefabsEditor> prefabs_editor = nullptr;