#include "Editors/PrefabsEditor/Palette.hpp"
#include "Editors/PrefabsEditor/Editor.hpp"

namespace PrefabsEditor {

	Palette::Palette() : Components::Palette() {

		_categories = std::make_shared<Categories>();

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\natures.png"),
			ObjectType::Nature,
			[this]() { loadAll(ObjectType::Nature); } // TO-DO - must be - selectCategory
		);


		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\monsters.png"),
			ObjectType::Monster,
			[this]() { loadAll(ObjectType::Monster); } // TO-DO - must be - selectCategory
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr,
			ObjectType::None,
			[this]() { loadAll(ObjectType::None); } // TO-DO - must be - selectCategory
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr,
			ObjectType::None,
			[this]() { loadAll(ObjectType::None); } // TO-DO - must be - selectCategory
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr,
			ObjectType::None,
			[this]() { loadAll(ObjectType::None); } // TO-DO - must be - selectCategory
		);

		_tools = std::make_shared<Tools>();
		_slots = std::make_shared<Slots>();

		setPosition(sf::Vector2i(_rect.position));

		// set the active group
		loadAll(ObjectType::Nature); // TO-DO - must be - selectCategory

		//_slots->setCategory(ObjectType::Terrain);
	}

	Palette::~Palette() {

	}

	void Palette::loadAll(ObjectType type) {
		_categories->setCategory(type);

		sf::Vector2i slotsPosition = sf::Vector2i(_rect.position.x, _categories->getPosition().y + _categories->getSize().y);
		if (_categories->_selectedType == ObjectType::Terrain) {
			slotsPosition.y += _tools->getSize().y + 16;
		}
		_slots->setPosition(slotsPosition);
		_slots->setCategory(type);

		_slots->setFunction(
			[this](std::shared_ptr<Slot> slot, int selectedSlotId) {
				std::shared_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(slot->_object);
				editor->_object = prefab;
				editor->_animator = std::make_shared<Animator>(prefab->getAnimations(), 0.2f);
				editor->_palette->loadAll(prefab->_type);
				editor->_palette->_slots->updateObjects();
				_slots->selectSlot(selectedSlotId);
			}
		);
	}
}
