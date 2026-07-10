#include "Objects/Mesh.hpp"
#include "RenderWindow.hpp"
#include "Theme.hpp"
#include "BinaryWriter.hpp"
#include "BinaryReader.hpp"

bool pointOnSegment(sf::Vector2i p, sf::Vector2i a, sf::Vector2i b)
{
	int cross = 1LL * (b.x - a.x) * (p.y - a.y) - 1LL * (b.y - a.y) * (p.x - a.x);

	if (cross != 0)
		return false;

	int minx = std::min(a.x, b.x);
	int maxx = std::max(a.x, b.x);
	int miny = std::min(a.y, b.y);
	int maxy = std::max(a.y, b.y);

	return(p.x >= minx && p.x <= maxx && p.y >= miny && p.y <= maxy);
}

bool isPointInPolygon(sf::Vector2i p, std::vector < sf::Vector2i >& poly)
{
	size_t n = poly.size();
	if (n < 3)
		return false;

	bool inside = false;
	for (size_t i = 0, j = n - 1; i < n; j = i++) {
		sf::Vector2i& a = poly[j];
		sf::Vector2i& b = poly[i];

		if (pointOnSegment(p, a, b))
			return true;

		bool crossesY = ((a.y > p.y) != (b.y > p.y));
		if (!crossesY)
			continue;

		int dy = b.y - a.y;
		int lhs = (p.x - a.x) * dy;
		int rhs = (b.x - a.x) * (p.y - a.y);

		bool hit = (dy > 0) ? (lhs < rhs)
			: (lhs > rhs);
		if (hit) inside = !inside;

	}
	return inside;
}



Shape::Shape() {
	_points.clear();
}

Shape::Shape(const Shape& other) {
	_points.reserve(other._points.size());

	for (const auto& point : other._points) {
		addPoint(point);
	}
}

Shape::~Shape() {

}

int Shape::getPointIndex(sf::Vector2i point) {
	for (int i = 0; i < _points.size();i+=1) {
		if (_points[i] == point) {
			return i;
		}
	}

	return -1;
}

void Shape::addPoint(sf::Vector2i point, int index) {
	if(getPointIndex(point) >= 0) {
		return;
	}

	_points.push_back(point);
	std::shared_ptr<sf::Text> text = std::make_shared<sf::Text>(basicFont, std::to_wstring(_points.size()), basic_text_size);
	text->setFillColor(basic_text_color);
	
	if(index < 0 || index >= _pointTexts.size()) {
		_pointTexts.push_back(text);
	} else {
		_pointTexts.insert(_pointTexts.begin() + index, text);
	}

}

void Shape::removePoint(sf::Vector2i point) {
	int index = getPointIndex(point);

	if (index < 0)
		return;

	_points.erase(_points.begin() + index);
	_pointTexts.erase(_pointTexts.begin() + index);

	for(int i = index; i < _pointTexts.size(); i++) {
		_pointTexts[i]->setString(std::to_wstring(i + 1));
	}
}

bool Shape::hasPoint(sf::Vector2i point) {
	
	if(std::find(_points.begin(), _points.end(), point) != _points.end())
		return true;

	return false;
}

int Shape::insidePoint(sf::Vector2i point, float radius, float scale) {
	if (_points.empty())
		return -1;

	sf::Vector2f cursorPosition((float)(point.x) * scale, (float)(point.y) * scale);

	for (int i = static_cast<int>(_points.size()) - 1; i >= 0; --i) {
		sf::Vector2f pointPosition((float)(_points[i].x) * scale, (float)(_points[i].y) * scale);

		float dx = pointPosition.x - cursorPosition.x;
		float dy = pointPosition.y - cursorPosition.y;

		float distanceSquared = dx * dx + dy * dy;

		if (distanceSquared <= radius * radius)
			return i;
	}
	
	return -1;
}

bool Shape::pointInShape(sf::Vector2i point) {
	return isPointInPolygon(point, _points);
}

void Shape::drawConvexShape(sf::Vector2i position, float scale, sf::Color color) {
	sf::ConvexShape convexShape;
	convexShape.setPointCount(_points.size());

	for (int i = 0; i < _points.size(); i++)
		convexShape.setPoint(i, sf::Vector2f(position + sf::Vector2i((float)_points[i].x * scale, (float)_points[i].y * scale)));

	convexShape.setFillColor(sf::Color::Transparent);
	convexShape.setOutlineColor(color);
	float thickness = scale / 2.f;
	if(thickness < 2.0f) {
		thickness = 2.0f;
	}
	convexShape.setOutlineThickness(thickness);
	Main::render_window->draw(convexShape);
}

void Shape::draw(sf::Vector2i position, float scale, sf::Color colorOfPoints, bool renderIndexes) {

	drawConvexShape(position, scale, colorOfPoints);

	float radius = 2.0f * scale; // Radius of the circle

	for(auto& point : _points) {
		
		sf::CircleShape outlineCircle(radius + scale/2.f);
		outlineCircle.setOrigin(sf::Vector2f(radius + scale/2.f, radius + scale/2.f));
		outlineCircle.setFillColor(sf::Color::Transparent);
		outlineCircle.setOutlineThickness(scale/2.f);
		outlineCircle.setOutlineColor(sf::Color(47, 47, 47));
		outlineCircle.setPosition(sf::Vector2f(position + sf::Vector2i((float)point.x * scale, (float)point.y * scale)));
		Main::render_window->draw(outlineCircle);

		sf::CircleShape circle(radius);
		circle.setOrigin(sf::Vector2f(radius, radius));
		circle.setFillColor(colorOfPoints);
		circle.setOutlineThickness(scale/2.f);
		circle.setOutlineColor(sf::Color(79, 79, 79));
		circle.setPosition(sf::Vector2f(position + sf::Vector2i((float)point.x * scale, (float)point.y * scale)));
		Main::render_window->draw(circle);
	}

	if (renderIndexes) {
		for (int i = 0; i < _pointTexts.size(); i++) {
			sf::Vector2f textPos;
			textPos.x = position.x + (float)_points[i].x * scale - _pointTexts[i]->getGlobalBounds().size.x + 16;
			textPos.y = position.y + (float)_points[i].y * scale - _pointTexts[i]->getGlobalBounds().size.y - radius - 16.f;
			_pointTexts[i]->setPosition(textPos);
			Main::render_window->draw(*_pointTexts[i]);
		}
	}
}


Mesh::Mesh(float radius, float scale) {
	_shapes = std::vector<std::shared_ptr<Shape>>();

	setRadius(radius);
	setScale(scale);
}

Mesh::Mesh(const Mesh& other)
	: _radius(other._radius),
	_scale(other._scale) {

	_shapes.reserve(other._shapes.size());

	for (const auto& shape : other._shapes) {
		if (shape) {
			_shapes.push_back(std::make_shared<Shape>(*shape));
		}
		else {
			_shapes.push_back(nullptr);
		}
	}
}

Mesh::~Mesh() {

}
void Mesh::setScale(float scale) {
	_scale = scale;
}

void Mesh::setRadius(float radius) {
	_radius = radius;
}

int Mesh::getShapeCount() {
	return _shapes.size();
}

void Mesh::addShape(std::shared_ptr<Shape> shape) {
	_shapes.push_back(shape);
}


std::shared_ptr<Shape> Mesh::getShape(int index) {
	if (index < 0 || index >= _shapes.size()) {
		return nullptr;
	}
	return _shapes[index];
}

std::shared_ptr<Shape> Mesh::getLastShape() {
	if (_shapes.empty()) {
		return nullptr;
	}
	return _shapes.back();
}

void Mesh::removeShape(int index) {
	if (index < 0 || index >= _shapes.size()) {
		return;
	}
	_shapes.erase(_shapes.begin() + index);
}

void Mesh::removeShape(std::shared_ptr<Shape> shape) {
	auto it = std::find(_shapes.begin(), _shapes.end(), shape);

	if (it != _shapes.end()) {
		_shapes.erase(it);
	}
}

void Mesh::clearShapes() {
	_shapes.clear();
}

bool Mesh::everyShapeIsComplete() {

	if (_shapes.empty())
		return false;

	for (auto& shape : _shapes) {
		if (shape->_points.size() < 3)
			return false;
	}

	return true;
}

bool Mesh::isPointInside(sf::Vector2i point, sf::Vector2i position) {
	for(auto& shape : _shapes) {
		if(shape->pointInShape(point - position)) {
			return true;
		}
	}

	return false;
}


void Mesh::draw(sf::Vector2i position, float scale, sf::Color colorOfPoints, bool renderIndexes, std::shared_ptr<Shape> activeShape) {
	for (auto& shape : _shapes) {
		sf::Color shapeColor = sf::Color(127, 127, 127);

		if (shape.get() == activeShape.get()) {
			shapeColor = colorOfPoints;
		}
		shape->draw(position, scale, shapeColor, renderIndexes);
	}
}

void Mesh::draw(sf::Vector2i position, sf::Color color) {
	for (auto& shape : _shapes) {
		shape->draw(position, 1.0f, color, false);
	}
}


void saveMesh(std::shared_ptr<Mesh> mesh, std::ofstream& saver) {
	BinaryWriter writer(saver);

	if (mesh == nullptr) {
		writer.write_int32(0);
		return;
	}

	writer.write_int32(mesh->_shapes.size());

	for (auto& shape : mesh->_shapes) {
		writer.write_int32(shape->_points.size());

		for (auto& point : shape->_points) {
			writer.write_Vector2i(point);
		}
	}
}

std::shared_ptr<Mesh> loadMesh(std::ifstream& loader) {
	BinaryReader reader(loader);

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(2.0f, 1.0f);
	int shapeCount = reader.read_int32();

	for (int i = 0; i < shapeCount; i++) {
		int pointCount = reader.read_int32();
		std::shared_ptr<Shape> shape = std::make_shared<Shape>();
		for (int j = 0; j < pointCount; j++) {
			sf::Vector2i point = reader.read_Vector2i();
			shape->addPoint(point);
		}
		mesh->addShape(shape);
	}

	return mesh;
}