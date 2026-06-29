#include "Controls/Button.hpp"
#include "SFML/Graphics.hpp"
#include "Theme.hpp"
#include "Time.hpp"
#include "Cursor.hpp"
#include "RenderWindow.hpp"
#include <iostream>
#include <memory>
#include "Tooltip.hpp"

Button::Button() : Element() {
	_isSelected = false;

	_rectIdleColor = sf::Color::Transparent;
	_rectHoverColor = sf::Color::Transparent;
	_rectPressColor = sf::Color::Transparent;

	_rectSelectIdleColor = sf::Color::Transparent;
	_rectSelectHoverColor = sf::Color::Transparent;
	_rectSelectPressColor = sf::Color::Transparent;

	_rectBorderWidth = 0;
	_rectIdleBorderColor = sf::Color::Transparent;
	_rectHoverBorderColor = sf::Color::Transparent;
	_rectPressBorderColor = sf::Color::Transparent;
	_rectSelectBorderColor = sf::Color::Transparent;

	_rectInactiveColor = sf::Color::Transparent;
	_rectInactiveBorderColor = sf::Color::Transparent;

	_isActive = true;
	_activatedByEnter = false;

	_state = ButtonState::Idle;
	_hover_func = {};
	_press_func = {};
	_onclick_func = {};
	_clickTime = sf::Time::Zero;

	_tooltipWidth = 256;
	_title = L"";
	_description = L"";
}



Button::~Button() {

}

void Button::setRectColors(sf::Color idleColor, sf::Color hoverColor, sf::Color pressColor, sf::Color selectIdleColor, sf::Color selectHoverColor, sf::Color selectPressColor, sf::Color inactiveColor) {
	_rectIdleColor = idleColor;
	_rectHoverColor = hoverColor;
	_rectPressColor = pressColor;

	_rectSelectIdleColor = selectIdleColor;
	_rectSelectHoverColor = selectHoverColor;
	_rectSelectPressColor = selectPressColor;

	_rectInactiveColor = inactiveColor;
}

void Button::setRectColors(sf::Color idleColor, sf::Color hoverColor, sf::Color pressColor, sf::Color selectIdleColor, sf::Color selectHoverColor, sf::Color selectPressColor, sf::Color inactiveColor,
	
	int borderWidth, sf::Color idleBorderColor, sf::Color hoverBorderColor, sf::Color pressBorderColor, sf::Color selectBorderColor, sf::Color inactiveBorderColor) {
	
	_rectIdleColor = idleColor;
	_rectHoverColor = hoverColor;
	_rectPressColor = pressColor;

	_rectSelectIdleColor = selectIdleColor;
	_rectSelectHoverColor = selectHoverColor;
	_rectSelectPressColor = selectPressColor;

	_rectInactiveColor = inactiveColor;

	_rectBorderWidth = borderWidth;

	_rectIdleBorderColor = idleBorderColor;
	_rectHoverBorderColor = hoverBorderColor;
	_rectPressBorderColor = pressBorderColor;
	_rectSelectBorderColor = selectBorderColor;
	_rectInactiveBorderColor = inactiveBorderColor;
}

void Button::setSize(sf::Vector2i size) {
	_rect.size = size;
}

sf::Vector2i Button::getSize() { 
	return _rect.size; 
}

sf::Vector2i Button::getPosition() {
	return _rect.position;
}

void Button::setPosition(sf::Vector2i position) {
	_rect.position = position;
}

void Button::setActive(bool active) {
	_isActive = active;
}

void Button::setSelect(bool select) {
	_isSelected = select;
}


void Button::unclick() {
	_state = ButtonState::Idle;
}

void Button::hover() {
	_state = ButtonState::Hover;
}

void Button::click() {
	_state = ButtonState::Pressed;
	_clickTime = currentTime;
}

void Button::activateByEnter(bool activate) {
	_activatedByEnter = activate;
}

void Button::setTooltip(int tooltipWidth, std::wstring title, std::wstring description) {
	_tooltipWidth = tooltipWidth;
	_title = title;
	_description = description;
}

void Button::cursorHover() {

	if (GUI_manager->Element_pressed != nullptr) {
		if(GUI_manager->Element_pressed.get() != this)
			return;
	}

	if (_rect.contains(Main::cursor->_position)) {
		Main::tooltip->setElement(this->shared_from_this(), _tooltipWidth);
	}

	if (!_isActive)
		return;

	if (_rect.contains(Main::cursor->_position)) {
		GUI_manager->Element_hovered = this->shared_from_this();
	}
}

void Button::handleEvent(const sf::Event& event) {


	if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
		if (GUI_manager->Element_pressed.get() == this) {

			if(GUI_manager->Element_hovered.get() == this)
				click();

			GUI_manager->Element_pressed = nullptr;
			return;
		}
	}
	
	if (GUI_manager->Element_hovered.get() == this) {
		if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
			GUI_manager->Element_pressed = this->shared_from_this();
			if(_press_func)
				_press_func();

			return;
		}
	}

	if (_isSelected && _activatedByEnter) {
		if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp && kp->code == sf::Keyboard::Key::Enter) {
			click();
			GUI_manager->Element_pressed = nullptr;
			return;
		}
	}
}

void Button::update() {
	if (_state == ButtonState::Pressed) {
		if ((currentTime - _clickTime).asSeconds() > 0.05f) {
			if (_onclick_func) {
				_onclick_func();
			}

			if (GUI_manager->Element_pressed.get() == this)
				GUI_manager->Element_pressed = nullptr;
			unclick();
		}
	}
	else if (GUI_manager->Element_hovered.get() == this) {
		hover();
	}
	else
		unclick();
}

void Button::draw() {
	sf::Vector2f rectSize;
	rectSize.x = float(_rect.size.x - 2 * _rectBorderWidth);
	rectSize.y = float(_rect.size.y - 2 * _rectBorderWidth);

	sf::RectangleShape rect(rectSize);
	if (_isActive) {
		if (_isSelected) {
			switch (_state) {
			case ButtonState::Pressed:
				rect.setFillColor(_rectSelectPressColor);
				rect.setOutlineThickness((float)_rectBorderWidth);
				rect.setOutlineColor(_rectSelectBorderColor);
				break;
			case ButtonState::Hover:
				rect.setFillColor(_rectSelectHoverColor);
				rect.setOutlineThickness((float)_rectBorderWidth);
				rect.setOutlineColor(_rectSelectBorderColor);
				break;
			case ButtonState::Idle:
				rect.setFillColor(_rectSelectIdleColor);
				rect.setOutlineThickness((float)_rectBorderWidth);
				rect.setOutlineColor(_rectSelectBorderColor);
				break;
			};
		}
		else {
			switch (_state) {
			case ButtonState::Pressed:
				rect.setFillColor(_rectPressColor);
				rect.setOutlineThickness((float)_rectBorderWidth);
				rect.setOutlineColor(_rectPressBorderColor);
				break;
			case ButtonState::Hover:
				rect.setFillColor(_rectHoverColor);
				rect.setOutlineThickness((float)_rectBorderWidth);
				rect.setOutlineColor(_rectHoverBorderColor);
				break;
			case ButtonState::Idle:
				rect.setFillColor(_rectIdleColor);
				rect.setOutlineThickness((float)_rectBorderWidth);
				rect.setOutlineColor(_rectIdleBorderColor);
				break;
			};
		}
	}
	else {
		rect.setFillColor(_rectInactiveColor);
		rect.setOutlineThickness((float)_rectBorderWidth);
		rect.setOutlineColor(_rectInactiveBorderColor);
	}

	sf::Vector2f rectPosition;
	rectPosition.x = float(_rect.position.x + _rectBorderWidth);
	rectPosition.y = float(_rect.position.y + _rectBorderWidth);
	rect.setPosition(rectPosition);
	Main::render_window->draw(rect);
}