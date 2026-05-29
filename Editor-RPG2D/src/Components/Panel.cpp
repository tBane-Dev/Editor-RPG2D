#include "Components/Panel.hpp"
#include "Cursor.hpp"

namespace Components {

	int Panel::_margin = 2;

	Panel::Panel(sf::Vector2i size, sf::Vector2i position) : Element() {

		_rect.size = sf::Vector2i(size.x - 2 * _margin, size.y - 2 * _margin);
		_rect.position = position + sf::Vector2i(_margin, _margin);
	}

	Panel::~Panel() {

	}


	sf::Vector2i Panel::getSize() {
		return _rect.size;
	}


	void Panel::setPosition(sf::Vector2i position) {
		_rect.position = position + sf::Vector2i(_margin, _margin);
	}

	sf::Vector2i Panel::getPosition() {
		return _rect.position - sf::Vector2i(_margin, _margin);
	}

	void Panel::cursorHover() {

		if (_rect.contains(cursor->_position)) {
			GUI_manager->Element_hovered = shared_from_this();
		}


	}

	void Panel::handleEvent(const sf::Event& event) {

		if (GUI_manager->Element_hovered.get() == this) {
			if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
				GUI_manager->Element_pressed = shared_from_this();
			}
		}

		if (GUI_manager->Element_pressed.get() == this) {
			if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
				GUI_manager->Element_pressed = nullptr;
			}
		}

	}

	void Panel::update() {

	}

	void Panel::draw() {

		GUI_manager->setView();

		int border = 2;
		sf::RectangleShape background(sf::Vector2f(_rect.size.x - 2 * border, _rect.size.y - 2 * border));
		background.setPosition(sf::Vector2f(_rect.position.x + border, _rect.position.y + border));
		background.setFillColor(sf::Color(31, 31, 31));
		background.setOutlineThickness(2);
		background.setOutlineColor(sf::Color(63, 63, 63));
		window->draw(background);
	}

}