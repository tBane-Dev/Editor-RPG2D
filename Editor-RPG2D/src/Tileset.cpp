#include "Tileset.hpp"
#include "EditorsManager.hpp"
#include "Editors/MapEditor/Editor.hpp"

Tileset::Tileset() {

	std::shared_ptr<Group> ocean = std::make_shared<Group>(L"ocean");
	std::shared_ptr<Group> ocean_beach = std::make_shared<Group>(L"ocean-beach");
	std::shared_ptr<Group> beach_grass = std::make_shared<Group>(L"beach-grass");
	std::shared_ptr<Group> beach_swamp = std::make_shared<Group>(L"beach-swamp");
	std::shared_ptr<Group> grass_swamp = std::make_shared<Group>(L"grass-swamp");
	std::shared_ptr<Group> swamp_bog = std::make_shared<Group>(L"swamp-bog");
	std::shared_ptr<Group> grass_rocks = std::make_shared<Group>(L"grass-rocks");
	std::shared_ptr<Group> rocks_magma = std::make_shared<Group>(L"rocks-magma");
	std::shared_ptr<Group> magma_lava = std::make_shared<Group>(L"magma-lava");

	groups.push_back(ocean);
	groups.push_back(ocean_beach);
	groups.push_back(beach_grass);
	groups.push_back(beach_swamp);
	groups.push_back(grass_swamp);
	groups.push_back(swamp_bog);
	groups.push_back(grass_rocks);
	groups.push_back(rocks_magma);
	groups.push_back(magma_lava);


	for(int i=0;i<groups.size(); i++) {
		groups[i]->SetPart(1, 32, 4*i);  // lewo-gora.
		groups[i]->SetPart(2, 48, 4*i);  // srodek-gora.
		groups[i]->SetPart(3, 16, 4*i);  // prawo-gora.
		groups[i]->SetPart(4, 40, 4*i);  // lewo-srodek.
		groups[i]->SetPart(5, 60, 4*i);  // srodek.
		groups[i]->SetPart(6, 20, 4*i);  // prawo-srodek.
		groups[i]->SetPart(7, 8, 4*i);  // lewo-dol.
		groups[i]->SetPart(8, 12, 4*i);  // srodek-dol.
		groups[i]->SetPart(9, 4, 4*i);   // prawo-dol.
		groups[i]->SetPart(11, 56, 4*i);  // wewnętrzny lewo-gora
		groups[i]->SetPart(13, 44, 4*i); // wewnętrzny lewo-dol
		groups[i]->SetPart(12, 52, 4*i); // wewnętrzny prawo-gora
		groups[i]->SetPart(14, 28, 4*i); // wewnętrzny prawo-dol
	}

	ocean_beach->friends.push_back(beach_grass);
	ocean_beach->friends.push_back(beach_swamp);
	ocean_beach->friends.push_back(grass_swamp);

	beach_grass->friends.push_back(grass_swamp);
	beach_grass->friends.push_back(grass_rocks);

	beach_swamp->friends.push_back(swamp_bog);

	grass_swamp->friends.push_back(swamp_bog);

	grass_rocks->friends.push_back(rocks_magma);

	rocks_magma->friends.push_back(magma_lava); 
}

bool areFriends(int group1, int group2) {
	if (group1 == -1 || group2 == -1) return true;
	if (group1 == group2) return true;

	for (auto& fr : tileset->groups[group1]->friends) {
		if (fr == tileset->groups[group2])
			return true;
	}
	return false;
}

std::shared_ptr<Tileset> tileset = nullptr;