#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Building/Roof.hpp"

class Roof2 : public Roof {
public:
	std::vector<std::vector<int>> _mask;
	std::vector<sf::IntRect> _rects;

	std::vector<sf::ConvexShape> _bottomTriangle;
	std::vector<sf::ConvexShape> _topTriangle;
	std::vector<sf::ConvexShape> _rect;

	Roof2();
	~Roof2();

	void generateMask(std::vector<std::vector<int>> tiles);
	void generateRects();
	void generateShape(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale = 1.0f);
	void generateOverhang(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale = 1.0f);
	void generateTexture(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale);
	int getRoofHeight(float scale);

	virtual int getTopOffset(int wallHeight, float scale);
	virtual void generate(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale = 1.f);
	virtual void draw(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f);
};