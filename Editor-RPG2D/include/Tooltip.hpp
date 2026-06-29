#pragma once
#include <SFML/Graphics.hpp>
#include <GUIManager.hpp>

namespace Main {
	class Tooltip {
	public:

		std::weak_ptr<Element> _element;
		sf::Time _timer;

		std::wstring _title;
		std::wstring _description;

		std::shared_ptr<sf::Text> _title_text;
		std::shared_ptr<sf::Text> _description_text;

		Tooltip();
		~Tooltip();

		void setElement(std::weak_ptr<Element> element, int width);
		float getHeightOfTitleAndDescription();

		void update();
		void draw();

	};

	extern std::shared_ptr<Tooltip> tooltip;
}
