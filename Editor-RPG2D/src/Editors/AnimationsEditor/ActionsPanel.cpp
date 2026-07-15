#include "Editors/AnimationsEditor/ActionsPanel.hpp"
#include "Editors/AnimationsEditor/Editor.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
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
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_inactive.png"),
			sf::Vector2i(startPosition.x+16, startPosition.y)
		);

		_addBtn = std::make_shared<ButtonWithTextAndSprite>(
			L"Add Animations",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_inactive.png"),
			sf::Vector2i(startPosition.x +16, _saveBtn->getPosition().y + _saveBtn->getSize().y + marginBetweenButtons)
		);

		_removeBtn = std::make_shared<ButtonWithTextAndSprite>(
			L"Remove Animations",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_inactive.png"), 
			sf::Vector2i(startPosition.x + 16, _addBtn->getPosition().y + _addBtn->getSize().y + marginBetweenButtons)
		);

		_saveBtn->_onclick_func = []() {
			if (!editor->_animations)
				return;

			if (!editor->_tempAnimations)
				return;

			if (!editor->_tempAnimations->getTexture())
				return;

			if(editor->_name_panel->_name->getText().empty())
				return;

			if (
				editor->_sprite_sheet_panel->_w->getNumber() == 0 || 
				editor->_sprite_sheet_panel->_h->getNumber() == 0 ||
				editor->_sprite_sheet_panel->_a->getNumber() == 0 ||
				editor->_sprite_sheet_panel->_f->getNumber() == 0 )
				return;

			editor->_animations->_path = editor->_tempAnimations->_path;
			editor->_animations->_frameSize = editor->_tempAnimations->_frameSize;
			editor->_animations->_animationsCount = editor->_tempAnimations->_animationsCount;
			editor->_animations->_framesCount = editor->_tempAnimations->_framesCount;
			editor->_animations->_offsetX = editor->_tempAnimations->_offsetX;
			editor->_animations->_offsetY = editor->_tempAnimations->_offsetY;
			editor->_animations->_texture = editor->_tempAnimations->_texture;

			editor->_list_panel->loadAll();
			};

		_addBtn->_onclick_func = []() {

			if (!editor->_animations)
				return;

			if(!editor->_tempAnimations)
				return;

			if (!editor->_tempAnimations->getTexture())
				return;

			if (
				editor->_sprite_sheet_panel->_w->getNumber() == 0 ||
				editor->_sprite_sheet_panel->_h->getNumber() == 0 ||
				editor->_sprite_sheet_panel->_a->getNumber() == 0 ||
				editor->_sprite_sheet_panel->_f->getNumber() == 0)
				return;

			int animations = 1;
			int frames = 1;

			int newID = animations_manager->addAnimations(std::make_shared<Animations>(*editor->_tempAnimations));

			editor->_animations = animations_manager->getAnimations(newID).lock();
			editor->_tempAnimations = std::make_shared<Animations>(*editor->_animations);
			editor->_animator->_animations = editor->_tempAnimations;

			editor->_list_panel->loadAll();

			if (newID >= std::floor((float)editor->_list_panel->_scrollbar->getValue() / (float)basic_text_rect_height) + editor->_list_panel->_items.size() - 2) {
				editor->_list_panel->_scrollbar->setValue(editor->_list_panel->_scrollbar->_max_value);
				editor->_list_panel->loadAll();
			}

			editor->_list_panel->selectItem(newID);
			editor->_actions_panel->setButtonsActivity();
			editor->_actions_panel->setTooltips();
		};

		_removeBtn->_onclick_func = []() {
			
			if (!editor->_animations)
				return;

			int animationID = editor->_list_panel->_selectedItemIndex;
			if (animationID >= 0) {

				animations_manager->removeAnimations(animationID);

				editor->_list_panel->loadAll();
				int newID = editor->_list_panel->_selectedItemIndex;
				if (newID >= animations_manager->getAnimationsCount()) 
					newID = animations_manager->getAnimationsCount() - 1;

				editor->_list_panel->selectItem(newID);

				editor->_animations = animations_manager->getAnimations(editor->_list_panel->_selectedItemIndex).lock();
				if (editor->_animations) {
					editor->_tempAnimations = std::make_shared<Animations>(*editor->_animations);
					editor->_animator = std::make_shared<Animator>(editor->_tempAnimations, 0.2f);
				}
				else {
					editor->_tempAnimations = nullptr;
					editor->_animator = nullptr;
				}
			}
			else {
				editor->_animations = nullptr;
				editor->_tempAnimations = nullptr;
				editor->_animator = nullptr;
			}

			editor->_name_panel->loadAnimations();;
			editor->_sprite_sheet_panel->loadAnimations();
			editor->_actions_panel->setButtonsActivity();
			editor->_actions_panel->setTooltips();
			editor->_preview_panel->loadAnimations();
			editor->_preview_panel->setButtonsActivity();
		};

		setButtonsActivity();
		setTooltips();
	}

	ActionsPanel::~ActionsPanel() {

	}

	void ActionsPanel::setButtonsActivity() {

		if (editor->_tempAnimations &&
			editor->_list_panel->_selectedItemIndex >= 0 &&
			AnimationsEditor::editor->_name_panel->_name->getText().length() > 0 && 
			AnimationsEditor::editor->_sprite_sheet_panel->_w->getNumber() > 0 &&
			AnimationsEditor::editor->_sprite_sheet_panel->_h->getNumber() > 0 &&
			AnimationsEditor::editor->_sprite_sheet_panel->_a->getNumber() > 0 &&
			AnimationsEditor::editor->_sprite_sheet_panel->_f->getNumber() > 0 
			) {
			_saveBtn->setActive(true);
		}
		else {
			_saveBtn->setActive(false);
		}

		if (editor->_tempAnimations &&
			AnimationsEditor::editor->_name_panel->_name->getText().length() > 0 &&
			AnimationsEditor::editor->_sprite_sheet_panel->_w->getNumber() > 0 &&
			AnimationsEditor::editor->_sprite_sheet_panel->_h->getNumber() > 0 &&
			AnimationsEditor::editor->_sprite_sheet_panel->_a->getNumber() > 0 &&
			AnimationsEditor::editor->_sprite_sheet_panel->_f->getNumber() > 0 
			) {
			_addBtn->setActive(true);
		}
		else {
			_addBtn->setActive(false);
		}

		if(editor->_tempAnimations){
			_removeBtn->setActive(true);
		}
		else {
			_removeBtn->setActive(false);
		}

	}

	void ActionsPanel::setTooltips() {
		if (_saveBtn->_isActive) {
			_saveBtn->setTooltip(256, L"Save Animations", L"Saves the current animations to the selected animations");
		}
		else {
			std::wstring tooltipTitle = L"Cannot Save Animations";
			std::wstring tooltipDesc = L"Please check that:\n";
			
			if (editor->_name_panel->_name->getText().empty())
				tooltipDesc += L"-The Name is not empty\n";

			if(!editor->_tempAnimations || !editor->_tempAnimations->getTexture())
				tooltipDesc += L"-The Sprite Sheet is loaded\n";

			if(editor->_list_panel->_selectedItemIndex < 0)
				tooltipDesc += L"-An Animations is selected in the list\n";

			tooltipDesc.pop_back(); // remove last newline character
			_saveBtn->setTooltip(256, tooltipTitle, tooltipDesc);
		}

		if (_addBtn->_isActive) {
			_addBtn->setTooltip(256, L"Add Animations", L"Adds a new animations to the list");
		}
		else {
			std::wstring tooltipTitle = L"Cannot add Animations";
			std::wstring tooltipDesc = L"Please check that:\n";

			if (editor->_name_panel->_name->getText().empty()) {
				tooltipDesc += L"-The Name is not empty\n";
			}

			if (!editor->_tempAnimations || !editor->_tempAnimations->getTexture()) {
				tooltipDesc += L"-The Sprite Sheet is loaded\n";
			}

			tooltipDesc.pop_back(); // remove last newline character
			_addBtn->setTooltip(256, tooltipTitle, tooltipDesc);
		}


		if(_removeBtn->_isActive)
			_removeBtn->setTooltip(256, L"Remove Animations", L"Removes the selected animations from the list");
		else
			_removeBtn->setTooltip(256, L"Cannot Remove Animations", L"Please select an animation from the list");

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

		Main::render_window->draw(*_title);

		_saveBtn->draw();
		_addBtn->draw();
		_removeBtn->draw();
	}
}