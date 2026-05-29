#pragma once
#include "Components/Panel.hpp"
#include "Controls/TextInput.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"

namespace AnimationsEditor {
	class PreviewPanel : public Components::Panel {
	public:

		std::unique_ptr<sf::Text> _title;

		PreviewPanel(sf::Vector2i margin);
		~PreviewPanel();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};
}
