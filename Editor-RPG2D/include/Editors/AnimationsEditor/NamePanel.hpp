#pragma once
#include "Components/Panel.hpp"
#include "Controls/TextInput.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"

namespace AnimationsEditor {
	class NamePanel : public Components::Panel {
	public:

		std::unique_ptr<sf::Text> _nameText;
		std::shared_ptr<TextInput> _name;

		NamePanel(sf::Vector2i margin);
		~NamePanel();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};
}
