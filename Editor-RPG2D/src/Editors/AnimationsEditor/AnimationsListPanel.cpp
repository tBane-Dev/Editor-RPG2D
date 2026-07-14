#include "Editors/AnimationsEditor/AnimationsListPanel.hpp"
#include "Theme.hpp"
#include "AnimationsManager.hpp"
#include "Editors/AnimationsEditor/Editor.hpp"
#include <iostream>

namespace AnimationsEditor {

	AnimationsListPanel::AnimationsListPanel(sf::Vector2i margin) : Components::ListPanel(margin, sf::Vector2i(420, 600), sf::Vector2i(margin.x, AnimationsEditor::editor->_main_menu->getSize().y + margin.y)) {
		_title->setString(L"Animations List");
		loadAll();
	}

	AnimationsListPanel::~AnimationsListPanel() {

	}

	void AnimationsListPanel::loadList() {

		resetItems();

		int max_id = (animations_manager->getAnimationsCount() < _items.size()) ? animations_manager->getAnimationsCount() : _items.size();
		
		for (int i = 0; i < max_id; i++) {
			std::shared_ptr<Components::ListPanelItem> item = _items[i];
		
			int index = i;
			if (_scrollbar) index += _scrollbar->getValue() / basic_text_rect_height;
		
			if (index < animations_manager->getAnimationsCount()) {
				item->setName(animations_manager->getAnimations(index).lock()->_path);
				item->_onclick_func = [this, index, item]() {
		
					selectItem(index);
		
					std::shared_ptr<Animations> animations = animations_manager->getAnimations(index).lock();
		
					if (!animations)
						return;
		
					editor->_animations = animations;
					editor->_tempAnimations = std::make_shared<Animations>(*editor->_animations);
					editor->_animator = std::make_shared<Animator>(editor->_tempAnimations, 0.2f);
		
					editor->_name_panel->loadAnimations();
					editor->_sprite_sheet_panel->loadAnimations();
					editor->_sprite_sheet_panel->setTextInputsRange();
		
					editor->_actions_panel->setButtonsActivity();
					editor->_actions_panel->setTooltips();
		
					editor->_preview_panel->loadAnimations();
					editor->_preview_panel->setButtonsActivity();
					editor->_preview_panel->setTooltips();
					
					};
			}
			else {
				item->setName(L"");
				item->_onclick_func = {};
			}
		
		
		
		}
	}
}
