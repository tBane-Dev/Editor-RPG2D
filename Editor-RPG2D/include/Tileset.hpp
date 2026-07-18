#pragma once
#include <SFML/Graphics.hpp>

class Tileset {
public:

	struct Tile {
		int x, y;

		void Set(int xx, int yy) {
			x = xx;
			y = yy;
		}
	};

	struct Group {
		std::wstring name;
		Tile tiles[16];

		std::vector<std::shared_ptr<Group>> friends;

		/*

		11           12
			1, 2, 3
			4, 5, 6
			7, 8, 9
		13          14

		*/
		void SetPart(int id, int xx, int yy) {
			tiles[id].Set(xx, yy);
		}

		Group(std::wstring name) {
			
			this->name = name;

			for (int i = 1; i <= 14; i++) {
				tiles[i].Set(0, 0);
			}
		}
	};

	std::vector<std::shared_ptr<Group>> groups;// Lista grup. możesz im dodać np std::string Name i nazywać aby było łatwiej w edytorze z tym działać

	Tileset();
};

bool areFriends(int group1, int group2);

extern std::shared_ptr<Tileset> tileset;