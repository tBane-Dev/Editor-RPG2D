#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/Building/Roof.hpp"
#include "TexturesManager.hpp"
#include "AnimationsManager.hpp"


class Roof1Part {
public:
	int _id = -1;
	sf::IntRect _textureRect;
	sf::Vector2i _position;

	Roof1Part(int id, sf::IntRect textureRect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)));
	~Roof1Part();

	void setPosition(sf::Vector2i position);
};

class Roof1set {
public:

	struct Roof {
		int x, y;

		void Set(int xx, int yy) {
			x = xx;
			y = yy;
		}

		sf::Vector2i get() {
			return sf::Vector2i(x, y);
		}
	};

	struct Group {
		std::wstring name;
		Roof roofs[49];

		void SetPart(int id, int xx, int yy) {
			roofs[id].Set(xx, yy);
		}

		Group(std::wstring name) {

			this->name = name;

			for (int i = 0; i < 49; i++) {
				roofs[i].Set(0, 0);
			}
		}
	};

	std::shared_ptr<Texture> _texture;
	std::shared_ptr<Texture> _overhangTexture;
	std::shared_ptr<Animations> _animations;

	std::vector<std::shared_ptr<Group>> _groups;

	Roof1set();
};

extern std::shared_ptr<Roof1set> roof1set;

class Roof1 : public Roof {
public:

	std::vector<std::vector<int>> _mask;
	std::vector<std::shared_ptr<Roof1Part>> _parts;

	Roof1();
	~Roof1();

	void generateMask(std::vector<std::vector<int>> tiles);
	void generateParts(sf::Vector2i position, float scale = 1.0f);
	void generateOverhang(sf::Vector2i position, float scale = 1.0f);
	void generateTexture(sf::Vector2i position, float scale);

	virtual int getTopOffset(int wallHeight, float scale);
	virtual void generate(std::vector<std::vector<int>> tiles, sf::Vector2i position, float scale = 1.f);
	virtual void draw(sf::RenderTarget& target, sf::Vector2i position, float scale = 1.0f);
};
