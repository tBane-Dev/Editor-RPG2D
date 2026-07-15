#include "Editors/BuildingsEditor/ActionsPanel.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/BuildingsListPanel.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"

namespace BuildingsEditor {
	
	ActionsPanel::ActionsPanel(sf::Vector2i margin) : Panel(
		sf::Vector2i(420, 300), 
		sf::Vector2i(BuildingsEditor::editor->_list_panel->getPosition().x, BuildingsEditor::editor->_list_panel->getPosition().y + BuildingsEditor::editor->_list_panel->getSize().y + 4)) {

		// title
		_title = std::make_unique<sf::Text>(basicFont, L"Actions", 20);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));
		
		sf::Vector2i startPosition;
		startPosition.x = getPosition().x;
		startPosition.y = getPosition().y + margin.y;

		int marginBetweenButtons = 8;

		_saveBtn = std::make_shared<ButtonWithTextAndSprite>(
			L"Save Building",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_inactive.png"),
			sf::Vector2i(startPosition.x+16, startPosition.y)
		);

		_addBtn = std::make_shared<ButtonWithTextAndSprite>(
			L"Add Building",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_inactive.png"),
			sf::Vector2i(startPosition.x +16, _saveBtn->getPosition().y + _saveBtn->getSize().y + marginBetweenButtons)
		);

		_removeBtn = std::make_shared<ButtonWithTextAndSprite>(
			L"Remove Building",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_inactive.png"), 
			sf::Vector2i(startPosition.x + 16, _addBtn->getPosition().y + _addBtn->getSize().y + marginBetweenButtons)
		);

		_saveBtn->_onclick_func = []() {
			};

		_addBtn->_onclick_func = []() {

		};

		_removeBtn->_onclick_func = []() {
			
		};

		setButtonsActivity();
		setTooltips();
	}

	ActionsPanel::~ActionsPanel() {

	}

	void ActionsPanel::setButtonsActivity() {

	}

	void ActionsPanel::setTooltips() {
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