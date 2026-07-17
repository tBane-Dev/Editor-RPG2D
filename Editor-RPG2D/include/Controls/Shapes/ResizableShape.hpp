#pragma once
#include "SFML/Graphics.hpp"
#include "GUIManager.hpp"
#include "Controls/Shapes/EdgePoint.hpp"
#include <functional>

enum class ResizableShapeState { Idle, Moving };
class ResizableShape : public Element{
public:

	sf::IntRect	_rect;
	int _step = 1;
	sf::Vector2i _minSize = sf::Vector2i(0, 0);
	sf::Vector2i _maxSize = sf::Vector2i(0, 0);

	ResizableShapeState _state = ResizableShapeState::Idle;
	sf::Vector2i _offset = sf::Vector2i(0, 0);

	sf::Color _color;
	sf::Color _outlineColor;

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

	sf::Vector2i getSize();
	void setStep(int step);
	void setMinSize(sf::Vector2i minSize);
	void setMaxSize(sf::Vector2i maxSize);
	void setColor(sf::Color color);
	void setOutlineColor(sf::Color outlineColor);

	virtual void generateEdgePoints();
	virtual void setPosition(sf::Vector2i position);
	sf::Vector2i getPosition();
	void resize(sf::Vector2i size);
	virtual void resize(std::shared_ptr<EdgePoint> edgePoint);

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	void drawOnlyEdgePoints();
	void drawOnlyRect();
	virtual void draw();
};