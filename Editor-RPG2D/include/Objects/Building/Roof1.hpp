#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Building/Roof.hpp"
#include "Objects/Building/FlatRoofPart.hpp"


class Roof1 : public Roof {
public:
	std::vector<std::vector<int>> _mask;
	std::vector<std::shared_ptr<FlatRoofPart>> _parts;
	
	Roof1();
	~Roof1();

	void generateMask(std::vector<std::vector<int>> tiles);
	void generateParts(sf::Vector2i position, float scale = 1.0f);
	void generateOverhang(sf::Vector2i position, float scale = 1.0f);
	void generateTexture(sf::Vector2i position, float scale);
	virtual void generate(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale = 1.f);
	virtual void draw(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f);
};
