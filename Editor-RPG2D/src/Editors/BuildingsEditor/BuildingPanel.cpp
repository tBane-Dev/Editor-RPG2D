#include "Editors/BuildingsEditor/BuildingPanel.hpp"
#include "RenderWindow.hpp"
#include "Editors/BuildingsEditor/BuildingsListPanel.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/NamePanel.hpp"

namespace BuildingsEditor {

	BuildingPanel::BuildingPanel(sf::Vector2i margin) : Components::Panel(
		sf::Vector2i(BuildingsEditor::editor->_name_panel->getSize().x + 4, 768 + 4),
		sf::Vector2i(BuildingsEditor::editor->_name_panel->getPosition().x, BuildingsEditor::editor->_name_panel->getPosition().y + BuildingsEditor::editor->_name_panel->getSize().y + 4)) {


		_buildingShape = std::make_shared<ResizableShape>();
		_buildingShape->setPosition(sf::Vector2i(_rect.position.x + (_rect.size.x - _buildingShape->getSize().x) / 2, _rect.position.y + (_rect.size.y - _buildingShape->getSize().y) / 2));
	}

	BuildingPanel::~BuildingPanel() {

	}

	void BuildingPanel::cursorHover() {
		Components::Panel::cursorHover();
		_buildingShape->cursorHover();
	}

	void BuildingPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);
		_buildingShape->handleEvent(event);
	}

	void BuildingPanel::update() {
		Components::Panel::update();
		_buildingShape->update();
	}

	void BuildingPanel::draw() {
		Components::Panel::draw();

		_buildingShape->draw();
	}
}
