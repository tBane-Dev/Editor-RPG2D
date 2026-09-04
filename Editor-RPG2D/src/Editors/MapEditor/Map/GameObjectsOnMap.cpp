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
	_hoveredGameObjectOnMap = std::weak_ptr<GameObjectOnMap>();
	 
	
}

GameObjectsOnMap::~GameObjectsOnMap() {

}

void GameObjectsOnMap::addGameObject(std::weak_ptr<GameObjectOnMap> gameObjectOnMap) {

	_visibleGameObjectsOnMap.push_back(gameObjectOnMap.lock());
	if(gameObjectOnMap.lock()->_type == ObjectType::Building) {
		std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(gameObjectOnMap.lock());
		building->addWallsToVisibleGameObjects();
		building->addSkeletsToVisibleGameObjects();
		building->addOutsideToVisibleGameObjects();
	}
}

void GameObjectsOnMap::removeGameObject(std::weak_ptr<GameObjectOnMap> gameObjectOnMap) {
	std::shared_ptr<GameObjectOnMap> objectToRemove = gameObjectOnMap.lock();
	
	if (!objectToRemove)
		return;

	std::erase_if(_visibleGameObjectsOnMap,
		[&](const std::shared_ptr<GameObjectOnMap>& object)
		{
			if (objectToRemove->_type == ObjectType::Building && object->_type == ObjectType::Wall) {
				std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(objectToRemove);
				std::shared_ptr<Wall> wall = std::dynamic_pointer_cast<Wall>(object);
				return wall->_building.lock() == building;
			}

			if (objectToRemove->_type == ObjectType::Building && object->_type == ObjectType::Skelet) {
				std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(objectToRemove);
				std::shared_ptr<Skelet> skelet = std::dynamic_pointer_cast<Skelet>(object);
				return skelet->_building.lock() == building;
			}

			if (objectToRemove->_type == ObjectType::Building && object->_type == ObjectType::Outside) {
				std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(objectToRemove);
				std::shared_ptr<Outside> outside = std::dynamic_pointer_cast<Outside>(object);
				return outside->_building.lock() == building;
			}

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

	for (auto& chunk : MapEditor::editor->_map->_chunks) {
		for (auto& objectOnMap : chunk->_gameObjectsOnMap) {

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
			if (newObjectOnMap->_type == ObjectType::Monster) {
				newObjectOnMap->setPosition(position + newPrefab->getOrigin());
			}
			else
				newObjectOnMap->setPosition(position);

			objectOnMap = newObjectOnMap;
		}
	}

	MapEditor::editor->setVisibleChunks();
	sort();
}

void GameObjectsOnMap::sort() {

	std::vector<ObjectType> types = {
		ObjectType::Skelet,
		ObjectType::WallMounted,
		ObjectType::Window,
		ObjectType::Door,
		ObjectType::Roof,
		ObjectType::Outside
	};

	auto getIndex = [&types](ObjectType type) -> int {
		auto it = std::find(types.begin(), types.end(), type);
		if (it != types.end())
			return it - types.begin();

		return -1; // other types
		};

	std::sort(_visibleGameObjectsOnMap.begin(), _visibleGameObjectsOnMap.end(), [&types,getIndex](const std::shared_ptr<GameObjectOnMap>& a, const std::shared_ptr<GameObjectOnMap>& b) {

		

		// OBJECT A - POSITION
		sf::Vector2i posA = a->_position;

		if (!a->_prefab.expired()) {
			if (a->_prefab.lock()->_collider && a->_prefab.lock()->_collider->_type == ColliderType::Rectangular) {
				std::shared_ptr<RectangularCollider> collider = std::dynamic_pointer_cast<RectangularCollider>(a->_prefab.lock()->getCollider());
				posA += collider->_rect.position + collider->_rect.size / 2;
			}
			else if (a->_prefab.lock()->_type != ObjectType::Monster) {
				posA += a->_prefab.lock()->getOrigin();
			}
		}
		
		// OBJECT B - POSITION
		sf::Vector2i posB = b->_position;

		if (!b->_prefab.expired()) {
			if (b->_prefab.lock()->_collider && b->_prefab.lock()->_collider->_type == ColliderType::Rectangular) {
				std::shared_ptr<RectangularCollider> collider = std::dynamic_pointer_cast<RectangularCollider>(b->_prefab.lock()->getCollider());
				posB += collider->_rect.position + collider->_rect.size / 2;
			}
			else if (b->_prefab.lock()->_type != ObjectType::Monster) {
				posB += b->_prefab.lock()->getOrigin();
			}
		}


		if (posA.y == posB.y) {

			int aIndex = getIndex(a->_type);
			int bIndex = getIndex(b->_type);

			if (aIndex != -1 && bIndex != -1) {
				if (aIndex < bIndex) return true;
				if (aIndex > bIndex) return false;
			}

			return posA.x < posB.x;
		}

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

	_visibleGameObjectsOnMap.clear();

	BinaryReader reader(loader);

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

void GameObjectsOnMap::cursorHover() {
	for (auto& object : _visibleGameObjectsOnMap) {
		if (object->_type == ObjectType::Building) {
			object->cursorHover();
		}
	}

	for (auto& object : _visibleGameObjectsOnMap) {
		if (object->_type != ObjectType::Building) {
			object->cursorHover();
		}
	}
}

void GameObjectsOnMap::update() {

	for (auto& object : _visibleGameObjectsOnMap) {
		object->update();
	}
}

void GameObjectsOnMap::draw() {

	MapEditor::editor->_camera->setView();

	for (auto& object : _visibleGameObjectsOnMap) {
		if (object->_type == ObjectType::Building) {
			std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(object);
			if (building) {
				std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(building->_prefab.lock());
				if(buildingPrefab)
					buildingPrefab->drawOnlyCollider(*Main::render_window, building->getPosition());

				building->draw(); // draw selected frame
			}
		}
	}

	for(auto& object : _visibleGameObjectsOnMap) {
		if (object->_type == ObjectType::Building) {
			std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(object);
			if (building) {
				std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(building->_prefab.lock());
				if (buildingPrefab)
					buildingPrefab->drawOnlyFloor(*Main::render_window, building->getPosition());
			}
		}
	}

	static int i = 0;
	for (auto& object : _visibleGameObjectsOnMap) {
		if(object->_type != ObjectType::Building) {
			object->draw();
		}
	}

}