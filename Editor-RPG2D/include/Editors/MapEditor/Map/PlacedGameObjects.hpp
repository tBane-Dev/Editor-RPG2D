#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/PlacedGameObject.hpp"
#include <fstream>

namespace MapEditor {
    class PlacedGameObjects {
    public:
        std::vector<std::shared_ptr<PlacedGameObject>> _visiblePlacedGameObjects;
        std::weak_ptr<PlacedGameObject> _hoveredPlacedGameObject;

        PlacedGameObjects();
        ~PlacedGameObjects();

        void addGameObject(std::weak_ptr<PlacedGameObject> placedGameObject);
        void removeGameObject(std::weak_ptr<PlacedGameObject> placedGameObject);
        void removeGameObjectsByPrefab(std::weak_ptr<GameObject> prefab);
        void removeGameObjectsByAnimations(int animationID);
        void replacePrefab(std::shared_ptr<GameObject> oldPrefab, std::shared_ptr<GameObject> newPrefab);

        void sort();

        void save(std::ofstream& saver);
        void load(std::ifstream& loader);

        virtual void cursorHover();
        virtual void update();
        virtual void draw();
    };



}