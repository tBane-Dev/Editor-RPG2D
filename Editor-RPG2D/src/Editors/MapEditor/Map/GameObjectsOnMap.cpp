#include "DebugLog.hpp"
#include "Objects/Nature.hpp"
#include "Objects/Monster.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include <typeinfo>
#include "PrefabsManager.hpp"

GameObjectsOnMap::GameObjectsOnMap() {
	_gameObjectsOnMap.clear();
	 
	std::shared_ptr<Map> map = MapEditor::editor->_map;
	sf::IntRect mapRect = map->getRect();
	sf::Vector2i texSize = sf::Vector2i(prefabs_manager->getPrefab(L"tree_1")->getAnimations().lock()->getTexture()->_texture->getSize());

	std::shared_ptr<GameObjectOnMap> tree_1 = std::make_shared<GameObjectOnMap>(prefabs_manager->getPrefab(L"tree_1"));
	tree_1->setPosition(sf::Vector2i(0, 0));
	addGameObject(tree_1);
	
	std::shared_ptr<GameObjectOnMap> tree_2 = std::make_shared<GameObjectOnMap>(prefabs_manager->getPrefab(L"tree_1"));
	tree_2->setPosition(sf::Vector2i(mapRect.size.x - texSize.x, 0));
	addGameObject(tree_2);

	std::shared_ptr<GameObjectOnMap> tree_3 = std::make_shared<GameObjectOnMap>(prefabs_manager->getPrefab(L"tree_1"));
	tree_3->setPosition(sf::Vector2i(0, mapRect.size.y-texSize.y));
	addGameObject(tree_3);

	std::shared_ptr<GameObjectOnMap> tree_4 = std::make_shared<GameObjectOnMap>(prefabs_manager->getPrefab(L"tree_1"));
	tree_4->setPosition(sf::Vector2i(mapRect.size.x - texSize.x, mapRect.size.y - texSize.y));
	addGameObject(tree_4);

}

GameObjectsOnMap::~GameObjectsOnMap() {

}

void GameObjectsOnMap::addGameObject(std::weak_ptr<GameObjectOnMap> gameObjectOnMap) {

	_gameObjectsOnMap.push_back(gameObjectOnMap.lock());
}

void GameObjectsOnMap::removeGameObjectsByAnimations(int animationID)
{
	std::shared_ptr<Animations> animation = animations_manager->getAnimations(animationID).lock();

	if (!animation)
		return;

	std::erase_if(_gameObjectsOnMap,
		[&](const std::weak_ptr<GameObjectOnMap>& weakObject)
		{
			auto object = weakObject.lock();
			if (!object)
				return true;

			auto prefab = object->_prefab.lock();
			if (!prefab)
				return true;

			auto prefabAnimation = prefab->_animations.lock();
			if (!prefabAnimation)
				return true;

			return prefabAnimation == animation;
		});
}

void GameObjectsOnMap::removeGameObjectsByPrefab(std::weak_ptr<GameObject> prefab)
{
    auto prefabPtr = prefab.lock();

    if (!prefabPtr)
        return;

    std::erase_if(_gameObjectsOnMap,
        [&](const std::shared_ptr<GameObjectOnMap>& object)
        {
            if (!object)
                return true;

            auto objectPrefab = object->_prefab.lock();

            if (!objectPrefab)
                return true;

            return objectPrefab == prefabPtr;
        });
}


void GameObjectsOnMap::replacePrefab(std::shared_ptr<GameObject> oldPrefab, std::shared_ptr<GameObject> newPrefab) {
	if (!oldPrefab || !newPrefab)
		return;

	for (auto& objectOnMap : _gameObjectsOnMap) {

		std::shared_ptr<GameObject> currentPrefab = objectOnMap->_prefab.lock();

		if (currentPrefab != oldPrefab)
			continue;

		// get the position of the current object on the map
		sf::Vector2i position = objectOnMap->_position;

		if (currentPrefab->_type == ObjectType::Monster) {
			auto monster = std::dynamic_pointer_cast<Monster>(objectOnMap);
			position = monster->_basePosition - monster->_prefab.lock()->getOrigin();
		}
			

		// create a new object on the map with the new prefab
		std::shared_ptr<GameObjectOnMap> newObjectOnMap;

		if (newPrefab->_type == ObjectType::Monster) newObjectOnMap = std::make_shared<Monster>(newPrefab);
		else if (newPrefab->_type == ObjectType::Nature) newObjectOnMap = std::make_shared<Nature>(newPrefab);
		else newObjectOnMap = std::make_shared<GameObjectOnMap>(newPrefab);

		// set the position
		newObjectOnMap->setPosition(position);

		objectOnMap = newObjectOnMap;
	}

	sort();
}

void GameObjectsOnMap::sort() {
	std::sort(_gameObjectsOnMap.begin(), _gameObjectsOnMap.end(), [](const std::shared_ptr<GameObjectOnMap>& a, const std::shared_ptr<GameObjectOnMap>& b) {

		// OBJECT A
		sf::Vector2i posA = a->_position;
		if (!dynamic_pointer_cast<MonsterPrefab>(a->_prefab.lock())) {
			posA += a->_prefab.lock()->getOrigin();
		}

		if (a->_prefab.lock()->_collider->_type == ColliderType::Rectangular) {
			std::shared_ptr<RectangularCollider> collider = std::dynamic_pointer_cast<RectangularCollider>(a->_prefab.lock()->_collider);
			posA += collider->_rect.position + collider->_rect.size/2;
		}
		
		// OBJECT B
		sf::Vector2i posB = b->_position;
		if (!dynamic_pointer_cast<MonsterPrefab>(b->_prefab.lock())) {
			posB += b->_prefab.lock()->getOrigin();
		}

		if (b->_prefab.lock()->_collider->_type == ColliderType::Rectangular) {
			std::shared_ptr<RectangularCollider> collider = std::dynamic_pointer_cast<RectangularCollider>(b->_prefab.lock()->_collider);
			posB += collider->_rect.position + collider->_rect.size / 2;
		}
		
		if (posA.y == posB.y)
			return posA.x < posB.x;

		return posA.y < posB.y;
	});
}

void GameObjectsOnMap::update() {
	for (auto& object : _gameObjectsOnMap) {
		object->update();
	}
}

void GameObjectsOnMap::draw() {

	MapEditor::editor->_camera->setView();

	for (auto& object : _gameObjectsOnMap) {
		object->draw();
	}
}