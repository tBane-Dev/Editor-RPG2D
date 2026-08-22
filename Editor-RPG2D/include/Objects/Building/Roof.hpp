#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Building/FlatRoofPart.hpp"

class Roof {
public:

	std::vector<std::vector<int>> _tiles;
	sf::Vector2i _roofOverhangSize;
	sf::VertexArray _roofOverhangVertexArray;
	sf::Texture _roofTexture;

	Roof();
	~Roof();

	virtual void generate(std::vector<std::vector<int>> tiles, sf::Vector2i positiion, float scale = 1.f);
	virtual void draw(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f);
};

class FlatRoof : public Roof {
public:
	std::vector<std::vector<int>> _mask;
	std::vector<std::shared_ptr<FlatRoofPart>> _parts;
	
	FlatRoof();
	~FlatRoof();

	void generateMask(std::vector<std::vector<int>> tiles);
	void generateParts(sf::Vector2i position, float scale = 1.0f);
	void generateOverhang(sf::Vector2i position, float scale = 1.0f);
	void generateTexture(sf::Vector2i position, float scale);
	virtual void generate(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale = 1.f);
	virtual void draw(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f);
};

class GableRoof : public Roof {
public:
	sf::ConvexShape _bottomTriangle;
	sf::ConvexShape _topTriangle;
	sf::ConvexShape _rect;

	GableRoof();
	~GableRoof();

	float getTopOffset(int wallHeight, float scale);

	void generateShape(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale = 1.0f);
	void generateOverhang(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale = 1.0f);
	void generateTexture(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale);
	virtual void generate(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale = 1.f);
	virtual void draw(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f);
};