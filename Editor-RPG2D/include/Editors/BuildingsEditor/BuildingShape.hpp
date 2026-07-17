#pragma once
#include "Controls/Shapes/ResizableShape.hpp"
#include "TexturesManager.hpp"

namespace BuildingsEditor {

	enum class BuildingEditStates { Idle, EditingFloor, Moving, Resizing };

	class BuildingShape : public ResizableShape {
	public:

		BuildingEditStates _state = BuildingEditStates::Idle;
		std::shared_ptr<Texture> _floorset;

		std::vector<int> _floor;
		sf::Vector2i _floorSize;
		sf::VertexArray _floorVertexArray;


		BuildingShape();
		~BuildingShape();

		void create();
		void createFloor();
		void resizeFloor(int offsetX, int offsetY);
		void generateFloorVertexArray();

		virtual void resize(std::shared_ptr<EdgePoint> edgePoint);
		void moveFloor(sf::Vector2i offset);

		virtual void cursorHover();
		virtual void handleEvent(const sf::Event& event);
		virtual void update();
		void drawOnlyShape();
		void drawOnlyFloor();
		void drawOnlyEdgePoints();
		virtual void draw();
	};
}