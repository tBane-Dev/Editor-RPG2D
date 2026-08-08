#pragma once
#include "Components/Panel.hpp"
#include "Editors/BuildingsEditor/EditableBuilding.hpp"
#include "TexturesManager.hpp"
#include "Editors/BuildingsEditor/CursorOnBuilding.hpp"

namespace BuildingsEditor {

	enum class BuildingState {
		Idle,
		Moving
	};

	class BuildingPanel : public Components::Panel {
	public:

		std::shared_ptr<BuildingPrefab> _buildingPrefab;
		std::shared_ptr<BuildingPrefab> _editablePrefab;
		std::shared_ptr<EditableBuilding> _building;
		std::shared_ptr<CursorOnBuilding> _cursorOnBuilding;

		BuildingPanel(sf::Vector2i margin);
		~BuildingPanel();
		void init();

		virtual void cursorHover();
		virtual void handleEvent(const sf::Event& event);
		virtual void update();
		virtual void draw();
	};

}