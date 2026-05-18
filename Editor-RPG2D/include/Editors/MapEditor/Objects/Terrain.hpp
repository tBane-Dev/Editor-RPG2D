#pragma once
#include <SFML/Graphics.hpp>
#include "Editors/MapEditor/Objects/Object.hpp"

class Terrain : public Object {
public:

	int _id = -1;

	Terrain();
	Terrain(int id);
	~Terrain();
};