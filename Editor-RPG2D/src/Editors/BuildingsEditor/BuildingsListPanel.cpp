#include "Editors/BuildingsEditor/BuildingsListPanel.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/MainMenu.hpp"

namespace BuildingsEditor {

	BuildingsListPanel::BuildingsListPanel(sf::Vector2i margin) : Components::ListPanel(margin, sf::Vector2i(420, 600), sf::Vector2i(margin.x, BuildingsEditor::editor->_main_menu->getSize().y + margin.y)) {
		_title->setString(L"Buildings List");
		loadAll();
	}

	BuildingsListPanel::~BuildingsListPanel() {

	}

	void BuildingsListPanel::loadList() {
		resetItems();
	}
}
