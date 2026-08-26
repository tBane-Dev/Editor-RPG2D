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

	virtual int getTopOffset(int wallHeight, float scale);
	virtual void generate(std::vector<std::vector<int>> tiles, sf::Vector2i positiion, float scale = 1.f);
	virtual void draw(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f);
};
