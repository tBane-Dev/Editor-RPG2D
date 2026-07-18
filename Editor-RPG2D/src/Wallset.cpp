#include "Wallset.hpp"

Wallset::Wallset() {

	_texture = textures_manager->getTexture(L"assets\\tex\\wallset.png");
	_animations = std::make_shared<Animations>(L"wallset", _texture, sf::Vector2i(32, 32), _texture->getSize().y / 32, _texture->getSize().x / 32);

	std::shared_ptr<Group> wooden_wall = std::make_shared<Group>(L"wooden wall");
	std::shared_ptr<Group> stone_wall = std::make_shared<Group>(L"stone wall");
	std::shared_ptr<Group> mulch_wall = std::make_shared<Group>(L"mulch wall");
	std::shared_ptr<Group> mud_wall = std::make_shared<Group>(L"mud wall");
	//std::shared_ptr<Group> brick_wall = std::make_shared<Group>(L"brick wall");
	//std::shared_ptr<Group> fortified_wall = std::make_shared<Group>(L"fortified wall");

	_groups.push_back(wooden_wall);
	_groups.push_back(stone_wall);
	_groups.push_back(mulch_wall);
	_groups.push_back(mud_wall);
	//groups.push_back(brick_wall);
	//groups.push_back(fortified_wall);


	for (int i = 0; i < _groups.size(); i++) {
		// Basic wall segments
		_groups[i]->SetPart(0, 0, i * 32);        // Empty / black tile
		_groups[i]->SetPart(1, 32, i * 32);       // Isolated wall tile

		_groups[i]->SetPart(2, 64, i * 32);       // Horizontal wall: left end
		_groups[i]->SetPart(3, 96, i * 32);       // Horizontal wall: right end
		_groups[i]->SetPart(4, 128, i * 32);      // Vertical wall: top end
		_groups[i]->SetPart(5, 160, i * 32);      // Vertical wall: bottom end
		_groups[i]->SetPart(6, 192, i * 32);      // Vertical wall: middle
		_groups[i]->SetPart(7, 224, i * 32);      // Horizontal wall: middle

		// Outer L-shaped corners
		_groups[i]->SetPart(8, 256, i * 32);      // Outer corner: top-left
		_groups[i]->SetPart(9, 288, i * 32);      // Outer corner: top-right
		_groups[i]->SetPart(10, 320, i * 32);     // Outer corner: bottom-left
		_groups[i]->SetPart(11, 352, i * 32);     // Outer corner: bottom-right

		// Inner L-shaped corners
		_groups[i]->SetPart(12, 384, i * 32);     // Inner corner: top-left
		_groups[i]->SetPart(13, 416, i * 32);     // Inner corner: top-right
		_groups[i]->SetPart(14, 448, i * 32);     // Inner corner: bottom-right
		_groups[i]->SetPart(15, 480, i * 32);     // Inner corner: bottom-left

		// 
		_groups[i]->SetPart(16, 512, i * 32);     // Missing bottom; top-right diagonal connected
		_groups[i]->SetPart(17, 544, i * 32);     // Missing left; bottom-right diagonal connected
		_groups[i]->SetPart(18, 576, i * 32);     // Missing top; bottom-left diagonal connected
		_groups[i]->SetPart(19, 608, i * 32);     // Missing right; top-left diagonal connected
		_groups[i]->SetPart(20, 640, i * 32);     // Missing bottom; top-left diagonal connected
		_groups[i]->SetPart(21, 672, i * 32);     // Missing left; top-right diagonal connected
		
		//
		_groups[i]->SetPart(22, 704, i * 32);     // Missing top; bottom-right diagonal connected
		_groups[i]->SetPart(23, 736, i * 32);     // Missing right; bottom-left diagonal connected
		_groups[i]->SetPart(24, 768, i * 32);     // T-junction: missing bottom
		_groups[i]->SetPart(25, 800, i * 32);     // T-junction: missing left

		// T-junctions without connected diagonals
		_groups[i]->SetPart(26, 832, i * 32);     // T-junction: missing bottom
		_groups[i]->SetPart(27, 864, i * 32);     // T-junction: missing left

		//

		// T-junctions with one connected diagonal
		_groups[i]->SetPart(28, 896, i * 32);     // Missing bottom; top-right diagonal connected
		_groups[i]->SetPart(29, 928, i * 32);     // Missing left; bottom-right diagonal connected
		_groups[i]->SetPart(30, 960, i * 32);     // Missing top; bottom-left diagonal connected
		_groups[i]->SetPart(31, 992, i * 32);     // Missing right; top-left diagonal connected

		_groups[i]->SetPart(32, 1024, i * 32);     // Missing bottom; top-left diagonal connected
		_groups[i]->SetPart(33, 1056, i * 32);     // Missing left; top-right diagonal connected
		_groups[i]->SetPart(34, 1088, i * 32);     // Missing top; bottom-right diagonal connected
		_groups[i]->SetPart(35, 1120, i * 32);     // Missing right; bottom-left diagonal connected

		// T-junctions without connected diagonals
		_groups[i]->SetPart(36, 1152, i * 32);     // T-junction: missing bottom
		_groups[i]->SetPart(37, 1184, i * 32);     // T-junction: missing left
		_groups[i]->SetPart(38, 1216, i * 32);     // T-junction: missing top
		_groups[i]->SetPart(39, 1248, i * 32);     // T-junction: missing right

		// Four-way junction
		_groups[i]->SetPart(40, 1280, i * 32);     // Cross: no connected diagonals

		// Extended outer L-shaped corners
		_groups[i]->SetPart(41, 1312, i * 32);     // Extended outer corner: top-left
		_groups[i]->SetPart(42, 1344, i * 32);     // Extended outer corner: top-right
		_groups[i]->SetPart(43, 1376, i * 32);     // Extended outer corner: bottom-left
		_groups[i]->SetPart(44, 1408, i * 32);    // Extended outer corner: bottom-right

		// Extended straight edges
		_groups[i]->SetPart(45, 1440, i * 32);    // Extended edge: left
		_groups[i]->SetPart(46, 1472, i * 32);    // Extended edge: top
		_groups[i]->SetPart(47, 1504, i * 32);    // Extended edge: right
		_groups[i]->SetPart(48, 1536, i * 32);    // Extended edge: bottom

		// Bottom wall layer
		_groups[i]->SetPart(49, 1568, i * 32);    // Bottom wall: isolated segment
		_groups[i]->SetPart(50, 1600, i * 32);    // Bottom wall: left end
		_groups[i]->SetPart(51, 1632, i * 32);    // Bottom wall: middle
		_groups[i]->SetPart(52, 1664, i * 32);    // Bottom wall: right end

	}

	for (int i = 0; i < _groups.size(); i++) {
		std::shared_ptr<RectangularCollider> collider = std::make_shared<RectangularCollider>(0, 0, 32, 32);
		std::shared_ptr<WallPrefab> prefab =
			std::make_shared<WallPrefab>(
				_groups[i]->name,
				_animations,
				sf::Vector2i(0, 0),
				collider,
				nullptr,
				i
			);
		_prefabs.emplace_back(prefab);
	}
}

std::shared_ptr<WallPrefab> Wallset::getPrefab(int id) {

	if (id < 0 || id >= _prefabs.size())
		return nullptr;

	return _prefabs[id];
}

std::shared_ptr<Wallset> wallset = nullptr;