#include "Editors/PrefabsEditor/MainPanel.hpp"
#include "Editors/PrefabsEditor/Editor.hpp"
#include "Editors/MapEditor/Editor.hpp"

#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"
#include "Objects/Monster.hpp"
#include "Objects/Nature.hpp"
#include "PrefabsManager.hpp"

namespace PrefabsEditor {

	MainPanel::MainPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 850), sf::Vector2i(margin.x, PrefabsEditor::editor->_main_menu->getSize().y + margin.y)) {

		_nameText = std::make_unique<sf::Text>(basicFont, L"Name", 20);
		_nameText->setFillColor(basic_text_color);
		_nameText->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 24));

		_name = std::make_shared<TextInput>(sf::Vector2i(384, 48), L"Type here", 20, 20);
		_name->setPosition(sf::Vector2i(_rect.position.x + 16, _nameText->getGlobalBounds().position.y + 32));

		_typeText = std::make_unique<sf::Text>(basicFont, L"Type", 20);
		_typeText->setFillColor(basic_text_color);
		_typeText->setPosition(sf::Vector2f(_rect.position.x + 16, _name->getPosition().y + _name->getSize().y + 32));

		_type = std::make_shared<TextInput>(sf::Vector2i(384, 48), L"Type here", 20, 20);
		_type->setPosition(sf::Vector2i(_rect.position.x + 16, _typeText->getGlobalBounds().position.y + 32));
		_type->setText(L"None");

		sf::Vector2i startPosition;
		startPosition.x = getPosition().x + (getSize().x - 380) / 2;
		startPosition.y = _margin + getPosition().y + 512 + 16;
		int distance_between_buttons = 16;

		_save_prefab = std::make_shared<ButtonWithTextAndSprite>(
			L"Save prefab",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_inactive.png"),
			sf::Vector2i(startPosition.x, startPosition.y)
		);

		_add_prefab = std::make_shared<ButtonWithTextAndSprite>(
			L"Add prefab",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_inactive.png"),
			sf::Vector2i(startPosition.x, startPosition.y + _save_prefab->getSize().y + distance_between_buttons)
		);

		_remove_prefab = std::make_shared<ButtonWithTextAndSprite>(
			L"Remove prefab",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_inactive.png"),
			sf::Vector2i(startPosition.x, _add_prefab->getPosition().y + _add_prefab->getSize().y + distance_between_buttons)
		);

		setButtonsActivity();
		setTooltips();

		_name->_onEditedFunction = [this]() {
			editor->_main_panel->setButtonsActivity();
			editor->_main_panel->setTooltips();
		};

		_type->_onEditedFunction = [this]() {
			editor->_main_panel->setButtonsActivity();
			editor->_main_panel->setTooltips();
			};

		_save_prefab->_onclick_func = [this]() {

			if (PrefabsEditor::editor->_object.expired())
				return;

			std::shared_ptr<Collider> collider;
			if (PrefabsEditor::editor->_collider_panel->_type->getText() == L"Rectangular") {
				collider = std::make_shared<RectangularCollider>(std::stoi(PrefabsEditor::editor->_collider_panel->_x->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_y->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_w->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_h->getText()));
			}
			else if (PrefabsEditor::editor->_collider_panel->_type->getText() == L"Circular") {
				collider = std::make_shared<CircularCollider>(std::stoi(PrefabsEditor::editor->_collider_panel->_x->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_y->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_w->getText()) / 2, std::stoi(PrefabsEditor::editor->_collider_panel->_h->getText()) / 2);
			}

			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(*PrefabsEditor::editor->_mesh);

			auto oldPrefab = PrefabsEditor::editor->_object.lock();

			if (!oldPrefab)
				return;

			std::shared_ptr<GameObject> prefab;

			if (_type->getText() == L"Monster") {
				prefab = std::make_shared<MonsterPrefab>(
					_name->getText(),
					PrefabsEditor::editor->_animator->getAnimations(),
					sf::Vector2i(std::stoi(PrefabsEditor::editor->_collider_panel->_x->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_y->getText())),
					4,
					collider,
					mesh
				);
			}
			else if (_type->getText() == L"Nature") {
				prefab = std::make_shared<NaturePrefab>(
					_name->getText(),
					PrefabsEditor::editor->_animator->getAnimations(),
					sf::Vector2i(std::stoi(PrefabsEditor::editor->_collider_panel->_x->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_y->getText())),
					collider,
					mesh
				);
			}
			else {
				return;
			}


			MapEditor::editor->_game_objects->replacePrefab(oldPrefab, prefab);
			if (MapEditor::editor->_cursor_on_map->_object.lock() == oldPrefab) MapEditor::editor->_cursor_on_map->_object = prefab;

			auto prefabs = prefabs_manager->getPrefabs(prefab->_type);
			int index = std::find(prefabs.begin(), prefabs.end(), oldPrefab) - prefabs.begin();
			
			prefabs_manager->replacePrefab(oldPrefab, prefab);

			MapEditor::editor->_palette->loadAll(MapEditor::editor->_palette->_categories->_selectedType);

			PrefabsEditor::editor->_object = prefab;
			PrefabsEditor::editor->_palette->loadAll(prefab->_type);
			PrefabsEditor::editor->_palette->_slots->selectSlot(index);
			PrefabsEditor::editor->_palette->_slots->_scrollbar->setValue(PrefabsEditor::editor->_palette->_slots->_scrollbar->_max_value);
			PrefabsEditor::editor->_palette->_slots->updateObjects();
			PrefabsEditor::MainPanel::setButtonsActivity();
			PrefabsEditor::MainPanel::setTooltips();

			};

		_add_prefab->_onclick_func = [this]() {

			std::shared_ptr<Collider> collider;
			if (PrefabsEditor::editor->_collider_panel->_type->getText() == L"Rectangular") {
				collider = std::make_shared<RectangularCollider>(std::stoi(PrefabsEditor::editor->_collider_panel->_x->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_y->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_w->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_h->getText()));
			}
			else if (PrefabsEditor::editor->_collider_panel->_type->getText() == L"Circular") {
				collider = std::make_shared<CircularCollider>(std::stoi(PrefabsEditor::editor->_collider_panel->_x->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_y->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_w->getText()) / 2, std::stoi(PrefabsEditor::editor->_collider_panel->_h->getText()) / 2);
			}

			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(*PrefabsEditor::editor->_mesh);

			std::shared_ptr<GameObject> prefab;

			if (_type->getText() == L"Monster") {
				prefab = std::make_shared<MonsterPrefab>(
					_name->getText(),
					PrefabsEditor::editor->_animator->getAnimations(),
					sf::Vector2i(std::stoi(PrefabsEditor::editor->_collider_panel->_x->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_y->getText())),
					4,
					collider,
					mesh
				);
			}
			else if (_type->getText() == L"Nature") {
				prefab = std::make_shared<NaturePrefab>(
					_name->getText(),
					PrefabsEditor::editor->_animator->getAnimations(),
					sf::Vector2i(std::stoi(PrefabsEditor::editor->_collider_panel->_x->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_y->getText())),
					collider,
					mesh
				);
			}
			else {
				return;
			}

			prefabs_manager->addPrefab(prefab);
			PrefabsEditor::editor->_object = prefab;
			PrefabsEditor::editor->_palette->loadAll(prefab->_type);
			PrefabsEditor::editor->_palette->_slots->selectLastSlot();
			PrefabsEditor::editor->_palette->_slots->_scrollbar->setValue(PrefabsEditor::editor->_palette->_slots->_scrollbar->_max_value);
			PrefabsEditor::editor->_palette->_slots->updateObjects();
			PrefabsEditor::MainPanel::setButtonsActivity();
			PrefabsEditor::MainPanel::setTooltips();


		};

		_remove_prefab->_onclick_func = [this]() {
				auto object = PrefabsEditor::editor->_object.lock();

				if (!object)
					return;

				int objectID = editor->_palette->_slots->_selectedSlotId;
				if (objectID >= 0) {
					// remove from map
					MapEditor::editor->_cursor_on_map->_object = std::weak_ptr<GameObject>();
					MapEditor::editor->_game_objects->removeGameObjectsByPrefab(object);

					// remove prefab
					prefabs_manager->removePrefab(object);

					// reload Prefabs Editor Palette
					PrefabsEditor::editor->_palette->loadAll(PrefabsEditor::editor->_palette->_categories->_selectedType);
					int newID = objectID;
					
					if (newID >= prefabs_manager->getPrefabs(PrefabsEditor::editor->_palette->_categories->_selectedType).size())
						newID = prefabs_manager->getPrefabs(PrefabsEditor::editor->_palette->_categories->_selectedType).size() - 1;

					PrefabsEditor::editor->_palette->_slots->selectSlot(newID);
					PrefabsEditor::editor->_palette->_slots->updateObjects();

					editor->_object = std::weak_ptr<GameObject>();
					editor->_animator = nullptr;
					editor->_main_panel->_name->setText(L"");
					editor->_main_panel->_type->setText(L"");
					editor->_collider = nullptr;
					editor->_collider_panel->_type->setText(L"");
					editor->_collider_panel->_x->setText(L"");
					editor->_collider_panel->_y->setText(L"");
					editor->_collider_panel->_w->setText(L"");
					editor->_collider_panel->_h->setText(L"");
					editor->_mesh = nullptr;


					if (newID >= 0) {
						// load all stuff
						std::shared_ptr<GameObject> newObject = std::dynamic_pointer_cast<GameObject>(PrefabsEditor::editor->_palette->_slots->_selectedSlot->_object.lock());

						editor->_object = newObject;
						editor->_animator = std::make_shared<Animator>(newObject->getAnimations(), 0.2f);

						editor->_main_panel->_name->setText(newObject->_name);
						editor->_main_panel->_type->setText(ObjectTypeToWString(newObject->_type));

						if (newObject->_collider->_type == ColliderType::Rectangular) {
							std::shared_ptr<RectangularCollider> collider = std::dynamic_pointer_cast<RectangularCollider>(newObject->_collider);
							editor->_collider = collider;
							editor->_collider_panel->_type->setText(L"Rectangular");
							editor->_collider_panel->_x->setText(std::to_wstring(collider->_rect.position.x));
							editor->_collider_panel->_y->setText(std::to_wstring(collider->_rect.position.y));
							editor->_collider_panel->_w->setText(std::to_wstring(collider->_rect.size.x));
							editor->_collider_panel->_h->setText(std::to_wstring(collider->_rect.size.y));
							editor->_mesh = std::make_shared<Mesh>(*newObject->getMesh());

						}
						else if (newObject->_collider->_type == ColliderType::Circular) {
							std::shared_ptr<CircularCollider> collider = std::dynamic_pointer_cast<CircularCollider>(newObject->_collider);
							editor->_collider = collider;
							editor->_collider_panel->_type->setText(L"Circular");
							editor->_collider_panel->_x->setText(std::to_wstring(collider->_x));
							editor->_collider_panel->_y->setText(std::to_wstring(collider->_y));
							editor->_collider_panel->_w->setText(std::to_wstring(collider->_radiusX * 2));
							editor->_collider_panel->_h->setText(std::to_wstring(collider->_radiusY * 2));
							editor->_mesh = std::make_shared<Mesh>(*newObject->getMesh());
						}
					}

					// reload Map Editor Palette
					MapEditor::editor->_palette->loadAll(ObjectType::Terrain);
					MapEditor::editor->_palette->_slots->_scrollbar->setValue(0);
					MapEditor::editor->_palette->_slots->updateObjects();

					PrefabsEditor::editor->_main_panel->setButtonsActivity();
					PrefabsEditor::editor->_main_panel->setTooltips();

					PrefabsEditor::editor->_animation_panel->loadStatsValues();
					PrefabsEditor::editor->_animation_panel->setButtonsActivity();
					PrefabsEditor::editor->_animation_panel->setTooltips();

				}
				
		};

		
	}

	MainPanel::~MainPanel() {
		
	}

	bool MainPanel::edited() {

		std::shared_ptr<GameObject> object = editor->_object.lock();

		if (!object)
			return false;

		if (WStringToObjectType(_type->getText()) == ObjectType::None)
			return false;

		if (!editor->_animator || editor->_animator->_animations.expired() || !editor->_animator->_animations.lock()->getTexture())
			return false;

		if (!PrefabsEditor::editor->_mesh)
			return false;

		if (PrefabsEditor::editor->_mesh->_shapes.empty())
			return false;

		if(!PrefabsEditor::editor->_mesh->everyShapeIsComplete())
			return false;

		// name
		if (object->_name != _name->getText())
			return true;

		// object type
		if (ObjectTypeToWString(object->_type) != _type->getText())
			return true;

		// animations
		if (object->_animations.lock() != editor->_animator->_animations.lock())
			return true;

		// collider
		std::shared_ptr<Collider> collider = object->getCollider();

		if (!collider)
			return false;

		if (collider->_type == ColliderType::Rectangular) {

			if (PrefabsEditor::editor->_collider_panel->_type->getText() != L"Rectangular")
				return true;

			std::shared_ptr<RectangularCollider> rectCollider = std::dynamic_pointer_cast<RectangularCollider>(collider);

			if (!rectCollider)
				return false;

			if (rectCollider->_rect.position.x != std::stoi(PrefabsEditor::editor->_collider_panel->_x->getText()))
				return true;

			if (rectCollider->_rect.position.y != std::stoi(PrefabsEditor::editor->_collider_panel->_y->getText()))
				return true;

			if (rectCollider->_rect.size.x != std::stoi(PrefabsEditor::editor->_collider_panel->_w->getText()))
				return true;

			if (rectCollider->_rect.size.y != std::stoi(PrefabsEditor::editor->_collider_panel->_h->getText()))
				return true;
		}
		else if (collider->_type == ColliderType::Circular) {

			if (PrefabsEditor::editor->_collider_panel->_type->getText() != L"Circular")
				return true;

			std::shared_ptr<CircularCollider> circCollider = std::dynamic_pointer_cast<CircularCollider>(collider);

			if (!circCollider)
				return false;

			if (circCollider->_x != std::stoi(PrefabsEditor::editor->_collider_panel->_x->getText()))
				return true;

			if (circCollider->_y != std::stoi(PrefabsEditor::editor->_collider_panel->_y->getText()))
				return true;

			if (circCollider->_radiusX * 2 != std::stoi(PrefabsEditor::editor->_collider_panel->_w->getText()))
				return true;

			if (circCollider->_radiusY * 2 != std::stoi(PrefabsEditor::editor->_collider_panel->_h->getText()))
				return true;
		}

		

		// mesh

		const auto editedMesh = PrefabsEditor::editor->_mesh;
		const auto objectMesh = object->getMesh();

		if (objectMesh == nullptr && editedMesh->everyShapeIsComplete())
			return true;

		if (editedMesh->_shapes.size() != objectMesh->_shapes.size())
			return true;

		for (std::size_t i = 0; i < editedMesh->_shapes.size(); ++i) {
			const auto& editedShape = editedMesh->_shapes[i];
			const auto& objectShape = objectMesh->_shapes[i];

			if (static_cast<bool>(editedShape) != static_cast<bool>(objectShape))
				return true;

			if (!editedShape)
				continue;

			if (editedShape->_points != objectShape->_points)
				return true;
		}

		return false;
	}

	void MainPanel::setButtonsActivity() {

		if (
			!editor ||
			!editor->_main_panel ||
			editor->_palette->_slots->_selectedSlotId < 0 ||
			WStringToObjectType(_type->getText()) == ObjectType::None ||
			!edited()
			) {
			_save_prefab->setActive(false);
		}
		else {
			_save_prefab->setActive(true);
		}

		if (
			!editor ||
			!editor->_main_panel ||
			WStringToObjectType(_type->getText()) == ObjectType::None ||
			(!editor->_animator || editor->_animator->_animations.expired() || !editor->_animator->_animations.lock()->getTexture()) ||
			!editor->_mesh || !editor->_mesh->everyShapeIsComplete()
			) {

			_add_prefab->setActive(false);
		}
		else {
			_add_prefab->setActive(true);
		}

		if(!editor || !editor->_palette || editor->_palette->_slots->_selectedSlotId < 0) {
			_remove_prefab->setActive(false);
		}
		else {
			_remove_prefab->setActive(true);
		}

	}

	void MainPanel::setTooltips() {
		if (_save_prefab->_isActive) {
			_save_prefab->setTooltip(256, L"Save prefab", L"Saves the current prefab");
		}
		else if (
			!editor->_object.expired() &&
			editor->_palette->_slots->_selectedSlotId >= 0 &&
			editor->_mesh && editor->_mesh->everyShapeIsComplete() &&
			!edited()
			) {
			_save_prefab->setTooltip(256, L"Cannot Save Prefab", L"There are no changes to save");
		}
		else {
			std::wstring tooltipTitle = L"Cannot Save Prefab";
			std::wstring tooltipDesc = L"Please check that:\n";

			if (!editor->_main_panel || editor->_object.expired() || editor->_palette->_slots->_selectedSlotId < 0)
				tooltipDesc += L"- A Prefab is selected from the Palette\n";

			if (!editor->_main_panel || editor->_main_panel->_name->getText().empty())
				tooltipDesc += L"-The Name is not empty\n";

			if (!editor->_main_panel || WStringToObjectType(editor->_main_panel->_type->getText()) == ObjectType::None)
				tooltipDesc += L"-The Type is valid\n";

			if (!editor->_animator || editor->_animator->_animations.expired() || !editor->_animator->_animations.lock()->getTexture())
				tooltipDesc += L"-The Animations is loaded\n";
			
			if(!editor->_mesh || !editor->_mesh->everyShapeIsComplete())
				tooltipDesc += L"-The Mesh is complete\n";

			tooltipDesc.pop_back(); // remove last newline character
			_save_prefab->setTooltip(256, tooltipTitle, tooltipDesc);
		}
		
		if (_add_prefab->_isActive) {
			_add_prefab->setTooltip(256, L"Add prefab", L"Adds the current object as a new prefab");
		}
		else {
			std::wstring tooltipTitle = L"Cannot Add Prefab";
			std::wstring tooltipDesc = L"Please check that:\n";

			if (!editor->_main_panel || editor->_main_panel->_name->getText().empty())
				tooltipDesc += L"-The Name is not empty\n";

			if (!editor->_main_panel || WStringToObjectType(editor->_main_panel->_type->getText()) == ObjectType::None)
				tooltipDesc += L"-The Type is valid\n";

			if (!editor->_animator || editor->_animator->_animations.expired() || !editor->_animator->_animations.lock()->getTexture())
				tooltipDesc += L"-The Animations is loaded\n";

			tooltipDesc.pop_back(); // remove last newline character
			_add_prefab->setTooltip(256, tooltipTitle, tooltipDesc);
		}

		if (_remove_prefab->_isActive) {
			_remove_prefab->setTooltip(256, L"Remove prefab", L"Removes the current prefab and all its instances from the map");
		}
		else {
			_remove_prefab->setTooltip(256, L"Cannot Remove Prefab", L"Please select an Prefab from the Palette");

		}

	}

	void MainPanel::cursorHover() {
		Components::Panel::cursorHover();

		_name->cursorHover();
		_type->cursorHover();

		_save_prefab->cursorHover();
		_add_prefab->cursorHover();
		_remove_prefab->cursorHover();

	}

	void MainPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);

		_name->handleEvent(event);
		_type->handleEvent(event);

		_save_prefab->handleEvent(event);
		_add_prefab->handleEvent(event);
		_remove_prefab->handleEvent(event);
	}

	void MainPanel::update() {
		Components::Panel::update();

		_name->update();
		_type->update();

		_save_prefab->update();
		_add_prefab->update();
		_remove_prefab->update();
	}

	void MainPanel::draw() {
		Components::Panel::draw();

		Main::render_window->draw(*_nameText);
		_name->draw();

		Main::render_window->draw(*_typeText);
		_type->draw();

		_save_prefab->draw();
		_add_prefab->draw();
		_remove_prefab->draw();


	}
}