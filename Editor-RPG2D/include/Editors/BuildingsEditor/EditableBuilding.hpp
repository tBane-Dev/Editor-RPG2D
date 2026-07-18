#pragma once
#include "Controls/Shapes/ResizableShape.hpp"
#include "TexturesManager.hpp"
#include "Objects/Wall.hpp"

namespace BuildingsEditor {

	enum class EditableBuildingStates { Idle, Moving, Resizing };
	enum class EditableBuildingEditStates { None, Floor, GameObject };

	class EditableBuilding : public ResizableShape {
	public:

		EditableBuildingStates _state = EditableBuildingStates::Idle;
		EditableBuildingEditStates _editState = EditableBuildingEditStates::None;

		std::shared_ptr<Texture> _floorset;

		std::vector<int> _floor;
		sf::Vector2i _floorSize;
		sf::VertexArray _floorVertexArray;

		std::vector<int> _walls;
		sf::Vector2i _wallsSize;
		std::vector<std::shared_ptr<Wall>> _wallsObjects;

		EditableBuilding();
		~EditableBuilding();

		void create();
		void createFloor();
		void resizeFloor(int offsetX, int offsetY);
		void generateFloorVertexArray();

		void createWalls();
		void resizeWalls(int offsetX, int offsetY);
		void generateWalls();

		virtual void resize(std::shared_ptr<EdgePoint> edgePoint);
		void moveFloor(sf::Vector2i offset);
		void moveWalls(sf::Vector2i offset);
		void editTileUnderCursor();
		void addWallUnderCursor();
		void removeWallUnderCursor();

		virtual void cursorHover();
		virtual void handleEvent(const sf::Event& event);
		virtual void update();
		void drawOnlyShape();
		void drawOnlyFloor();
		void drawOnlyWalls();
		void drawOnlyEdgePoints();
		virtual void draw();
	};
}