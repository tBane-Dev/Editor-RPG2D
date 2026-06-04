#include "Controls/Scrollbar.hpp"
#include "RenderWindow.hpp"
#include "Cursor.hpp"
#include <iostream>
#include "TexturesManager.hpp"

Scrollbar::Scrollbar(int x, int y, int width, int height, int min_value, int max_value, int slider_size, int value) {
	
	if(width < 32)
		width = 32;

	if (height < 3 * 32)
		height = 3 * 32;

	_rect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(width, height));
	_slider = sf::IntRect(sf::Vector2i(0, width), sf::Vector2i(width, width));

	_min_value = min_value;
	_max_value = max_value;
	_slider_size = slider_size;
	_value = value;

	updateSliderSize();
	setPosition(sf::Vector2i(x,y));

	_state = ScrollbarState::Idle;
	_func = {};
	_dragOffset = sf::Vector2i(0, 0);

	_scrollArea = nullptr;
}


Scrollbar::~Scrollbar() {

}

sf::Vector2i Scrollbar:: getSize() { 
	return _rect.size; 
}

void Scrollbar::setMin(int min_value) {

	_min_value = min_value;
	updateSliderSize();
}

void Scrollbar::setMax(int max_value) {
	_max_value = max_value;
	if(_max_value < _min_value)
		_max_value = _min_value;
	updateSliderSize();
}

void Scrollbar::setValue(int value) {
	_value = std::clamp(value, _min_value, _max_value);

	updateSliderPosition();
}

void Scrollbar::setScrollArea(std::shared_ptr<sf::IntRect> rect, float scrollStep) {
	_scrollArea = rect;
	_scrollStep = scrollStep;
}

void Scrollbar::updateSliderSize() {
	int range = _max_value - _min_value;
	int trackHeight = _rect.size.y - 64;

	if (range <= 0) {
		_slider.size = sf::Vector2i(_rect.size.x, trackHeight);
		return;
	}

	int sizeY = int((float)trackHeight * ((float)_slider_size / ((float)range + (float)_slider_size)));

	if (sizeY < 32)
		sizeY = 32;

	_slider.size = sf::Vector2i(_rect.size.x, sizeY);

}void Scrollbar::updateSliderPosition() {
	int range = _max_value - _min_value;

	int trackTop = _rect.position.y + 32;
	int trackHeight = _rect.size.y - 64;

	if (range <= 0) {
		_slider.position = sf::Vector2i(_rect.position.x, trackTop);
		return;
	}

	int movableHeight = trackHeight - _slider.size.y;

	_slider.position.x = _rect.position.x;
	_slider.position.y = trackTop + int(
		((float)(_value - _min_value) / (float)range) * movableHeight
		);
}

void Scrollbar::setPosition(sf::Vector2i position) {
	_rect.position = position;
	updateSliderPosition();
}

int Scrollbar::getValue() {
	return _value;
}

void Scrollbar::cursorHover() {
	if (_rect.contains(Main::cursor->_position)) {
		GUI_manager->Element_hovered = this->shared_from_this();
	}
}

void Scrollbar::handleEvent(const sf::Event& event) {

	if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
		if (GUI_manager->Element_pressed.get() == this) {
			_state = ScrollbarState::Idle;
			GUI_manager->Element_pressed = nullptr;
		}
	}
	else if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
		if (_slider.contains(Main::cursor->_position)) {
			GUI_manager->Element_pressed = this->shared_from_this();
			if (_slider.size.y < _rect.size.y) {
				_state = ScrollbarState::Dragging;
				_dragOffset = Main::cursor->_position - _slider.position;
			}
		}
		
	}
	else if (const auto* mws = event.getIf<sf::Event::MouseWheelScrolled>(); mws) {
		if ((_rect.contains(Main::cursor->_position) || (_scrollArea != nullptr && _scrollArea->contains(Main::cursor->_position)))) {
			_state = ScrollbarState::Scrolled;
			_deltaScroll = -(int)mws->delta;
		}
	}
	

}

void Scrollbar::update() {
	if (_state == ScrollbarState::Dragging) {
		int trackTop = _rect.position.y + 32;
		int trackHeight = _rect.size.y - 64;
		int movableHeight = trackHeight - _slider.size.y;

		if (movableHeight <= 0)
			return;

		int newY = Main::cursor->_position.y - _dragOffset.y;

		newY = std::clamp(
			newY,
			trackTop,
			trackTop + movableHeight
		);

		float percent = float(newY - trackTop) / float(movableHeight);

		int newValue = _min_value + int(percent * float(_max_value - _min_value));

		if (newValue != _value) {
			_value = newValue;
			updateSliderPosition();

			if (_func)
				_func();
		}
	}

	if (_state == ScrollbarState::Scrolled) {
		setValue(_value + int((float)_deltaScroll*_scrollStep));
		
		if(_func)
			_func();
		_state = ScrollbarState::Idle;
	}
}

void Scrollbar::draw() {
	//sf::RectangleShape rect(sf::Vector2f(_rect.size));
	//rect.setPosition(sf::Vector2f(_rect.position));
	//rect.setFillColor(sf::Color(79, 79, 79));
	//window->draw(rect);

	//sf::RectangleShape slider(sf::Vector2f(_slider.size));
	//slider.setPosition(sf::Vector2f(_slider.position));
	//slider.setFillColor(sf::Color(47, 47, 47));
	//window->draw(slider);

	// textures 
	sf::Sprite upArrow(*textures_manager->getTexture(L"assets\\tex\\controls\\scrollbar\\up.png")->_texture);
	upArrow.setPosition(sf::Vector2f(_rect.position.x, _rect.position.y));
	Main::render_window->draw(upArrow);

	sf::Sprite downArrow(*textures_manager->getTexture(L"assets\\tex\\controls\\scrollbar\\down.png")->_texture);
	downArrow.setPosition(sf::Vector2f(_rect.position.x, _rect.position.y + _rect.size.y - 32));
	Main::render_window->draw(downArrow);

	sf::Sprite center(*textures_manager->getTexture(L"assets\\tex\\controls\\scrollbar\\center.png")->_texture);
	center.setPosition(sf::Vector2f(_rect.position.x, _rect.position.y + 32));
	center.setScale(sf::Vector2f(1, float(_rect.size.y - 64) / 32.f));
	Main::render_window->draw(center);

	sf::Sprite sliderTop(*textures_manager->getTexture(L"assets\\tex\\controls\\scrollbar\\slider_top.png")->_texture);
	sliderTop.setPosition(sf::Vector2f(_slider.position.x, _slider.position.y));
	Main::render_window->draw(sliderTop);

	sf::Sprite sliderCenter(*textures_manager->getTexture(L"assets\\tex\\controls\\scrollbar\\slider_center.png")->_texture);
	sliderCenter.setPosition(sf::Vector2f(_slider.position.x, _slider.position.y + 32));
	sliderCenter.setScale(sf::Vector2f(1, float(_slider.size.y - 64) / 32.f));
	Main::render_window->draw(sliderCenter);

	sf::Sprite sliderBottom(*textures_manager->getTexture(L"assets\\tex\\controls\\scrollbar\\slider_bottom.png")->_texture);
	sliderBottom.setPosition(sf::Vector2f(_slider.position.x, _slider.position.y + _slider.size.y - 32));
	Main::render_window->draw(sliderBottom);

}