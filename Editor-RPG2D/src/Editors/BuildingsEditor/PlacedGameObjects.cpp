#include "DebugLog.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/PlacedGameObjects.hpp"
#include <typeinfo>
#include "PrefabsManager.hpp"

namespace BuildingsEditor {

	PlacedGameObjects::PlacedGameObjects() {
		_visiblePlacedGameObjects.clear();
		_hoveredPlacedGameObject = std::weak_ptr<PlacedGameObject>();
	}

	PlacedGameObjects::~PlacedGameObjects() {

	}

	void PlacedGameObjects::addGameObject(std::weak_ptr<PlacedGameObject> placedGameObject) {

		_visiblePlacedGameObjects.push_back(placedGameObject.lock());
		if (placedGameObject.lock()->_type == ObjectType::Building) {
			std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(placedGameObject.lock());
			building->addWallsToVisibleGameObjects();
			building->addSkeletsToVisibleGameObjects();
			building->addOutsideToVisibleGameObjects();
		}
	}

	void PlacedGameObjects::removeGameObject(std::weak_ptr<PlacedGameObject> placedGameObject) {
		std::shared_ptr<PlacedGameObject> objectToRemove = placedGameObject.lock();

		if (!objectToRemove)
			return;

		std::erase_if(_visiblePlacedGameObjects,
			[&](const std::shared_ptr<PlacedGameObject>& object)
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

	void PlacedGameObjects::sort() {

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

		std::sort(_visiblePlacedGameObjects.begin(), _visiblePlacedGameObjects.end(), [&types, getIndex](const std::shared_ptr<PlacedGameObject>& a, const std::shared_ptr<PlacedGameObject>& b) {



			// OBJECT A - POSITION
			sf::Vector2i posA = a->_position;

			if (!a->_prefab.expired()) {
				if (a->_prefab.lock()->_type == ObjectType::Skelet) {
					std::shared_ptr<RectangularCollider> collider = std::dynamic_pointer_cast<RectangularCollider>(a->_prefab.lock()->getCollider());
					posA.x += collider->_rect.position.x + collider->_rect.size.x / 2;
					posA.y += collider->_rect.position.y;
				}
				else if (a->_prefab.lock()->_collider && a->_prefab.lock()->_collider->_type == ColliderType::Rectangular) {
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
				if (b->_prefab.lock()->_type == ObjectType::Skelet) {
					std::shared_ptr<RectangularCollider> collider = std::dynamic_pointer_cast<RectangularCollider>(b->_prefab.lock()->getCollider());
					posB.x += collider->_rect.position.x + collider->_rect.size.x / 2;
					posB.y += collider->_rect.position.y;
				}
				else if (b->_prefab.lock()->_collider && b->_prefab.lock()->_collider->_type == ColliderType::Rectangular) {
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

	void PlacedGameObjects::cursorHover() {
		for (auto& object : _visiblePlacedGameObjects) {
			if (object->_type == ObjectType::Building) {
				object->cursorHover();
			}
		}

		for (auto& object : _visiblePlacedGameObjects) {
			if (object->_type != ObjectType::Building) {
				object->cursorHover();
			}
		}
	}

	void PlacedGameObjects::update() {

		for (auto& object : _visiblePlacedGameObjects) {
			object->update();
		}
	}

	void PlacedGameObjects::draw() {

		for (auto& object : _visiblePlacedGameObjects) {
			if (object->_type == ObjectType::Building) {
				std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(object);
				if (building) {
					std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(building->_prefab.lock());
					if (buildingPrefab)
						buildingPrefab->drawOnlyCollider(*Main::render_window, building->getPosition());

					building->draw(); // draw selected frame
				}
			}
		}

		for (auto& object : _visiblePlacedGameObjects) {
			if (object->_type == ObjectType::Building) {
				std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(object);
				if (building) {
					std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(building->_prefab.lock());
					if (buildingPrefab)
						buildingPrefab->drawOnlyFloor(*Main::render_window, building->getPosition());
				}
			}
		}

		for (auto& object : _visiblePlacedGameObjects) {
			if (object->_type != ObjectType::Building) {
				object->draw();
			}
		}

	}
}