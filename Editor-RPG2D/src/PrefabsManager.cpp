#include "PrefabsManager.hpp"
#include "DebugLog.hpp"
#include "Objects/Monster.hpp"
#include "Objects/Nature.hpp"
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

void PrefabsManager::replacePrefab(std::shared_ptr<GameObject> oldPrefab, std::shared_ptr<GameObject> newPrefab) {
    if (!oldPrefab || !newPrefab)
        return;

    if(oldPrefab == newPrefab)
		return;

    std::erase_if(_prefabs, [&](const std::shared_ptr<GameObject>& prefab) {
        if (!prefab)
            return false;

        return prefab == oldPrefab;
        }
    );

    addPrefab(newPrefab);
}

void PrefabsManager::save(std::ofstream& saver) {

	BinaryWriter writer(saver);

    writer.write_int32(_prefabs.size());
    
    for (auto& prefab : _prefabs) {
        
        if (prefab->_type == ObjectType::Monster) {
			std::shared_ptr<MonsterPrefab> monsterPrefab = std::dynamic_pointer_cast<MonsterPrefab>(prefab);
			writer.write_int8((int)monsterPrefab->_type);
            writer.write_wstring(monsterPrefab->_name);
			saveCollider(monsterPrefab->getCollider(), saver);
            writer.write_Vector2i(monsterPrefab->_origin);
            writer.write_int32(monsterPrefab->_stepSize);
            writer.write_wstring((!monsterPrefab->_animations.expired()) ? monsterPrefab->_animations.lock()->_path : L"");

        }
        
        if (prefab->_type == ObjectType::Nature) {
			std::shared_ptr<NaturePrefab> naturePrefab = std::dynamic_pointer_cast<NaturePrefab>(prefab);
			writer.write_int8((int)naturePrefab->_type);
            writer.write_wstring(naturePrefab->_name);
            saveCollider(naturePrefab->getCollider(), saver);
			writer.write_Vector2i(naturePrefab->_origin);
            writer.write_wstring((!naturePrefab->_animations.expired()) ? naturePrefab->_animations.lock()->_path : L"");


        }
    }
}

void PrefabsManager::load(std::ifstream& loader) {
    
    _prefabs.clear();

    BinaryReader reader(loader);
    
    int prefabsCount = reader.read_int32();

    for (int i = 0; i < prefabsCount; i++) {
        int type = reader.read_int8();

        if(type == (int)ObjectType::Monster) {

            std::wstring name = reader.read_wstring();
            std::shared_ptr<Collider> collider = loadCollider(loader);
            sf::Vector2i origin = reader.read_Vector2i();
            int stepSize = reader.read_int32();
            std::wstring animationsPath = reader.read_wstring();
            std::shared_ptr<Animations> animations = animations_manager->getAnimations(animationsPath).lock();
            std::shared_ptr<GameObject> prefab = std::make_shared<MonsterPrefab>(name, animations, origin, stepSize, collider);
            addPrefab(prefab);
		}

        if(type == (int)ObjectType::Nature) {
            std::wstring name = reader.read_wstring();
            std::shared_ptr<Collider> collider = loadCollider(loader);
            sf::Vector2i origin = reader.read_Vector2i();
            std::wstring animationsPath = reader.read_wstring();
            std::shared_ptr<Animations> animations = animations_manager->getAnimations(animationsPath).lock();
            std::shared_ptr<GameObject> prefab = std::make_shared<NaturePrefab>(name, animations, origin, collider);
            addPrefab(prefab);
		}
    }
}



void PrefabsManager::loadPrefabs() {
	
    {
        std::shared_ptr<GameObject> prefab = std::make_shared<MonsterPrefab>(
            L"golem",
            animations_manager->getAnimations(L"assets\\tex\\monsters\\golem.png"),
            sf::Vector2i(62, 96),
            4,
			std::make_shared<CircularCollider>(62, 96, 48, 32)
        );
        addPrefab(prefab);
    }

    {
        std::shared_ptr<GameObject> prefab = std::make_shared<MonsterPrefab>(
            L"troll",
            animations_manager->getAnimations(L"assets\\tex\\monsters\\troll.png"),
            sf::Vector2i(62, 96),
            4,
            std::make_shared<CircularCollider>(62, 96, 48, 32)
        );
        addPrefab(prefab);
    }

    {
        std::shared_ptr<GameObject> prefab = std::make_shared<MonsterPrefab>(
            L"monster",
            animations_manager->getAnimations(L"assets\\tex\\monsters\\monster.png"),
            sf::Vector2i(62, 96),
            4,
            std::make_shared<CircularCollider>(62, 96, 48, 32)
        );
        addPrefab(prefab);
    }

    {
        std::shared_ptr<GameObject> prefab = std::make_shared<MonsterPrefab>(
            L"dziobak",
            animations_manager->getAnimations(L"assets\\tex\\monsters\\dziobak.png"),
            sf::Vector2i(62, 88),
            6,
			std::make_shared<CircularCollider>(62, 88, 48, 32)
        );
        addPrefab(prefab);
    }

    {
        std::shared_ptr<GameObject> prefab = std::make_shared<MonsterPrefab>(
            L"deer",
            animations_manager->getAnimations(L"assets\\tex\\monsters\\deer.png"),
            sf::Vector2i(65, 90),
            6,
            std::make_shared<CircularCollider>(65, 90, 48, 32)
        );
        addPrefab(prefab);
    }

    {
        std::shared_ptr<GameObject> prefab = std::make_shared<MonsterPrefab>(
            L"goblin",
            animations_manager->getAnimations(L"assets\\tex\\monsters\\goblin.png"),
            sf::Vector2i(62, 90),
            6,
			std::make_shared<CircularCollider>(62, 90, 24, 16)
        );
        addPrefab(prefab);
    }

    { 

        std::shared_ptr<GameObject> prefab = std::make_shared<MonsterPrefab>(
            L"bies",
            animations_manager->getAnimations(L"assets\\tex\\monsters\\bies.png"),
            sf::Vector2i(64, 90),
            6,
            std::make_shared<CircularCollider>(64, 90, 64, 48)
        );
        addPrefab(prefab);
    }

    {
        std::shared_ptr<GameObject> prefab = std::make_shared<MonsterPrefab>(
            L"hero",
            animations_manager->getAnimations(L"assets\\tex\\monsters\\hero.png"),
            sf::Vector2i(64, 192),
            12,
            std::make_shared<CircularCollider>(64, 192, 48, 32)
        );
        addPrefab(prefab);
    }

    {
        std::shared_ptr<GameObject> prefab = std::make_shared<NaturePrefab>(
            L"tree_1",
            animations_manager->getAnimations(L"assets\\tex\\tree_1.png"),
            sf::Vector2i(128, 214),
			std::make_shared<CircularCollider>(128, 214, 32, 24)
        );
        addPrefab(prefab);
    }

    {

        std::shared_ptr<GameObject> prefab = std::make_shared<NaturePrefab>(
            L"boulder_1",
            animations_manager->getAnimations(L"assets\\tex\\boulder_1.png"),
            sf::Vector2i(61, 68),
			std::make_shared<CircularCollider>(61, 68, 47, 36)
        );
        addPrefab(prefab);
    }

    {
        std::shared_ptr<GameObject> prefab = std::make_shared<NaturePrefab>(
            L"boulder_2",
            animations_manager->getAnimations(L"assets\\tex\\boulder_2.png"),
            sf::Vector2i(62, 70),
			std::make_shared<CircularCollider>(62, 70, 47, 37)
        );
        addPrefab(prefab);
    }
    

    std::vector<std::shared_ptr<GameObject>> p;
    for (auto& prefab : _prefabs) {
        p.push_back(prefab);
    }

    _prefabs = p;
}
std::shared_ptr<PrefabsManager> prefabs_manager = nullptr;
