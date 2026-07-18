#include "Controls/Shapes/ResizableShape.hpp"
#include "Cursor.hpp"

ResizableShape::ResizableShape() : Element() {

	int size = 512;

	_rect = sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(size, size));
	_state = ResizableShapeState::Idle;
	_step = 1;

	sf::Vector2i _minSize = sf::Vector2i(0, 0);
	sf::Vector2i _maxSize = sf::Vector2i(size, size);

	_state = ResizableShapeState::Idle;
	_offset = sf::Vector2i(0, 0);

	_color = sf::Color(191, 0, 0);
	_outlineColor = sf::Color(191, 0, 0);

	generateEdgePoints();

}

ResizableShape::~ResizableShape() {

}

sf::Vector2i ResizableShape::getSize() {
	return sf::Vector2i((float)_rect.size.x * _scale, (float)_rect.size.y * _scale);
}

void ResizableShape::setStep(int step) {
	_step = step;
}

void ResizableShape::setMinSize(sf::Vector2i minSize) {
	_minSize = minSize;
}

void ResizableShape::setMaxSize(sf::Vector2i maxSize) {
	_maxSize = maxSize;
}

void ResizableShape::setColor(sf::Color color) {
	_color = color;
}

void ResizableShape::setOutlineColor(sf::Color outlineColor) {
	_outlineColor = outlineColor;

}
void ResizableShape::generateEdgePoints() {

	_edgePoints.clear();

	_point_left_top = std::make_shared<EdgePoint>(EdgePointType::LeftTop, sf::Vector2i(_rect.position));
	_point_top = std::make_shared<EdgePoint>(EdgePointType::Top, sf::Vector2i(_rect.position) + sf::Vector2i(getSize().x / 2, 0));
	_point_right_top = std::make_shared<EdgePoint>(EdgePointType::RightTop, sf::Vector2i(_rect.position) + sf::Vector2i(getSize().x, 0));

	_point_left = std::make_shared<EdgePoint>(EdgePointType::Left, sf::Vector2i(_rect.position) + sf::Vector2i(0, getSize().y / 2));
	_point_right = std::make_shared<EdgePoint>(EdgePointType::Right, sf::Vector2i(_rect.position) + sf::Vector2i(getSize().x, getSize().y / 2));

	_point_left_bottom = std::make_shared<EdgePoint>(EdgePointType::LeftBottom, sf::Vector2i(_rect.position) + sf::Vector2i(0, getSize().y));
	_point_bottom = std::make_shared<EdgePoint>(EdgePointType::Bottom, sf::Vector2i(_rect.position) + sf::Vector2i(getSize().x / 2, getSize().y));
	_point_right_bottom = std::make_shared<EdgePoint>(EdgePointType::RightBottom, sf::Vector2i(_rect.position) + sf::Vector2i(getSize().x, getSize().y));

	_edgePoints.push_back(_point_left_top);
	_edgePoints.push_back(_point_top);
	_edgePoints.push_back(_point_right_top);
	_edgePoints.push_back(_point_left);
	_edgePoints.push_back(_point_right);
	_edgePoints.push_back(_point_left_bottom);
	_edgePoints.push_back(_point_bottom);
	_edgePoints.push_back(_point_right_bottom);

}

void ResizableShape::setPosition(sf::Vector2i position) {

	_rect.position = position;

	_point_left_top->setPosition(sf::Vector2i(_rect.position));
	_point_top->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i(getSize().x / 2, 0));
	_point_right_top->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i(getSize().x, 0));
	_point_left->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i(0, getSize().y / 2));
	_point_right->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i(getSize().x, getSize().y / 2));
	_point_left_bottom->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i(0, getSize().y));
	_point_bottom->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i(getSize().x / 2, getSize().y));
	_point_right_bottom->setPosition(sf::Vector2i(_rect.position) + sf::Vector2i(getSize().x, getSize().y));
}

sf::Vector2i ResizableShape::getPosition() {
	return _rect.position;
}

void ResizableShape::resize(sf::Vector2i size) {
	_rect.size = size;
}

void ResizableShape::resize(std::shared_ptr<EdgePoint> edgePoint) {

	if (!edgePoint)
		return;

	sf::Vector2i p = Main::cursor->_position - edgePoint->getPosition();
	p = p / _step * _step;

	int minX = _point_left->getPosition().x;
	int minY = _point_top->getPosition().y;
	int maxX = _point_right->getPosition().x;
	int maxY = _point_bottom->getPosition().y;

	bool resizeLeft =
		edgePoint == _point_left_top ||
		edgePoint == _point_left ||
		edgePoint == _point_left_bottom;

	bool resizeRight =
		edgePoint == _point_right_top ||
		edgePoint == _point_right ||
		edgePoint == _point_right_bottom;

	bool resizeTop =
		edgePoint == _point_left_top ||
		edgePoint == _point_top ||
		edgePoint == _point_right_top;

	bool resizeBottom =
		edgePoint == _point_left_bottom ||
		edgePoint == _point_bottom ||
		edgePoint == _point_right_bottom;

	if (resizeLeft)
		minX += p.x;
	else if (resizeRight)
		maxX += p.x;

	if (resizeTop)
		minY += p.y;
	else if (resizeBottom)
		maxY += p.y;

	if (resizeLeft) {
		int width = std::clamp(maxX - minX, _minSize.x, _maxSize.x);
		minX = maxX - width;
	}
	else if (resizeRight) {
		int width = std::clamp(maxX - minX, _minSize.x, _maxSize.x);
		maxX = minX + width;
	}

	if (resizeTop) {
		int height = std::clamp(maxY - minY, _minSize.y, _maxSize.y);
		minY = maxY - height;
	}
	else if (resizeBottom) {
		int height = std::clamp(maxY - minY, _minSize.y, _maxSize.y);
		maxY = minY + height;
	}

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

	sf::IntRect rect = sf::IntRect(getPosition(), getSize());

	if (rect.contains(Main::cursor->_position)) {
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
			return;
		}
	}

	if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Middle) {
		if(GUI_manager->Element_hovered.get() == this) {
			_state = ResizableShapeState::Moving;
			_offset = Main::cursor->_position - getPosition();
			GUI_manager->Element_pressed = nullptr;
		}
	}

	
	if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Middle) {
		_state = ResizableShapeState::Idle;
		_offset = sf::Vector2i(0, 0);
		if(GUI_manager->Element_pressed.get() == this)
			GUI_manager->Element_pressed = nullptr;
	}
}

void ResizableShape::update() {

	if(_state == ResizableShapeState::Moving) {
		setPosition(Main::cursor->_position - _offset);

		return;
	}

	for (auto& point : _edgePoints) {
		point->update();
	}
}

void ResizableShape::drawOnlyEdgePoints() {
	for (auto& point : _edgePoints) {
		point->draw(); 
	}
}

void ResizableShape::drawOnlyRect() {
	float border = 2.0f;
	sf::RectangleShape rect(sf::Vector2f(getSize().x - 2 * border, getSize().y - 2 * border));
	rect.setFillColor(_color);
	rect.setOutlineThickness(border);
	rect.setOutlineColor(_outlineColor);
	rect.setPosition(sf::Vector2f(getPosition().x + border, getPosition().y + border));
	Main::render_window->draw(rect);
} 

void ResizableShape::draw() {
	float border = 2.0f;
	sf::RectangleShape rect(sf::Vector2f(getSize().x - 2 * border, getSize().y - 2 * border));
	rect.setFillColor(_color);
	rect.setOutlineThickness(border);
	rect.setOutlineColor(_outlineColor);
	rect.setPosition(sf::Vector2f(getPosition().x + border, getPosition().y + border));
	Main::render_window->draw(rect);

	for (auto& point : _edgePoints) {
		point->draw();
	}
}

