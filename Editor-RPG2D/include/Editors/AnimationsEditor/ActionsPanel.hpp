#pragma once
#include "Components/Panel.hpp"
#include "Controls/TextInput.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"
#include "Controls/Scrollbar.hpp"

namespace AnimationsEditor {


	class ActionsPanel : public Components::Panel {
	public:

		std::unique_ptr<sf::Text> _title;

		std::shared_ptr<ButtonWithTextAndSprite> _saveBtn;
		std::shared_ptr<ButtonWithTextAndSprite> _addBtn;
		std::shared_ptr<ButtonWithTextAndSprite> _removeBtn;

		ActionsPanel(sf::Vector2i margin);
		~ActionsPanel();

		void setButtonsActivity();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};
}
