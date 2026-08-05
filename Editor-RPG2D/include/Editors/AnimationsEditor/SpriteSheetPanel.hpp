#pragma once
#include "Components/Panel.hpp"
#include "Controls/NumberInput.hpp"
#include "Controls/RealInput.hpp"
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
		std::unique_ptr<sf::Text> _aLabel;
		std::unique_ptr<sf::Text> _fLabel;
		std::unique_ptr<sf::Text> _iLabel;

		// text inputs
		std::shared_ptr<NumberInput> _x;	// position x
		std::shared_ptr<NumberInput> _y;	// position y
		std::shared_ptr<NumberInput> _w;	// width
		std::shared_ptr<NumberInput> _h;	// height
		std::shared_ptr<NumberInput> _a;	// count of animations
		std::shared_ptr<NumberInput> _f;	// count of frames
		std::shared_ptr<RealInput> _i;	// interval between frames

		SpriteSheetPanel(sf::Vector2i margin);
		~SpriteSheetPanel();

		void loadAnimations();
		void setTextInputsRange();
		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};
}
