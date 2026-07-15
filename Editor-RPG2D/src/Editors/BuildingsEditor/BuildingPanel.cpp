#include "Editors/BuildingsEditor/BuildingPanel.hpp"
#include "RenderWindow.hpp"
#include "Editors/BuildingsEditor/BuildingsListPanel.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/NamePanel.hpp"

namespace BuildingsEditor {

	BuildingPanel::BuildingPanel(sf::Vector2i margin) : Components::Panel(
		sf::Vector2i(BuildingsEditor::editor->_name_panel->getSize().x + 4, 768),
		sf::Vector2i(BuildingsEditor::editor->_name_panel->getPosition().x, BuildingsEditor::editor->_name_panel->getPosition().y + BuildingsEditor::editor->_name_panel->getSize().y + 4)) {

	}

	BuildingPanel::~BuildingPanel() {

	}

}
