#include "PrefabsManager.hpp"
#include "DebugLog.hpp"
#include "Editors/MapEditor/Objects/Monster.hpp"

PrefabsManager::PrefabsManager() {
	_prefabs.clear();
}

PrefabsManager::~PrefabsManager() {

}

void PrefabsManager::addPrefab(std::shared_ptr<GameObject> prefab) {
	_prefabs.push_back(prefab);
}

std::shared_ptr<GameObject> PrefabsManager::getPrefab(std::wstring path) {
	for (auto& prefab : _prefabs) {
		if (prefab->_name == path) {
			return prefab;
		}
	}
	return nullptr;
}

std::vector<std::shared_ptr<GameObject>>& PrefabsManager::getAllPrefabs() {
	return _prefabs;
}

void PrefabsManager::loadPrefabs() {
	
    std::shared_ptr<GameObject> prefab1 = std::make_shared<MonsterPrefab>(
        L"golem",
        animations_manager->getAnimations(L"assets\\tex\\monsters\\golem.png"),
        sf::Vector2i(62, 96),
        4
    );

    std::shared_ptr<GameObject> prefab3 = std::make_shared<MonsterPrefab>(
        L"dziobak",
        animations_manager->getAnimations(L"assets\\tex\\monsters\\dziobak.png"),
        sf::Vector2i(62, 88),
        6
    );

    std::shared_ptr<GameObject> prefab4 = std::make_shared<MonsterPrefab>(
        L"goblin",
        animations_manager->getAnimations(L"assets\\tex\\monsters\\goblin.png"),
        sf::Vector2i(62, 90),
        6
    );


    std::shared_ptr<GameObject> prefab2 = std::make_shared<GameObject>(
        L"tree",
        animations_manager->getAnimations(L"assets\\tex\\tree.png"),
        sf::Vector2i(128, 214)
    );

    addPrefab(prefab1);
    addPrefab(prefab2);
    addPrefab(prefab3);
    addPrefab(prefab4);
}
std::shared_ptr<PrefabsManager> prefabs_manager = nullptr;
