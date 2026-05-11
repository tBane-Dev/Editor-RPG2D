#include "GUIManager.hpp"
#include "SFML/Graphics.hpp"
#include "Window.hpp"

Element::Element() {

}

Element::~Element() {

}

void Element::cursorHover() {

}

void Element::handleEvent(const sf::Event& event) {

}

void Element::update() {

}

void Element::draw() {

}

GUIManager::GUIManager() {
	_view = sf::View(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(window->getSize())));

	Element_hovered = nullptr;
	Element_pressed = nullptr;
}

GUIManager::~GUIManager() {
}

void GUIManager::setView() {
	window->setView(_view);
}

std::shared_ptr<GUIManager> GUI_manager = nullptr;