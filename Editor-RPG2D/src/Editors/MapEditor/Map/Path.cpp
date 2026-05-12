#include "Editors/MapEditor/Map/Path.hpp"
#include <unordered_set>
#include "Editors/MapEditor/Map/Map.hpp"
#include <iostream>
#include "Editors/Editor.hpp"
#include "Editors/MapEditor/MapEditor.hpp"

Path::Path() {
	_startPoint = sf::Vector2i(0, 0);
	_endPoint = sf::Vector2i(0, 0);
	_pathPoints.clear();

}

Path::~Path() {

}

void Path::setStartPoint(sf::Vector2i point) {
	_startPoint = point;
}

void Path::setEndPoint(sf::Vector2i point) {
	_endPoint = point;
}

bool Path::isEmpty() {
	return _pathPoints.empty();
}

sf::Vector2i Path::getStartPoint() {
	return _startPoint;
}

sf::Vector2i Path::getEndPoint() {
	return _endPoint;
}

void Path::generatePath() {

	std::shared_ptr<Map> mapa = map_editor->_map;
	if (mapa == nullptr) return;

	enum Direction { Top, Bottom };

	Direction direction = Direction(rand() % 2);

	std::vector<std::vector<sf::Vector2i>> paths;

	// visited points hash
	struct Vector2iHash {
		std::size_t operator()(const sf::Vector2i& v) const {
			return v.x ^ (v.y << 16);
		}
	};

	std::unordered_set<sf::Vector2i, Vector2iHash> visited;

	paths.push_back({_startPoint});

	while (!paths.empty()) {

		std::vector<sf::Vector2i> path = paths.front();
		paths.erase(paths.begin());

		sf::Vector2i point = path.back();

		if(!mapa->getRect().contains(point))
			continue;

		if (visited.contains(point))
			continue;

		visited.insert(point);

		if (point == _endPoint) {
			_pathPoints = path;
			return;
		}

		if (direction == Direction::Top) {
			std::vector<sf::Vector2i> top = path;
			top.push_back({ point.x, point.y - Tile::tileSize });
			paths.push_back(top);

			std::vector<sf::Vector2i> bottom = path;
			bottom.push_back({ point.x, point.y + Tile::tileSize });
			paths.push_back(bottom);
		}
		else {
			std::vector<sf::Vector2i> bottom = path;
			bottom.push_back({ point.x, point.y + Tile::tileSize });
			paths.push_back(bottom);

			std::vector<sf::Vector2i> top = path;
			top.push_back({ point.x, point.y - Tile::tileSize });
			paths.push_back(top);
		}
		
		std::vector<sf::Vector2i> right = path;
		right.push_back({ point.x + Tile::tileSize, point.y });
		paths.push_back(right);

		std::vector<sf::Vector2i> left = path;
		left.push_back({ point.x - Tile::tileSize, point.y });
		paths.push_back(left);
	}

	_pathPoints.clear();
}

void Path::nextPoint() {
	if (_pathPoints.empty()) return;

	_pathPoints.erase(_pathPoints.begin());
	if (!_pathPoints.empty()) {
		_startPoint = _pathPoints.front();
	}
	else {
		_startPoint = _endPoint;
	}
}

void Path::draw() {
	if (_pathPoints.empty()) return;

	int radius = 4;
	int edgeRadius = radius + 4;

	sf::IntRect map_rect = map_editor->_map->getRect();

	// path points
	sf::CircleShape pointShape((float)radius, 8);
	pointShape.setOrigin(sf::Vector2f((float)radius, (float)radius));
	pointShape.setFillColor(sf::Color::Red);

	for(auto& point : _pathPoints) {
		pointShape.setPosition(sf::Vector2f(map_rect.position) + sf::Vector2f(point));
		window->draw(pointShape);
	}

	// edge points
	sf::CircleShape edgePointShape((float)edgeRadius,8);
	edgePointShape.setOrigin(sf::Vector2f((float)edgeRadius, (float)edgeRadius));
	edgePointShape.setFillColor(sf::Color::Blue);

	edgePointShape.setPosition(sf::Vector2f(map_rect.position) + sf::Vector2f(_startPoint));
	window->draw(edgePointShape);

	edgePointShape.setPosition(sf::Vector2f(map_rect.position) + sf::Vector2f(_endPoint));
	window->draw(edgePointShape);
}