#pragma once
#include "Components/Panel.hpp"
#include "Controls/TextInput.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"

namespace AnimationsEditor {
	class SpriteSheetPanel : public Components::Panel {
	public:
		
		std::unique_ptr<sf::Text> _title;

		std::shared_ptr<ButtonWithTextAndSprite> _loadBtn;

		// text labels
		std::unique_ptr<sf::Text> _xLabel;	
		std::unique_ptr<sf::Text> _yLabel;
		std::unique_ptr<sf::Text> _wLabel;
		std::unique_ptr<sf::Text> _hLabel;

		// text inputs
		std::shared_ptr<TextInput> _x;
		std::shared_ptr<TextInput> _y;
		std::shared_ptr<TextInput> _w;
		std::shared_ptr<TextInput> _h;

		SpriteSheetPanel(sf::Vector2i margin);
		~SpriteSheetPanel();

		void loadAnimations();
		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};
}
