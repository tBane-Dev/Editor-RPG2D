#pragma once
#include "Components/Panel.hpp"
#include "Editors/BuildingsEditor/EditableBuilding.hpp"
#include "TexturesManager.hpp"

namespace BuildingsEditor {

	enum class BuildingState {
		Idle,
		Moving
	};

	class BuildingPanel : public Components::Panel {
	public:

		std::shared_ptr<EditableBuilding> _building;

		BuildingPanel(sf::Vector2i margin);
		~BuildingPanel();

		virtual void cursorHover();
		virtual void handleEvent(const sf::Event& event);
		virtual void update();
		virtual void draw();
	};

}