#pragma once
#include <SFML/Graphics.hpp>
#include "Editors/MapEditor/Map/GameObjectOnMap.hpp"


class GameObjectsOnMap {
public:
    std::vector<std::shared_ptr<GameObjectOnMap>> _gameObjectsOnMap;
    
    GameObjectsOnMap();
    ~GameObjectsOnMap(); 
    
	void addGameObject(std::weak_ptr<GameObjectOnMap> gameObjectOnMap);
    void removeGameObjectsByPrefab(std::weak_ptr<GameObject> prefab);
    void removeGameObjectsByAnimations(int animationID);
    void sort();

    virtual void update();
    virtual void draw();
};


