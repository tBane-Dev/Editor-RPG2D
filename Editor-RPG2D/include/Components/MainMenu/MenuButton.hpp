#pragma once
#include "Controls/Button.hpp"
#include "Controls/Option.hpp"
#include "Components/MainMenu/MainMenu.hpp"

namespace Components {
	class MenuButton : public Button {
	public:
		std::shared_ptr<MainMenu> _menu;
		std::unique_ptr<sf::Text> _text;
		std::vector<std::shared_ptr<Option>> _options;

		MenuButton(std::shared_ptr<MainMenu> menu, std::wstring text);
		~MenuButton();

		void setText(std::wstring text);
		void resizeOptions();
		void addOption(std::shared_ptr<Option> option);
		void setPosition(sf::Vector2i position);

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();
	};

}