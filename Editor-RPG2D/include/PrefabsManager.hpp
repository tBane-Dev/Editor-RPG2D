#pragma once
#include <SFML/Graphics.hpp>
#include "Editors/MapEditor/Objects/GameObject.hpp"


class PrefabsManager {
public:
    std::vector<std::shared_ptr<GameObject>> _prefabs;
    
    PrefabsManager();
    ~PrefabsManager();
    
	void addPrefab(std::shared_ptr<GameObject> prefab);
    std::shared_ptr<GameObject> getPrefab(std::wstring path);
	std::vector<std::shared_ptr<GameObject>>& getAllPrefabs();
    void loadPrefabs();
};

extern std::shared_ptr<PrefabsManager> prefabs_manager;
