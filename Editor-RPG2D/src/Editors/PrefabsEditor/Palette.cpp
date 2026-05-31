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
				
				_slots->selectSlot(selectedSlotId);

				std::shared_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(slot->_object);
				editor->_object = prefab;
				editor->_animator = std::make_shared<Animator>(prefab->getAnimations(), 0.2f);

				editor->_main_panel->_name->setText(prefab->_name);
				//editor->_main_panel->_type->setText(prefab->_type.toString()); // TO-DO - enum to string
				
				// TO-DO 2 - start - must be - move to collider panel

				if (prefab->_collider->_type == ColliderType::Rectangular) {
					std::shared_ptr<RectangularCollider> collider = std::dynamic_pointer_cast<RectangularCollider>(prefab->_collider);
					editor->_collider_panel->_collider = collider;
					editor->_collider_panel->_type->setText(L"Rectangular");
					editor->_collider_panel->_x->setText(std::to_wstring(collider->_rect.position.x));
					editor->_collider_panel->_y->setText(std::to_wstring(collider->_rect.position.y));
					editor->_collider_panel->_w->setText(std::to_wstring(collider->_rect.size.x));
					editor->_collider_panel->_h->setText(std::to_wstring(collider->_rect.size.y));
					
				}
				else if (prefab->_collider->_type == ColliderType::Circular) {
					std::shared_ptr<CircularCollider> collider = std::dynamic_pointer_cast<CircularCollider>(prefab->_collider);
					editor->_collider_panel->_collider = collider;
					editor->_collider_panel->_type->setText(L"Circular");
					editor->_collider_panel->_x->setText(std::to_wstring(collider->_x));
					editor->_collider_panel->_y->setText(std::to_wstring(collider->_y));
					editor->_collider_panel->_w->setText(std::to_wstring(collider->_radiusX*2));
					editor->_collider_panel->_h->setText(std::to_wstring(collider->_radiusY*2));
				}

				// TO-DO 2 - end
				

			}
		);
	}
}
