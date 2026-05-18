#pragma once
#include "Editors/MapEditor/Objects/Terrain.hpp"

Terrain::Terrain() {
	_id = -1;
	_type = ObjectType::Terrain;
}

Terrain::Terrain(int id) {
	_id = id;
	_type = ObjectType::Terrain;
}

Terrain::~Terrain() {

}
