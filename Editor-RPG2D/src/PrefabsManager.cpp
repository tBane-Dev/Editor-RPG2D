#include "PrefabsManager.hpp"
#include "DebugLog.hpp"
#include "Editors/MapEditor/Objects/Monster.hpp"
#include "Editors/MapEditor/Objects/Nature.hpp"

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
	
    {
        std::shared_ptr<GameObject> prefab = std::make_shared<MonsterPrefab>(
            L"golem",
            animations_manager->getAnimations(L"assets\\tex\\monsters\\golem.png"),
            sf::Vector2i(62, 96),
            4
        );
        addPrefab(prefab);
    }

    {
        std::shared_ptr<GameObject> prefab = std::make_shared<MonsterPrefab>(
            L"dziobak",
            animations_manager->getAnimations(L"assets\\tex\\monsters\\dziobak.png"),
            sf::Vector2i(62, 88),
            6
        );
        addPrefab(prefab);
    }

    {
        std::shared_ptr<GameObject> prefab = std::make_shared<MonsterPrefab>(
            L"goblin",
            animations_manager->getAnimations(L"assets\\tex\\monsters\\goblin.png"),
            sf::Vector2i(62, 90),
            6
        );
        addPrefab(prefab);
    }

    {
        std::shared_ptr<GameObject> prefab = std::make_shared<NaturePrefab>(
            L"tree_1",
            animations_manager->getAnimations(L"assets\\tex\\tree_1.png"),
            sf::Vector2i(128, 214)
        );
        addPrefab(prefab);
    }

    {
        std::shared_ptr<GameObject> prefab = std::make_shared<NaturePrefab>(
            L"boulder_1",
            animations_manager->getAnimations(L"assets\\tex\\boulder_1.png"),
            sf::Vector2i(61, 74)
        );
        addPrefab(prefab);
    }

    {
        std::shared_ptr<GameObject> prefab = std::make_shared<NaturePrefab>(
            L"boulder_2",
            animations_manager->getAnimations(L"assets\\tex\\boulder_2.png"),
            sf::Vector2i(62, 74)
        );
        addPrefab(prefab);
    }
    
}
std::shared_ptr<PrefabsManager> prefabs_manager = nullptr;
