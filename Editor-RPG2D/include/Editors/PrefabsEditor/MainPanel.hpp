#pragma once
#include "Components/Panel.hpp"
#include "Controls/TextInput.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"

namespace PrefabsEditor {
	class MainPanel : public Components::Panel {
	public:

		std::unique_ptr<sf::Text> _nameText;
		std::shared_ptr<TextInput> _name;

		std::unique_ptr<sf::Text> _typeText;
		std::shared_ptr<TextInput> _type;

		std::shared_ptr<ButtonWithTextAndSprite> _add_prefab;
		std::shared_ptr<ButtonWithTextAndSprite> _duplicate_prefab;
		std::shared_ptr<ButtonWithTextAndSprite> _remove_prefab;

		MainPanel(sf::Vector2i margin);
		~MainPanel();

		void setButtonsActivity();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};
}
