#pragma once
#include <SFML/Graphics.hpp>

class Shape {
public:

	std::vector<sf::Vector2i> _points;
	std::vector<std::shared_ptr<sf::Text>> _pointTexts;
	sf::ConvexShape convexShape;

	Shape();
	Shape(const Shape& other, float scale);  // copy constructor
	~Shape();

	int getPointIndex(sf::Vector2i point);
	void addPoint(sf::Vector2i point, int index = -1, float scale = 1.0f);
	void removePoint(sf::Vector2i point, float scale = 1.0f);
	bool hasPoint(sf::Vector2i point);
	int insidePoint(sf::Vector2i point, float radius, float scale);
	bool pointInShape(sf::Vector2i point);
	void generateConvexShape();
	void drawConvexShape(sf::Vector2i position, float scale, sf::Color color);
	void draw(sf::Vector2i position, float scale, sf::Color colorOfPoints, bool renderIndexes, bool renderPoints = false);
};
 
class Mesh {
public:

	float _radius;
	float _scale;

	std::vector<std::shared_ptr<Shape>> _shapes;

	Mesh(float radius, float scale);
	Mesh(const Mesh& other); // copy constructor
	~Mesh();

	void setScale(float scale);
	void setRadius(float radius);

	int getShapeCount();
	void addShape(std::shared_ptr<Shape> shape);
	std::shared_ptr<Shape> getShape(int index);
	std::shared_ptr<Shape> getLastShape();
	void removeShape(int index);
	void removeShape(std::shared_ptr<Shape> shape);
	void clearShapes();
	bool everyShapeIsComplete();
	bool isPointInside(sf::Vector2i point, sf::Vector2i position);

	void drawConvexShapes(sf::Vector2i position, float scale, sf::Color color);
	void draw(sf::Vector2i position, float scale, sf::Color colorOfPoints, bool renderIndexes = true, std::shared_ptr<Shape> activeShape = nullptr);
	void draw(sf::Vector2i position, sf::Color color, bool renderPoints = false);
};

void saveMesh(std::shared_ptr<Mesh> mesh, std::ofstream& saver);
std::shared_ptr<Mesh> loadMesh(std::ifstream& loader);