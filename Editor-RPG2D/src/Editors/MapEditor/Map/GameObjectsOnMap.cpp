#include "DebugLog.hpp"
#include "Editors/MapEditor/Objects/Monster.hpp"
#include "Editors/MapEditor/MapEditor.hpp"

GameObjectsOnMap::GameObjectsOnMap() {
	_gameObjectsOnMap.clear();
}

GameObjectsOnMap::~GameObjectsOnMap() {

}

void GameObjectsOnMap::addGameObject(std::shared_ptr<GameObjectOnMap> gameObjectOnMap) {
	_gameObjectsOnMap.push_back(gameObjectOnMap);
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

	map_editor->_camera->setView();

	for (auto& object : _gameObjectsOnMap) {
		object->draw();
	}
}