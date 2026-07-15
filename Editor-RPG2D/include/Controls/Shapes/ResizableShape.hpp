#pragma once
#include "SFML/Graphics.hpp"
#include "GUIManager.hpp"
#include "Controls/Shapes/EdgePoint.hpp"


class ResizableShape : public Element{
public:

	sf::IntRect	_rect;

	// edge points
	float edgePointSize = 16.0f;
	std::vector<std::shared_ptr<EdgePoint>> _edgePoints;
	std::shared_ptr<EdgePoint> _point_left_top;
	std::shared_ptr<EdgePoint> _point_top;
	std::shared_ptr<EdgePoint> _point_right_top;
	std::shared_ptr<EdgePoint> _point_left;
	std::shared_ptr<EdgePoint> _point_right;
	std::shared_ptr<EdgePoint> _point_left_bottom;
	std::shared_ptr<EdgePoint> _point_bottom;
	std::shared_ptr<EdgePoint> _point_right_bottom;
	

	ResizableShape();
	~ResizableShape();

	void generateEdgePoints();

	sf::Vector2i getSize();
	virtual void setPosition(sf::Vector2i position);
	void resize(std::shared_ptr<EdgePoint> edgePoint);

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};