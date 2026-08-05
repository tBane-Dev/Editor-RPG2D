#pragma once
#include "Controls/Shapes/ResizableShape.hpp"
#include "TexturesManager.hpp"
#include "Objects/Building/Building.hpp"
#include "Objects/Wall.hpp"

namespace BuildingsEditor {

	enum class EditableBuildingStates { Idle, Moving, Resizing };

	class EditableBuilding : public ResizableShape {
	public:

		std::shared_ptr<Building> _building;
		EditableBuildingStates _state = EditableBuildingStates::Idle;

		EditableBuilding();
		~EditableBuilding();

		void create(std::shared_ptr<BuildingPrefab> prefab);
		void resizeFloor(int offsetX, int offsetY);
		void resizeWalls(int offsetX, int offsetY);
		void resizeRoof();

		virtual void resize(std::shared_ptr<EdgePoint> edgePoint);

		sf::Vector2i clampPosition(sf::Vector2i position);

		virtual void cursorHover();
		virtual void handleEvent(const sf::Event& event);
		virtual void update();
		void drawOnlyShape();
		void drawOnlyFloor();
		void drawOnlyWalls();
		void drawOnlyRoof();
		void drawOnlyEdgePoints();
		virtual void draw();
	};
}