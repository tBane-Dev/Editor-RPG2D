#pragma once
#include <SFML/Graphics.hpp>

class Roof {
public:
	int _type;
	int _wallHeight;

	std::vector<std::vector<int>> _tiles;
	sf::Vector2i _roofOverhangSize;
	sf::VertexArray _roofOverhangVertexArray;
	sf::Texture _texture;

	Roof();
	Roof(int type, int wallHeight);
	~Roof();

	virtual int getTopOffset(float scale);
	virtual void generate(std::vector<std::vector<int>> tiles, sf::Vector2i positiion, float scale = 1.f);
	virtual void draw(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f);
};
