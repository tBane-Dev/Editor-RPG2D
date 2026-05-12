#pragma once
#include <SFML/Graphics.hpp>
#include "Editors/MapEditor/Objects/GameObjectOnMap.hpp"


class GameObjectsOnMap {
public:
    std::vector<std::shared_ptr<GameObjectOnMap>> _gameObjectsOnMap;
    
    GameObjectsOnMap();
    ~GameObjectsOnMap(); 
    
	void addGameObject(std::shared_ptr<GameObjectOnMap> gameObjectOnMap);
    void sort();

    virtual void update();
    virtual void draw();
};


