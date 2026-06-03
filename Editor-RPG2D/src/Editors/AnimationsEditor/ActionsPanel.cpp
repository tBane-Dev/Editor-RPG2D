#include "Editors/AnimationsEditor/ListPanel.hpp"
#include "Editors/AnimationsEditor/Editor.hpp"
#include "Theme.hpp"
#include "Window.hpp"
#include "DebugLog.hpp"
#include "Objects/Monster.hpp"
#include "PrefabsManager.hpp"
#include "Editors/MapEditor/Editor.hpp"

namespace AnimationsEditor {
	
	ActionsPanel::ActionsPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 300), sf::Vector2i(margin.x, AnimationsEditor::editor->_main_menu->getSize().y + 600 + margin.y)) {

		// title
		_title = std::make_unique<sf::Text>(basicFont, L"Actions", 20);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));
		
		sf::Vector2i startPosition;
		startPosition.x = getPosition().x;
		startPosition.y = getPosition().y + margin.y;

		int marginBetweenButtons = 8;

		_saveBtn = std::make_shared<ButtonWithTextAndSprite>(
			L"Save Animations",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			sf::Vector2i(startPosition.x+16, startPosition.y)
		);

		_addBtn = std::make_shared<ButtonWithTextAndSprite>(
			L"Add Animations",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			sf::Vector2i(startPosition.x +16, _saveBtn->getPosition().y + _saveBtn->getSize().y + marginBetweenButtons)
		);

		_removeBtn = std::make_shared<ButtonWithTextAndSprite>(
			L"Remove Animations",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			sf::Vector2i(startPosition.x + 16, _addBtn->getPosition().y + _addBtn->getSize().y + marginBetweenButtons)
		);

		_addBtn->_onclick_func = []() {

			if (!editor->_animations)
				return;

			if (!editor->_animations->getTexture())
				return;

			if (editor->_sprite_sheet_panel->_w->getText().empty() || editor->_sprite_sheet_panel->_h->getText().empty())
				return;

			int newID = animations_manager->addAnimations(
				editor->_animation_name_panel->_name->getText(),
				editor->_animations->getTexture(),
				editor->_animations->_animationsCount,
				editor->_animations->_framesCount
			);

			editor->_list_panel->loadAll();

			if (newID >= std::floor((float)editor->_list_panel->_scrollbar->getValue() / (float)basic_text_rect_height) + editor->_list_panel->_items.size() - 2) {
				editor->_list_panel->_scrollbar->setValue(editor->_list_panel->_scrollbar->_max_value);
				editor->_list_panel->loadAll();
			}

			editor->_list_panel->selectItem(newID);
		};

		_removeBtn->_onclick_func = []() {
			
			if (!editor->_animations)
				return;

			int animationID = editor->_list_panel->_selectedItemIndex;
			if (animationID >= 0) {

				//MapEditor::editor->_game_objects->removePrefabsByAnimations(animationID);
				//prefabs_manager->removePrefabsByAnimations(animationID);
				animations_manager->removeAnimations(animationID);

				editor->_list_panel->loadAll();
				int newID = editor->_list_panel->_selectedItemIndex;
				if (newID >= animations_manager->getAnimationsCount()) newID = animations_manager->getAnimationsCount() - 1;
				editor->_list_panel->selectItem(newID);

				editor->_animations = animations_manager->getAnimations(editor->_list_panel->_selectedItemIndex);
				editor->_animator = std::make_shared<Animator>(editor->_animations, 0.2f);
				editor->_animation_name_panel->loadAnimations();;
				editor->_sprite_sheet_panel->loadAnimations();
				editor->_preview_panel->loadAnimations();
			}
		};
	}

	ActionsPanel::~ActionsPanel() {

	}

	void ActionsPanel::cursorHover() {
		Components::Panel::cursorHover();
	
		_saveBtn->cursorHover();
		_addBtn->cursorHover();
		_removeBtn->cursorHover();	
	}

	void ActionsPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);

		_saveBtn->handleEvent(event);
		_addBtn->handleEvent(event);
		_removeBtn->handleEvent(event);
	}

	void ActionsPanel::update() {
		Components::Panel::update();
		
		_saveBtn->update();
		_addBtn->update();
		_removeBtn->update();
	}

	void ActionsPanel::draw() {
		Components::Panel::draw();

		window->draw(*_title);

		_saveBtn->draw();
		_addBtn->draw();
		_removeBtn->draw();
	}
}