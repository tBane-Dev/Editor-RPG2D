#include "Editors/PrefabsEditor/MainPanel.hpp"
#include "Editors/PrefabsEditor/Editor.hpp"
#include "Theme.hpp"
#include "Window.hpp"
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

		sf::Vector2i startPosition;
		startPosition.x = getPosition().x + (getSize().x - 380) / 2;
		startPosition.y = _margin + getPosition().y + 512 + 16;
		int distance_between_buttons = 16;

		_add_prefab = std::make_shared<ButtonWithTextAndSprite>(
			L"Add prefab",
			textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\largeButton_press.png"),
			sf::Vector2i(startPosition.x, startPosition.y)
		);

		_add_prefab->_onclick_func = [this]() {

			if (!PrefabsEditor::editor->_object)
				return;

			std::shared_ptr<Collider> collider;
			if (PrefabsEditor::editor->_collider_panel->_type->getText() == L"Rectangular") {
				collider = std::make_shared<RectangularCollider>(std::stoi(PrefabsEditor::editor->_collider_panel->_x->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_y->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_w->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_h->getText()));
			}
			else if (PrefabsEditor::editor->_collider_panel->_type->getText() == L"Circular") {
				collider = std::make_shared<CircularCollider>(std::stoi(PrefabsEditor::editor->_collider_panel->_x->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_y->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_w->getText()) / 2, std::stoi(PrefabsEditor::editor->_collider_panel->_h->getText()) / 2);
			}

			std::shared_ptr<GameObject> prefab = std::make_shared<NaturePrefab>(
				_name->getText(),
				PrefabsEditor::editor->_animator->getAnimations(),
				sf::Vector2i(std::stoi(PrefabsEditor::editor->_collider_panel->_x->getText()), std::stoi(PrefabsEditor::editor->_collider_panel->_y->getText())),
				collider
			);

			prefabs_manager->addPrefab(prefab);
			PrefabsEditor::editor->_object = prefab;
			PrefabsEditor::editor->_palette->loadAll(prefab->_type);
			PrefabsEditor::editor->_palette->_slots->selectLastSlot();
			PrefabsEditor::editor->_palette->_slots->_scrollbar->setValue(PrefabsEditor::editor->_palette->_slots->_scrollbar->_max_value);
			PrefabsEditor::editor->_palette->_slots->updateObjects();

			};

		_duplicate_prefab = std::make_shared<ButtonWithTextAndSprite>(
			L"Duplicate prefab",
			textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\largeButton_press.png"),
			sf::Vector2i(startPosition.x, startPosition.y + _add_prefab->getSize().y + distance_between_buttons)
		);

		_remove_prefab = std::make_shared<ButtonWithTextAndSprite>(
			L"Remove prefab",
			textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\largeButton_press.png"),
			sf::Vector2i(startPosition.x, _duplicate_prefab->getPosition().y + _duplicate_prefab->getSize().y + distance_between_buttons)
		);

		_remove_prefab->_onclick_func = [this]() {

			if (PrefabsEditor::editor->_object == nullptr) {
				return;
			}

			int selectedSlotId = PrefabsEditor::editor->_palette->_slots->_selectedSlotId;
			int scrollbarValue = PrefabsEditor::editor->_palette->_slots->_scrollbar->getValue();

			prefabs_manager->removePrefab(PrefabsEditor::editor->_object);
			PrefabsEditor::editor->_object = nullptr;
			PrefabsEditor::editor->_palette->loadAll(PrefabsEditor::editor->_palette->_categories->_selectedType);
			PrefabsEditor::editor->_palette->_slots->_scrollbar->setValue(scrollbarValue);

			selectedSlotId -= 1;

			PrefabsEditor::editor->_palette->_slots->selectSlot(selectedSlotId);

			if (PrefabsEditor::editor->_palette->_slots->_selectedSlotId >= 0) {
				PrefabsEditor::editor->_object = std::dynamic_pointer_cast<GameObject>(PrefabsEditor::editor->_palette->_slots->_selectedSlot->_object);
			}

			PrefabsEditor::editor->_palette->_slots->updateObjects();
			PrefabsEditor::editor->_animator = nullptr;

			if (PrefabsEditor::editor->_object)
				PrefabsEditor::editor->_animator = std::make_shared<Animator>(PrefabsEditor::editor->_object->_animations, 0.2f);
			};
	}

	MainPanel::~MainPanel() {

	}

	void MainPanel::cursorHover() {
		Components::Panel::cursorHover();

		_name->cursorHover();
		_type->cursorHover();

		_add_prefab->cursorHover();
		_duplicate_prefab->cursorHover();
		_remove_prefab->cursorHover();

	}

	void MainPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);

		_name->handleEvent(event);
		_type->handleEvent(event);

		_add_prefab->handleEvent(event);
		_duplicate_prefab->handleEvent(event);
		_remove_prefab->handleEvent(event);
	}

	void MainPanel::update() {
		Components::Panel::update();

		_name->update();
		_type->update();

		_add_prefab->update();
		_duplicate_prefab->update();
		_remove_prefab->update();
	}

	void MainPanel::draw() {
		Components::Panel::draw();

		window->draw(*_nameText);
		_name->draw();

		window->draw(*_typeText);
		_type->draw();

		_add_prefab->draw();
		_duplicate_prefab->draw();
		_remove_prefab->draw();


	}
}