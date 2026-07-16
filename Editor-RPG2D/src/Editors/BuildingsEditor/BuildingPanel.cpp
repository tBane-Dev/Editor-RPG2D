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

		sf::Vector2i buildingSize = sf::Vector2i(9*32, 9*32);

		_buildingShape = std::make_shared<ResizableShape>();
		_buildingShape->resize(buildingSize);
		_buildingShape->setColor(sf::Color(79, 79, 79));
		_buildingShape->setOutlineColor(sf::Color(47, 47, 47));
		_buildingShape->setPosition(sf::Vector2i(_rect.position.x + (_rect.size.x - _buildingShape->getSize().x) / 2, _rect.position.y + (_rect.size.y - _buildingShape->getSize().y) / 2));
		_buildingShape->setStep(32);
		_buildingShape->setMinSize(sf::Vector2i(7 * 32, 7 * 32));
		_buildingShape->setMaxSize(sf::Vector2i(19 * 32, 19 * 32));
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

		_buildingShape->drawOnlyRect();

		grid2_shader->setUniform("rectPosition", sf::Vector2f(_buildingShape->getPosition()));
		grid2_shader->setUniform("rectSize", sf::Vector2f(_buildingShape->getSize()));
		grid2_shader->setUniform("gridSize", sf::Vector2f(32.f, 32.f));
		grid2_shader->setUniform("chunkSize", sf::Vector2f(32.f, 32.f));
		grid2_shader->setUniform("gridWidth", 2.f);
		grid2_shader->setUniform("mainGridMultiplier", 1.f);
		grid2_shader->setUniform("gridMainColor", sf::Glsl::Vec4(sf::Color(127, 7, 7, 255)));
		grid2_shader->setUniform("gridSecondColor", sf::Glsl::Vec4(sf::Color(47, 47, 47, 255)));

		sf::RenderStates rs;
		rs.shader = &*grid2_shader;

		sf::RectangleShape gridRect(sf::Vector2f(_buildingShape->getSize()));
		gridRect.setPosition(sf::Vector2f(_buildingShape->getPosition()));
		gridRect.setFillColor(sf::Color(127, 47, 47, 127));
		Main::render_window->draw(gridRect, rs);
		
		_buildingShape->drawOnlyEdgePoints();

	}
}
