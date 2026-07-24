#include "Editors/BuildingsEditor/BuildingPanel.hpp"
#include "RenderWindow.hpp"
#include "Editors/BuildingsEditor/BuildingsListPanel.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/NamePanel.hpp"
#include "ShadersManager.hpp"

namespace BuildingsEditor {

	BuildingPanel::BuildingPanel(sf::Vector2i margin) : Components::Panel(
		sf::Vector2i(BuildingsEditor::editor->_name_panel->getSize().x + 4, 768 + 4),
		sf::Vector2i(BuildingsEditor::editor->_name_panel->getPosition().x, BuildingsEditor::editor->_name_panel->getPosition().y + BuildingsEditor::editor->_name_panel->getSize().y + 4)) {

		_building = std::make_shared<EditableBuilding>();
		_cursorOnBuilding = std::make_shared<CursorOnBuilding>();
	}

	BuildingPanel::~BuildingPanel() {

	}

	void BuildingPanel::cursorHover() {
		Components::Panel::cursorHover();
		_building->cursorHover();
	}

	void BuildingPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);
		_building->handleEvent(event);
		_cursorOnBuilding->handleEvent(event);
	}

	void BuildingPanel::update() {
		Components::Panel::update();
		_building->update();
		_cursorOnBuilding->update();
	}

	void BuildingPanel::draw() {
		Components::Panel::draw();
		
		// clamp the view
		sf::FloatRect fr;
		fr.size = sf::Vector2f(BuildingsEditor::editor->_building_panel->_rect.size - sf::Vector2i(4,4));
		fr.position = sf::Vector2f(BuildingsEditor::editor->_building_panel->_rect.position + sf::Vector2i(2,2));

		sf::View view(fr);

		sf::FloatRect vp(
			sf::Vector2f(
				fr.position.x / GUI_manager->_view.getSize().x,
				fr.position.y / GUI_manager->_view.getSize().y
			),

			sf::Vector2f(
				fr.size.x / GUI_manager->_view.getSize().x,
				fr.size.y / GUI_manager->_view.getSize().y
			)
		);

		view.setViewport(vp);
		Main::render_window->setView(view);

		// draw building
		_building->drawOnlyShape();
		_building->drawOnlyFloor();
		_building->drawOnlyWalls();

		// draw grid
		if (BuildingsEditor::editor->_main_menu->_render_grid->_checkbox->_value == 1) {
			grid2_shader->setUniform("rectPosition", sf::Vector2f(_building->getPosition()));
			grid2_shader->setUniform("rectSize", sf::Vector2f(_building->getSize()));
			grid2_shader->setUniform("gridSize", sf::Vector2f(32.f, 32.f) * _building->_scale);
			grid2_shader->setUniform("chunkSize", sf::Vector2f(32.f, 32.f) * _building->_scale);
			grid2_shader->setUniform("gridWidth", 2.f);
			grid2_shader->setUniform("mainGridMultiplier", 1.f);
			grid2_shader->setUniform("gridMainColor", sf::Glsl::Vec4(sf::Color(127, 7, 7, 255)));
			grid2_shader->setUniform("gridSecondColor", sf::Glsl::Vec4(sf::Color(47, 47, 47, 255)));

			sf::RenderStates rs;
			rs.shader = &*grid2_shader;

			sf::RectangleShape gridRect(sf::Vector2f(_building->getSize()));
			gridRect.setPosition(sf::Vector2f(_building->getPosition()));
			gridRect.setFillColor(sf::Color(127, 47, 47, 127));
			Main::render_window->draw(gridRect, rs);
		}
		
		// draw cursor on building
		_cursorOnBuilding->draw();

		// draw edge points
		_building->drawOnlyEdgePoints();

	}
}
