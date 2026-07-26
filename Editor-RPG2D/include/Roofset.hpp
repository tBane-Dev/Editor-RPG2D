#pragma once
#include <SFML/Graphics.hpp>
#include "TexturesManager.hpp"
#include "AnimationsManager.hpp"
#include "Objects/Building/FlatRoofPart.hpp"

class Roofset {
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
	std::vector<std::shared_ptr<FlatRoofPartPrefab>> _prefabs;

	Roofset();

	std::shared_ptr<FlatRoofPartPrefab> getPrefab(int id);
};

extern std::shared_ptr<Roofset> roofset;