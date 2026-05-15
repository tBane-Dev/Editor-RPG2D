#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "DebugLog.hpp"

PrefabsEditor::PrefabsEditor() {
	
}

PrefabsEditor::~PrefabsEditor() {
	
}

void PrefabsEditor::createMainMenu() {
	_main_menu = std::make_shared<MainMenu>();
}

void PrefabsEditor::createPalette() {
	_palette = std::make_shared<Palette>();
}

void PrefabsEditor::cursorHover() {
	_main_menu->cursorHover();
	_palette->cursorHover();
}

void PrefabsEditor::handleEvent(const sf::Event& event) {
	_main_menu->handleEvent(event);
	_palette->handleEvent(event);
}

void PrefabsEditor::update() {

	_main_menu->update();
	_palette->update();


}

void PrefabsEditor::draw() {
	
	GUI_manager->setView();
	_main_menu->draw();
	_palette->draw();
}

std::shared_ptr<PrefabsEditor> prefabs_editor = nullptr;