#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/PlacedGameObject.hpp"

namespace BuildingsEditor {
    class PlacedGameObjects {
    public:
        std::vector<std::shared_ptr<PlacedGameObject>> _visiblePlacedGameObjects;
        std::weak_ptr<PlacedGameObject> _hoveredPlacedGameObject;

        PlacedGameObjects();
        ~PlacedGameObjects();

        void addGameObject(std::weak_ptr<PlacedGameObject> placedGameObjectOn);
        void removeGameObject(std::weak_ptr<PlacedGameObject> placedGameObject);

        void sort();

        virtual void cursorHover();
        virtual void update();
        virtual void draw();
    };

}


