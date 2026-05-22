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

void PrefabsEditor::createMainMenu() {
	_main_menu = std::make_shared<MainMenu>();
}

void PrefabsEditor::createPalette() {
	_palette = std::make_shared<Palette>();
}

void PrefabsEditor::createMainPanel() {
	_main_panel = std::make_shared<MainPanel>(_margin);
}

void PrefabsEditor::createPreviewPanel() {
	_preview_panel = std::make_shared<PreviewPanel>(_margin);
}

void PrefabsEditor::createColliderPanel() {
	_collider_panel = std::make_shared<ColliderPanel>(_margin);
}

void PrefabsEditor::cursorHover() {
	_main_menu->cursorHover();
	_palette->cursorHover();

	_main_panel->cursorHover();
	_preview_panel->cursorHover();
	_collider_panel->cursorHover();
}

void PrefabsEditor::handleEvent(const sf::Event& event) {
	_main_menu->handleEvent(event);
	_palette->handleEvent(event);

	_main_panel->handleEvent(event);
	_preview_panel->handleEvent(event);
	_collider_panel->handleEvent(event);
}

void PrefabsEditor::update() {

	_main_menu->update();
	_palette->update();

	_main_panel->update();
	_preview_panel->update();
	_collider_panel->update();
}

void PrefabsEditor::draw() {
	
	GUI_manager->setView();

	window->draw(*_title);

	_main_menu->draw();
	_palette->draw();

	_main_panel->draw();
	_preview_panel->draw();
	_collider_panel->draw();
}

std::shared_ptr<PrefabsEditor> prefabs_editor = nullptr;