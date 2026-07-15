#include "Editors/BuildingsEditor/BuildingPanel.hpp"
#include "RenderWindow.hpp"
#include "Editors/BuildingsEditor/BuildingsListPanel.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/MainMenu.hpp"

namespace BuildingsEditor {

	BuildingPanel::BuildingPanel(sf::Vector2i margin) : Components::Panel(
		sf::Vector2i(Main::render_window->getSize().x - BuildingsEditor::editor->_list_panel->getSize().x - BuildingsEditor::editor->_palette->getSize().x - margin.x - 8, 900),
		sf::Vector2i(420 + margin.x, BuildingsEditor::editor->_main_menu->getSize().y + margin.y)) {

	}

	BuildingPanel::~BuildingPanel() {

	}

}
