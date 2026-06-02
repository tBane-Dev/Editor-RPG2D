#include "DebugLog.hpp"
#include "Objects/Monster.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include <typeinfo>
#include "PrefabsManager.hpp"

GameObjectsOnMap::GameObjectsOnMap() {
	_gameObjectsOnMap.clear();
	 
	std::shared_ptr<Map> map = MapEditor::editor->_map;
	sf::IntRect mapRect = map->getRect();
	sf::Vector2i texSize = sf::Vector2i(prefabs_manager->getPrefab(L"tree_1")->getAnimations()->getTexture()->_texture->getSize());

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

void GameObjectsOnMap::addGameObject(std::shared_ptr<GameObjectOnMap> gameObjectOnMap) {

	_gameObjectsOnMap.push_back(gameObjectOnMap);
}

void GameObjectsOnMap::removeGameObjectsByAnimations(int animationID) {

	std::shared_ptr<Animations> animation = animations_manager->getAnimations(animationID);

	std::erase_if(_gameObjectsOnMap, [&](const std::shared_ptr<GameObjectOnMap>& object)
		{ return object->_prefab->_animations.lock() == animation; }
	);
}

void GameObjectsOnMap::sort() {
	std::sort(_gameObjectsOnMap.begin(), _gameObjectsOnMap.end(), [](const std::shared_ptr<GameObjectOnMap>& a, const std::shared_ptr<GameObjectOnMap>& b) {
		
		sf::Vector2i posA = a->_position + (dynamic_pointer_cast<MonsterPrefab>(a->_prefab)? sf::Vector2i(0, 0) : a->_prefab->getOrigin());
		sf::Vector2i posB = b->_position + (dynamic_pointer_cast<MonsterPrefab>(b->_prefab)? sf::Vector2i(0, 0) : b->_prefab->getOrigin());

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