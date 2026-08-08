#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Building/FlatRoofPart.hpp"

class Roof {
public:

	sf::Vector2i _size;
	std::vector<int> _tiles;

	Roof();
	~Roof();

	virtual void generate(sf::Vector2i size, std::vector<int> tiles, sf::Vector2i positiion, float scale = 1.f);
	virtual void draw(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f);
};

class FlatRoof : public Roof {
public:
	std::vector<int> _mask;
	std::vector<std::shared_ptr<FlatRoofPart>> _parts;
	sf::VertexArray _roofOverhangVertexArray;
	sf::Texture _roofTexture;

	FlatRoof();
	~FlatRoof();

	void generateMask(std::vector<int> tiles);
	void generateParts(sf::Vector2i position, float scale = 1.0f);
	void generateOverhang(sf::Vector2i position, float scale = 1.0f);
	void generateTexture(sf::Vector2i position, float scale);
	virtual void generate(sf::Vector2i size, std::vector<int> tiles, sf::Vector2i position, float scale = 1.f);
	virtual void draw(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f);
};

class GableRoof : public Roof {
public:

	GableRoof();
	~GableRoof();

	virtual void generate(sf::Vector2i size, std::vector<int> tiles, sf::Vector2i position, float scale = 1.f);
	virtual void draw(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f);
};