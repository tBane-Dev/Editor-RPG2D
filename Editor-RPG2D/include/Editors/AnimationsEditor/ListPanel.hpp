#pragma once
#include "Components/Panel.hpp"
#include "Controls/TextInput.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"

namespace AnimationsEditor {
	class ListPanel : public Components::Panel {
	public:

		std::unique_ptr<sf::Text> _title;

		ListPanel(sf::Vector2i margin);
		~ListPanel();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};
}
