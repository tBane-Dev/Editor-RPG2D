#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.hpp"
#include <fstream>

class PrefabsManager {
public:
    std::vector<std::shared_ptr<GameObject>> _prefabs;
    
    PrefabsManager();
    ~PrefabsManager();
    
	void addPrefab(std::shared_ptr<GameObject> prefab);
    void removePrefab(std::weak_ptr<GameObject> prefab);
    std::shared_ptr<GameObject> getPrefab(std::wstring path);
	std::vector<std::shared_ptr<GameObject>>& getAllPrefabs();
	std::vector<std::shared_ptr<GameObject>> getPrefabs(ObjectType type);
    void removePrefabsByAnimations(int animationID);
    void replacePrefab(std::shared_ptr<GameObject> oldPrefab, std::shared_ptr<GameObject> newPrefab);

	void saveFromProject(std::ofstream& saver);
	void loadFromProject(std::ifstream& loader);

    void loadBuildingsPartsPrefabs();
};

extern std::shared_ptr<PrefabsManager> prefabs_manager;
extern std::shared_ptr<PrefabsManager> buildings_parts_prefabs_manager;
