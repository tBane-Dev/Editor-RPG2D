#pragma once
#include "Components/Palette/Tools.hpp"
#include "Theme.hpp"
#include "DebugLog.hpp"

Tools::Tools() {
	_main_margin = 16;
	_top_margin = 22;
	_outer_margin = 16;

	_text = std::make_unique<sf::Text>(basicFont, L"Tools", 20);
	_text->setFillColor(basic_text_color);
}

Tools::~Tools() {

}

sf::Vector2i Tools::getPosition() {
	return _rect.position;
}

sf::Vector2i Tools::getSize() {
	return _rect.size;
}

void Tools::setPosition(sf::Vector2i position) {

	_rect.position = position + sf::Vector2i(_main_margin, _main_margin);
	

	int x = _rect.position.x + _outer_margin;
	int y = _rect.position.y + _outer_margin;

	if(_text)
		_text->setPosition(sf::Vector2f(x, y));

}

void Tools::cursorHover() {

}

void Tools::handleEvent(const sf::Event& event) {

}

void Tools::update() {

}

void Tools::draw() {

}