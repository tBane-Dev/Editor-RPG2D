#include "Window.hpp"
#include "SFML/Graphics.hpp"
#include "Editors/MapEditor/Map/Map.hpp"

class Path {
public:
	sf::Vector2i _startPoint, _endPoint;
	std::vector<sf::Vector2i> _pathPoints;

	Path();
	~Path();
	void setStartPoint(sf::Vector2i point);
	void setEndPoint(sf::Vector2i point);
	bool isEmpty();
	sf::Vector2i getStartPoint();
	sf::Vector2i getEndPoint();
	void generatePath();
	void nextPoint();
	void draw();
};