#include "Controls/Shapes/ResizableShape.hpp"
#include "Cursor.hpp"

ResizableShape::ResizableShape() : Element() {

	_rect = sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(512, 512));
	generateEdgePoints();
}

ResizableShape::~ResizableShape() {

}

void ResizableShape::generateEdgePoints() {

	float m = edgePointSize / 2.0;

	_edgePoints.clear();

	_point_left_top = std::make_shared<EdgePoint>(EdgePointType::LeftTop, sf::Vector2i(_rect.position) + sf::Vector2i((int)(-m), (int)(-m)));
	_point_top = std::make_shared<EdgePoint>(EdgePointType::Top, sf::Vector2i(_rect.position) + sf::Vector2i((int)(_rect.size.x / 2), (int)(-m)));
	_point_right_top = std::make_shared<EdgePoint>(EdgePointType::RightTop, sf::Vector2i(_rect.position) + sf::Vector2i((int)(_rect.size.x + m), (int)(-m)));

	_point_left = std::make_shared<EdgePoint>(EdgePointType::Left, sf::Vector2i(_rect.position) + sf::Vector2i((int)(-m), (int)(_rect.size.y / 2)));
	_point_right = std::make_shared<EdgePoint>(EdgePointType::Right, sf::Vector2i(_rect.position) + sf::Vector2i((int)(_rect.size.x + m), (int)(_rect.size.y / 2)));

	_point_left_bottom = std::make_shared<EdgePoint>(EdgePointType::LeftBottom, sf::Vector2i(_rect.position) + sf::Vector2i((int)(-m), (int)(_rect.size.y + m)));
	_point_bottom = std::make_shared<EdgePoint>(EdgePointType::Bottom, sf::Vector2i(_rect.position) + sf::Vector2i((int)(_rect.size.x / 2), (int)(_rect.size.y + m)));
	_point_right_bottom = std::make_shared<EdgePoint>(EdgePointType::RightBottom, sf::Vector2i(_rect.position) + sf::Vector2i((int)(_rect.size.x + m), (int)(_rect.size.y + m)));

	_edgePoints.push_back(_point_left_top);
	_edgePoints.push_back(_point_top);
	_edgePoints.push_back(_point_right_top);
	_edgePoints.push_back(_point_left);
	_edgePoints.push_back(_point_right);
	_edgePoints.push_back(_point_left_bottom);
	_edgePoints.push_back(_point_bottom);
	_edgePoints.push_back(_point_right_bottom);

}

sf::Vector2i ResizableShape::getSize() {
	return _rect.size;
}

void ResizableShape::setPosition(sf::Vector2i position) {
	_rect.position = position;

	_point_left_top->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i((int)(-edgePointSize / 2), (int)(-edgePointSize / 2)));
	_point_top->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i((int)(_rect.size.x / 2), (int)(-edgePointSize / 2)));
	_point_right_top->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i((int)(_rect.size.x + edgePointSize / 2), (int)(-edgePointSize / 2)));
	_point_left->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i((int)(-edgePointSize / 2), (int)(_rect.size.y / 2)));
	_point_right->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i((int)(_rect.size.x + edgePointSize / 2), (int)(_rect.size.y / 2)));
	_point_left_bottom->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i((int)(-edgePointSize / 2), (int)(_rect.size.y + edgePointSize / 2)));
	_point_bottom->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i((int)(_rect.size.x / 2), (int)(_rect.size.y + edgePointSize / 2)));
	_point_right_bottom->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i((int)(_rect.size.x + edgePointSize / 2), (int)(_rect.size.y + edgePointSize / 2)));
}

void ResizableShape::resize(std::shared_ptr<EdgePoint> edgePoint) {

	sf::Vector2i p = Main::cursor->_position + _edgePoints[0]->getSize() / 2 - edgePoint->getPosition();

	float minX = _point_left->getPosition().x;
	float minY = _point_top->getPosition().y;
	float maxX = _point_right->getPosition().x;
	float maxY = _point_bottom->getPosition().y;

	if (edgePoint == _point_left_top) {
		minX = _point_left->getPosition().x + p.x;
		minY = _point_top->getPosition().y + p.y;
	}
	else if (edgePoint == _point_right_top) {
		maxX = _point_right->getPosition().x + p.x;
		minY = _point_top->getPosition().y + p.y;
	}
	else if (edgePoint == _point_left_bottom) {
		minX = _point_left->getPosition().x + p.x;
		maxY = _point_bottom->getPosition().y + p.y;
	}
	else if (edgePoint == _point_right_bottom) {
		maxX = _point_right->getPosition().x + p.x;
		maxY = _point_bottom->getPosition().y + p.y;
	}
	else if (edgePoint == _point_left) {
		minX = _point_left->getPosition().x + p.x;
	}
	else if (edgePoint == _point_right) {
		maxX = _point_right->getPosition().x + p.x;
	}
	else if (edgePoint == _point_top) {
		minY = _point_top->getPosition().y + p.y;
	}
	else if (edgePoint == _point_bottom) {
		maxY = _point_bottom->getPosition().y + p.y;
	}

	if (edgePoint == _point_left_top || edgePoint == _point_left || edgePoint == _point_left_bottom)
		minX = std::min(minX, maxX);

	if (edgePoint == _point_right_top || edgePoint == _point_right || edgePoint == _point_right_bottom)
		maxX = std::max(maxX, minX);

	if (edgePoint == _point_left_top || edgePoint == _point_top || edgePoint == _point_right_top)
		minY = std::min(minY, maxY);

	if (edgePoint == _point_left_bottom || edgePoint == _point_bottom || edgePoint == _point_right_bottom)
		maxY = std::max(maxY, minY);

	_point_left_top->setPosition(sf::Vector2i(minX, minY));
	_point_top->setPosition(sf::Vector2i((minX + maxX) / 2, minY));
	_point_right_top->setPosition(sf::Vector2i(maxX, minY));

	_point_left->setPosition(sf::Vector2i(minX, (minY + maxY) / 2));
	_point_right->setPosition(sf::Vector2i(maxX, (minY + maxY) / 2));

	_point_left_bottom->setPosition(sf::Vector2i(minX, maxY));
	_point_bottom->setPosition(sf::Vector2i((minX + maxX) / 2, maxY));
	_point_right_bottom->setPosition(sf::Vector2i(maxX, maxY));

	_rect = sf::IntRect(sf::Vector2i(minX, minY), sf::Vector2i(maxX - minX, maxY - minY));
}

void ResizableShape::cursorHover() {

	if (_rect.contains(Main::cursor->_position)) {
		GUI_manager->Element_hovered = this->shared_from_this();
	}

	for (auto& point : _edgePoints) {
		point->cursorHover();
	}
}

void ResizableShape::handleEvent(const sf::Event& event) {

	for (auto& point : _edgePoints) {
		point->handleEvent(event);

		if(point == GUI_manager->Element_pressed) {
			resize(point);
		}
	}
}

void ResizableShape::update() {
	for (auto& point : _edgePoints) {
		point->update();
	}
}

void ResizableShape::draw() {
	float border = 2.0f;
	sf::RectangleShape rect(sf::Vector2f(_rect.size.x - 2 * border, _rect.size.y - 2 * border));
	rect.setFillColor(sf::Color(191, 0, 0));
	rect.setOutlineThickness(border);
	rect.setOutlineColor(sf::Color(127, 0, 0));
	rect.setPosition(sf::Vector2f(_rect.position.x + border, _rect.position.y + border));
	Main::render_window->draw(rect);

	for(auto& point : _edgePoints) {
		point->draw();
	}
}
