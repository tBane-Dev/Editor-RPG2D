#pragma once
#include <SFML/Graphics.hpp>
#include "TexturesManager.hpp"
#include "AnimationsManager.hpp"
#include "Objects/Wall.hpp"

class Wallset {
public:

	struct Wall {
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
		Wall walls[53];

		void SetPart(int id, int xx, int yy) {
			walls[id].Set(xx, yy);
		}

		Group(std::wstring name) {

			this->name = name;

			for (int i = 0; i < 53; i++) {
				walls[i].Set(0, 0);
			}
		}
	};

	std::shared_ptr<Texture> _texture;
	std::shared_ptr<Animations> _animations;

	std::vector<std::shared_ptr<Group>> _groups;
	std::vector<std::shared_ptr<WallPrefab>> _prefabs;

	Wallset();

	std::shared_ptr<WallPrefab> getPrefab(int id);
};

extern std::shared_ptr<Wallset> wallset;