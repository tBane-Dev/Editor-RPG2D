#pragma once
#include "Components/Panel.hpp"
#include "Controls/Shapes/ResizableShape.hpp"

namespace BuildingsEditor {

	enum class BuildingState {
		Idle,
		Moving
	};

	class BuildingPanel : public Components::Panel {
	public:

		std::shared_ptr<ResizableShape> _buildingShape;

		BuildingPanel(sf::Vector2i margin);
		~BuildingPanel();

		virtual void cursorHover();
		virtual void handleEvent(const sf::Event& event);
		virtual void update();
		virtual void draw();
	};

}