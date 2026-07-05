#include "DebugLog.hpp"
#include "Objects/Nature.hpp"
#include "Objects/Monster.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include <typeinfo>
#include "PrefabsManager.hpp"
#include "BinaryWriter.hpp"
#include "BinaryReader.hpp"

GameObjectsOnMap::GameObjectsOnMap() {
	_visibleGameObjectsOnMap.clear();
	 
	std::shared_ptr<Map> map = MapEditor::editor->_map;
	sf::IntRect mapRect = map->getRect();
	sf::Vector2i texSize = sf::Vector2i(prefabs_manager->getPrefab(L"tree_1")->getAnimations().lock()->getTexture()->_texture->getSize());

	std::shared_ptr<GameObjectOnMap> tree_1 = std::make_shared<Nature>(prefabs_manager->getPrefab(L"tree_1"));
	tree_1->setPosition(sf::Vector2i(0, 0));
	MapEditor::editor->_map->getChunkByGlobalPosition(tree_1->getPosition())->addGameObjectOnMap(tree_1);
	
	std::shared_ptr<GameObjectOnMap> tree_2 = std::make_shared<Nature>(prefabs_manager->getPrefab(L"tree_1"));
	tree_2->setPosition(sf::Vector2i(mapRect.size.x - texSize.x, 0));
	MapEditor::editor->_map->getChunkByGlobalPosition(tree_2->getPosition())->addGameObjectOnMap(tree_2);

	std::shared_ptr<GameObjectOnMap> tree_3 = std::make_shared<Nature>(prefabs_manager->getPrefab(L"tree_1"));
	tree_3->setPosition(sf::Vector2i(0, mapRect.size.y-texSize.y));
	MapEditor::editor->_map->getChunkByGlobalPosition(tree_3->getPosition())->addGameObjectOnMap(tree_3);

	std::shared_ptr<GameObjectOnMap> tree_4 = std::make_shared<Nature>(prefabs_manager->getPrefab(L"tree_1"));
	tree_4->setPosition(sf::Vector2i(mapRect.size.x - texSize.x, mapRect.size.y - texSize.y));
	MapEditor::editor->_map->getChunkByGlobalPosition(tree_4->getPosition())->addGameObjectOnMap(tree_4);

}

GameObjectsOnMap::~GameObjectsOnMap() {

}

void GameObjectsOnMap::addGameObject(std::weak_ptr<GameObjectOnMap> gameObjectOnMap) {

	_visibleGameObjectsOnMap.push_back(gameObjectOnMap.lock());
}

void GameObjectsOnMap::removeGameObject(std::weak_ptr<GameObjectOnMap> gameObjectOnMap) {
	std::shared_ptr<GameObjectOnMap> objectToRemove = gameObjectOnMap.lock();
	
	if (!objectToRemove)
		return;

	std::erase_if(_visibleGameObjectsOnMap,
		[&](const std::shared_ptr<GameObjectOnMap>& object)
		{
			return object == objectToRemove;
		});
}

void GameObjectsOnMap::removeGameObjectsByAnimations(int animationID)
{
	std::shared_ptr<Animations> animation = animations_manager->getAnimations(animationID).lock();

	if (!animation)
		return;

	std::erase_if(_visibleGameObjectsOnMap,
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

    std::erase_if(_visibleGameObjectsOnMap,
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

	for (auto& objectOnMap : _visibleGameObjectsOnMap) {

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
		if(newObjectOnMap->_type == ObjectType::Monster) {
			newObjectOnMap->setPosition(position + newPrefab->getOrigin());
		}
		else
			newObjectOnMap->setPosition(position);

		objectOnMap = newObjectOnMap;
	}

	sort();
}

void GameObjectsOnMap::sort() {
	std::sort(_visibleGameObjectsOnMap.begin(), _visibleGameObjectsOnMap.end(), [](const std::shared_ptr<GameObjectOnMap>& a, const std::shared_ptr<GameObjectOnMap>& b) {

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

void GameObjectsOnMap::save(std::ofstream& saver) {
	
	BinaryWriter writer(saver);

	int32_t count = 0;
	for(auto& chunk : MapEditor::editor->_map->_chunks) {
		for (auto& object : chunk->_gameObjectsOnMap) {
			if (object)
				count++;
		}
	}

	writer.write_int32(count);

	for(auto& chunk : MapEditor::editor->_map->_chunks) {
		for (auto& object : chunk->_gameObjectsOnMap) {

			if (!object) continue;

			if (object->_type == ObjectType::None) {
				writer.write_int8((int8_t)object->_type);
				writer.write_Vector2i(object->_position);
				writer.write_int8(object->_animator->_animation);
				writer.write_int8(object->_animator->_frame);
				writer.write_float(object->_animator->_timer);
			}

			else if (object->_type == ObjectType::Nature) {
				std::shared_ptr<Nature> nature = std::dynamic_pointer_cast<Nature>(object);
				if (nature) {
					writer.write_int8((int8_t)nature->_type);
					writer.write_wstring((!nature->_prefab.expired()) ? nature->_prefab.lock()->_name : L"");
					writer.write_Vector2i(nature->_position);
					writer.write_int8(nature->_animator->_animation);
					writer.write_int8(nature->_animator->_frame);
					writer.write_float(nature->_animator->_timer);
				}
			}

			else if (object->_type == ObjectType::Monster) {
				std::shared_ptr<Monster> monster = std::dynamic_pointer_cast<Monster>(object);
				if (monster) {
					writer.write_int8((int8_t)monster->_type);
					writer.write_wstring((!monster->_prefab.expired()) ? monster->_prefab.lock()->_name : L"");
					writer.write_Vector2i(monster->_basePosition);
					writer.write_Vector2i(monster->_position);
					writer.write_int8((int)monster->_direction);
					writer.write_int8((int)monster->_state);
					writer.write_int8(monster->_animator->_animation);
					writer.write_int8(monster->_animator->_frame);
					writer.write_float(monster->_animator->_timer);
				}
			}

		}
	}
	
}

void GameObjectsOnMap::load(std::ifstream& loader) {
	BinaryReader reader(loader);
	
	_visibleGameObjectsOnMap.clear();

	int objectsCount = reader.read_int32();

	for (int i = 0; i < objectsCount; i++) {
		ObjectType type = (ObjectType)reader.read_int8();

		if (type == ObjectType::None) {
			std::shared_ptr<GameObjectOnMap> object = std::make_shared<GameObjectOnMap>(std::weak_ptr<GameObject>());
			object->setPosition(reader.read_Vector2i());
			object->_animator->_animation = reader.read_int8();
			object->_animator->_frame = reader.read_int8();
			object->_animator->_timer = reader.read_float();
			std::shared_ptr<Chunk> chunk = MapEditor::editor->_map->getChunkByGlobalPosition(object->_position);
			if (chunk) chunk->addGameObjectOnMap(object);
		}

		if( type == ObjectType::Nature) {
			std::wstring prefabName = reader.read_wstring();
			std::shared_ptr<GameObject> prefab = prefabs_manager->getPrefab(prefabName);
			std::shared_ptr<Nature> nature = std::make_shared<Nature>(prefab);
			nature->setPosition(reader.read_Vector2i());
			nature->_animator->_animation = reader.read_int8();
			nature->_animator->_frame = reader.read_int8();
			nature->_animator->_timer = reader.read_float();
			std::shared_ptr<Chunk> chunk = MapEditor::editor->_map->getChunkByGlobalPosition(nature->_position);
			if(chunk) chunk->addGameObjectOnMap(nature);
		}

		if( type == ObjectType::Monster) {
			std::wstring prefabName = reader.read_wstring();
			std::shared_ptr<GameObject> prefab = prefabs_manager->getPrefab(prefabName);
			std::shared_ptr<Monster> monster = std::make_shared<Monster>(prefab);
			monster->_basePosition = reader.read_Vector2i();
			monster->setPosition(reader.read_Vector2i());
			monster->_direction = (Direction)reader.read_int8();
			monster->_state = (MonsterState)reader.read_int8();
			monster->_animator->_animation = reader.read_int8();
			monster->_animator->_frame = reader.read_int8();
			monster->_animator->_timer = reader.read_float();
			std::shared_ptr<Chunk> chunk = MapEditor::editor->_map->getChunkByGlobalPosition(monster->_basePosition);
			if (chunk) chunk->addGameObjectOnMap(monster);
		}
	}

	MapEditor::editor->_map->setVisibleChunks();
}

void GameObjectsOnMap::update() {
	for (auto& object : _visibleGameObjectsOnMap) {
		object->update();
	}
}

void GameObjectsOnMap::draw() {

	MapEditor::editor->_camera->setView();

	for (auto& object : _visibleGameObjectsOnMap) {
		object->draw();
	}
}