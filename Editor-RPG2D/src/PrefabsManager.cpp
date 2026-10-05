#include "PrefabsManager.hpp"
#include "DebugLog.hpp"
#include "Objects/Monster.hpp"
#include "Objects/Nature.hpp"
#include "Objects/Building/Wall.hpp"
#include "Objects/Building/Window.hpp"
#include "Objects/Building/WallMounted.hpp"
#include "Objects/Building/Building.hpp"
#include "BinaryWriter.hpp"
#include "BinaryReader.hpp"
#include "Objects/Collider.hpp"

PrefabsManager::PrefabsManager() {
	_prefabs.clear();
}

PrefabsManager::~PrefabsManager() {

}

void PrefabsManager::addPrefab(std::shared_ptr<GameObject> prefab) {
	_prefabs.push_back(prefab);
}

void PrefabsManager::removePrefab(std::weak_ptr<GameObject> prefab)
{
    auto prefabPtr = prefab.lock();
    if (!prefabPtr)
        return;

    std::erase_if(_prefabs,
        [&](const std::shared_ptr<GameObject>& p)
        {
			if(p == prefabPtr)
                DebugLog(L"usunieto prefab: " + p->_name);
            
            return p == prefabPtr;
        });
}

std::shared_ptr<GameObject> PrefabsManager::getPrefab(std::wstring path) {
	for (auto& prefab : _prefabs) {
		if (prefab->_name == path) {
			return prefab;
		}
	}
	return nullptr;
}

std::vector<std::shared_ptr<GameObject>>& PrefabsManager::getAllPrefabs() {
	return _prefabs;
}

std::vector<std::shared_ptr<GameObject>> PrefabsManager::getPrefabs(ObjectType type) {
	std::vector<std::shared_ptr<GameObject>> prefabsOfType;

    for (auto& prefab : _prefabs) {
        if(prefab->_type == type) {
            prefabsOfType.push_back(prefab);
		}
    }

    return prefabsOfType;
}

void PrefabsManager::removePrefabsByAnimations(int animationID) {

    std::shared_ptr<Animations> animation =
        animations_manager->getAnimations(animationID).lock();

    if (!animation)
        return;

    std::erase_if(_prefabs, [&](const std::shared_ptr<GameObject>& prefab) {
            if (!prefab)
                return false;

            return prefab->_animations.lock() == animation;
        }
    );
}

void PrefabsManager::replacePrefab(
    std::shared_ptr<GameObject> oldPrefab,
    std::shared_ptr<GameObject> newPrefab
) {
    if (!oldPrefab || !newPrefab)
        return;

    if (oldPrefab == newPrefab)
        return;

    auto it = std::find(_prefabs.begin(), _prefabs.end(), oldPrefab);

    if (it == _prefabs.end())
        return;

    *it = newPrefab;
}

void PrefabsManager::saveFromProject(std::ofstream& saver) {

	BinaryWriter writer(saver);

    int32_t count = 0;

    for (auto& prefab : _prefabs) {

        if (!prefab)
            continue;

        if (prefab->_type == ObjectType::Monster ||
            prefab->_type == ObjectType::Nature) {
            count++;
        }
    }

    writer.write_int32(count);
    
    for (auto& prefab : _prefabs) {
        
        if (prefab->_type == ObjectType::Monster) {
			std::shared_ptr<MonsterPrefab> monsterPrefab = std::dynamic_pointer_cast<MonsterPrefab>(prefab);
			writer.write_int8((int)monsterPrefab->_type);
            writer.write_wstring(monsterPrefab->_name);
			saveCollider(monsterPrefab->getCollider(), saver);
			saveMesh(monsterPrefab->getMesh(), saver);
            writer.write_Vector2i(monsterPrefab->_origin);
            writer.write_int32(monsterPrefab->_stepSize);
            writer.write_wstring((!monsterPrefab->_animations.expired()) ? monsterPrefab->_animations.lock()->_path : L"");
        }
        
        if (prefab->_type == ObjectType::Nature) {
			std::shared_ptr<NaturePrefab> naturePrefab = std::dynamic_pointer_cast<NaturePrefab>(prefab);
			writer.write_int8((int)naturePrefab->_type);
            writer.write_wstring(naturePrefab->_name);
            saveCollider(naturePrefab->getCollider(), saver);
			saveMesh(naturePrefab->getMesh(), saver);
			writer.write_Vector2i(naturePrefab->_origin);
            writer.write_wstring((!naturePrefab->_animations.expired()) ? naturePrefab->_animations.lock()->_path : L"");
        }

        

    }

}

void PrefabsManager::loadFromProject(std::ifstream& loader) {
    
    _prefabs.clear();

    // Loaded animations
    DebugLog(L"Loading prefabs:");

    BinaryReader reader(loader);
    
    int prefabsCount = reader.read_int32();

    for (int i = 0; i < prefabsCount; i++) {
        int type = reader.read_int8();

        if(type == (int)ObjectType::Monster) {

            std::wstring name = reader.read_wstring();
            std::shared_ptr<Collider> collider = loadCollider(loader);
            std::shared_ptr<Mesh> mesh = loadMesh(loader);
            sf::Vector2i origin = reader.read_Vector2i();
            int stepSize = reader.read_int32();
            std::wstring animationsPath = reader.read_wstring();
            std::shared_ptr<Animations> animations = animations_manager->getAnimations(animationsPath).lock();
            std::shared_ptr<GameObject> prefab = std::make_shared<MonsterPrefab>(name, animations, origin, stepSize, collider, mesh);
            addPrefab(prefab);
			DebugStat(name);
		}

        if(type == (int)ObjectType::Nature) {
            std::wstring name = reader.read_wstring();
            std::shared_ptr<Collider> collider = loadCollider(loader);
			std::shared_ptr<Mesh> mesh = loadMesh(loader);
            sf::Vector2i origin = reader.read_Vector2i();
            std::wstring animationsPath = reader.read_wstring();
            std::shared_ptr<Animations> animations = animations_manager->getAnimations(animationsPath).lock();
            std::shared_ptr<GameObject> prefab = std::make_shared<NaturePrefab>(name, animations, origin, collider, mesh);
            addPrefab(prefab);
            DebugStat(name);
		}

        

        
    }
}

void PrefabsManager::loadBuildingsPartsPrefabs() {

    struct data {
        std::wstring name;
        std::wstring animationsPath;
	};
    
    data doors_datas[] = {
        { L"Wooden Door", L"assets\\tex\\buildings\\doors\\wooden_door.png" },
        { L"Stone Door", L"assets\\tex\\buildings\\doors\\stone_door.png" },
	};

    data windows_datas[] = {
        { L"Wooden Window 1", L"assets\\tex\\buildings\\windows\\wooden_window_1.png" },
        { L"Wooden Window 2", L"assets\\tex\\buildings\\windows\\wooden_window_2.png" },
        { L"Wooden Window 3", L"assets\\tex\\buildings\\windows\\wooden_window_3.png" },
    };

    data wall_mounted_datas[] = {
        { L"Lit Torch", L"assets\\tex\\buildings\\wall_mounted\\lit_torch.png" },
    };

    DebugLog(L"Loading buildings parts prefabs:");

    for (auto& data : doors_datas) {
        std::shared_ptr<Collider> doors_collider = std::make_shared<RectangularCollider>(0, 0, 64, 16);
        std::shared_ptr<Mesh> doors_mesh = std::make_shared<Mesh>(1.0f, 1.0f);
        doors_mesh->addShape(std::make_shared<Shape>());
        doors_mesh->getShape(0)->addPoint(sf::Vector2i(0, 0-48));
        doors_mesh->getShape(0)->addPoint(sf::Vector2i(64, 0-48));
        doors_mesh->getShape(0)->addPoint(sf::Vector2i(64, 64-48));
        doors_mesh->getShape(0)->addPoint(sf::Vector2i(0, 64-48));
        std::shared_ptr<GameObject> doors_prefab = std::make_shared<DoorPrefab>(data.name, buildings_parts_animations_manager->getAnimations(data.animationsPath), sf::Vector2i(32, 32), doors_collider, doors_mesh);
        doors_prefab->_type = ObjectType::Door;
        addPrefab(doors_prefab);
		DebugStat(data.name);
    }

    for (auto& data : windows_datas) {
        std::shared_ptr<Collider> window_collider = std::make_shared<RectangularCollider>(0, 0, 32, 32);
        std::shared_ptr<Mesh> window_mesh = std::make_shared<Mesh>(1.0f, 1.0f);
        window_mesh->addShape(std::make_shared<Shape>());
        window_mesh->getShape(0)->addPoint(sf::Vector2i(0, 0));
        window_mesh->getShape(0)->addPoint(sf::Vector2i(32, 0));
        window_mesh->getShape(0)->addPoint(sf::Vector2i(32, 32));
        window_mesh->getShape(0)->addPoint(sf::Vector2i(0, 32));
        std::shared_ptr<WindowPrefab> window_prefab = std::make_shared<WindowPrefab>(data.name, buildings_parts_animations_manager->getAnimations(data.animationsPath), sf::Vector2i(32, 32), window_collider, window_mesh);
        addPrefab(window_prefab);
        DebugStat(data.name);
    }
    
    for (auto& data : wall_mounted_datas) { 
        std::shared_ptr<Collider> wall_mounted_collider = std::make_shared<RectangularCollider>(0, 0, 32, 32);
        std::shared_ptr<Mesh> wall_mounted_mesh = std::make_shared<Mesh>(1.0f, 1.0f);
        wall_mounted_mesh->addShape(std::make_shared<Shape>());
        wall_mounted_mesh->getShape(0)->addPoint(sf::Vector2i(0, 0));
        wall_mounted_mesh->getShape(0)->addPoint(sf::Vector2i(32, 0));
        wall_mounted_mesh->getShape(0)->addPoint(sf::Vector2i(32, 32));
        wall_mounted_mesh->getShape(0)->addPoint(sf::Vector2i(0, 32));
        std::shared_ptr<WallMountedPrefab> wall_mounted_prefab = std::make_shared<WallMountedPrefab>(data.name, buildings_parts_animations_manager->getAnimations(data.animationsPath), sf::Vector2i(32, 32), wall_mounted_collider, wall_mounted_mesh);
        addPrefab(wall_mounted_prefab);
        DebugStat(data.name);
    }
}

std::shared_ptr<PrefabsManager> prefabs_manager = nullptr;
std::shared_ptr<PrefabsManager> buildings_parts_prefabs_manager = nullptr;
