#include "Components/MainMenu/MainMenu.hpp"
#include "Time.hpp"
#include "Cursor.hpp"
#include "Window.hpp"
#include "Theme.hpp"
#include "Components/MainMenu/MenuButton.hpp"
#include "DebugLog.hpp"


MainMenu::MainMenu() : Element() {
	_state = MainMenuStates::Closed;
	resize();
}

MainMenu::~MainMenu() {

}

void MainMenu::resize() {
	sf::Vector2i rectSize;
	rectSize.x = (int)(window->getSize().x);
	rectSize.y = menu_height;

	_rect.size = rectSize;
	_rect.position = sf::Vector2i(0, 0);
}

sf::Vector2i MainMenu::getSize() {
	return _rect.size;
}

void MainMenu::setPosition(sf::Vector2i position) {


	int x = 24;
	int y = position.y + menu_padding;
	for (int i = 0; i < _menu_boxes.size(); i++) {
		_menu_boxes[i]->setPosition(sf::Vector2i(position.x + x, y));
		x = x + (int)(_menu_boxes[i]->getSize().x);
	}
}

void MainMenu::hideMenu() {
	if (_open_menu_button != nullptr)
		_open_menu_button->_isSelected = false;

	_open_menu_button = nullptr;
	_state = MainMenuStates::Closing;
}

void MainMenu::closeMenu() {
	if (_open_menu_button != nullptr)
		_open_menu_button->_isSelected = false;

	_state = MainMenuStates::Closed;
	_open_menu_button = nullptr;
}


void MainMenu::openMenuButton(std::shared_ptr<MenuButton> menuButton) {
	_state = MainMenuStates::Opened;
	_open_menu_button = menuButton;
	_open_menu_button->_isSelected = true;
}

bool MainMenu::cursorOnAnyMenuButton() {
	for (auto& mb : _menu_boxes) {
		if (mb->_rect.contains(cursor->_position))
			return true;
		
		for(auto& op : mb->_options) {
			if (op->_rect.contains(cursor->_position))
				return true;
		}
	}
	return false;
}

void MainMenu::cursorHover() {

	if (_rect.contains(cursor->_position)) {
		GUI_manager->Element_hovered = this->shared_from_this();
	}

	for (auto& mb : _menu_boxes)
		mb->cursorHover();

}

void MainMenu::handleEvent(const sf::Event& event) {

	// TO-DO: add shortcuts for menu options
	//if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp) {
	//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
	//		if(kp->code == sf::Keyboard::Key::N) {
	//			//file_new->_onclick_func();
	//			closeMenu();
	//			return;
	//		}
	//		else if(kp->code == sf::Keyboard::Key::S) {
	//			//file_saveAs->_onclick_func();
	//			closeMenu();
	//			return;
	//		}
	//		else if(kp->code == sf::Keyboard::Key::O) {
	//			//file_load->_onclick_func();
	//			closeMenu();
	//			return;
	//		}
	//		else if(kp->code == sf::Keyboard::Key::E) {
	//			//file_export->_onclick_func();
	//			closeMenu();
	//			return;
	//		}
	//		else if(kp->code == sf::Keyboard::Key::I) {
	//			//file_import->_onclick_func();
	//			closeMenu();
	//			return;
	//		}
	//	}
	//}

	if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>();
		mbp && mbp->button == sf::Mouse::Button::Left) {

		if (_state == MainMenuStates::Opened) {
			if (!cursorOnAnyMenuButton()) {
				closeMenu();
				return;
			}
		}
	}

	for (auto& mb : _menu_boxes)
		mb->handleEvent(event);

	if (_state == MainMenuStates::Opened) {
		std::shared_ptr<MenuButton> hoverBox =
			std::dynamic_pointer_cast<MenuButton>(GUI_manager->Element_hovered);

		if (hoverBox != nullptr && _open_menu_button != hoverBox) {
			hideMenu();
			openMenuButton(hoverBox);
		}
	}
}

void MainMenu::update() {
	for (auto& mb : _menu_boxes)
		mb->update();
	
}

void MainMenu::draw() {

	if (_open_menu_button && _open_menu_button->_isSelected && !_open_menu_button->_options.empty()) {
		
		sf::Vector2f rectSize;
		rectSize.x = (float)(_open_menu_button->_options.front()->getSize().x);
		rectSize.y = (float)(_open_menu_button->_options.size()) * (float)(optionbox_height);

		sf::RectangleShape rect(rectSize);
		rect.setPosition((sf::Vector2f)(_open_menu_button->_options.front()->getPosition()));
		rect.setOutlineThickness((float)(optionbox_border_width));
		rect.setOutlineColor(optionbox_border_color);
		window->draw(rect);

		for (auto& option : _open_menu_button->_options)
			option->draw();
	}

	sf::RectangleShape rect(sf::Vector2f(_rect.size));
	rect.setFillColor(menu_bar_color);
	rect.setPosition(sf::Vector2f(_rect.position));
	window->draw(rect);

	for (auto& mb : _menu_boxes)
		mb->draw();
}
