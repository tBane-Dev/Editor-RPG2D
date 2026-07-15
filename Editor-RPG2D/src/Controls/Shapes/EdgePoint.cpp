#include "Controls/Shapes/EdgePoint.hpp"
#include "Cursor.hpp"

EdgePoint::EdgePoint(EdgePointType type, sf::Vector2i position) : Element() {
	_type = type;
	_rect = sf::IntRect(position, sf::Vector2i(8, 8));
}

EdgePoint::~EdgePoint() {

}

void EdgePoint::setPosition(sf::Vector2i position){
	_rect.position = position;
}

sf::Vector2i EdgePoint::getPosition() {
	return _rect.position;
}

sf::Vector2i EdgePoint::getSize() {
	return _rect.size;
}
void EdgePoint::cursorHover() {

	sf::IntRect r(_rect.position - _rect.size / 2, _rect.size);
	if (r.contains(Main::cursor->_position)) {
		GUI_manager->Element_hovered = this->shared_from_this();
	}
}

void EdgePoint::handleEvent(const sf::Event& event) {
	if (GUI_manager->Element_pressed.get() == this) {
		if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
			if (GUI_manager->Element_pressed.get() == this)
				GUI_manager->Element_pressed = nullptr;
			return;
		}
	}

	sf::IntRect r(_rect.position - _rect.size / 2, _rect.size);
	if (r.contains(Main::cursor->_position)) {

		if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
			GUI_manager->Element_pressed = this->shared_from_this();
		}
	}
}

void EdgePoint::update() {
	
}

void EdgePoint::draw() {
	sf::RectangleShape rect(sf::Vector2f(_rect.size));
	rect.setOrigin(sf::Vector2f(_rect.size/2));
	rect.setFillColor((GUI_manager->Element_hovered.get() == this) ? sf::Color(0, 191, 0) : sf::Color(191, 0, 0));
	rect.setPosition(sf::Vector2f(_rect.position));
	Main::render_window->draw(rect);
}
