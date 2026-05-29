#pragma once
#include "Components/Panel.hpp"
#include "Controls/TextInput.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"

namespace AnimationsEditor {
	class SpriteSheetPanel : public Components::Panel {
	public:

		std::unique_ptr<sf::Text> _title;

		SpriteSheetPanel(sf::Vector2i margin);
		~SpriteSheetPanel();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};
}
