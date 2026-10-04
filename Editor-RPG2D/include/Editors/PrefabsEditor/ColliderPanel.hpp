#pragma once
#include "Components/Panel.hpp"
#include "Controls/TextInput.hpp"
#include "Controls/NumberInput.hpp"
#include "Objects/Collider.hpp"

namespace PrefabsEditor {
	class ColliderPanel : public Components::Panel {
	public:

		std::unique_ptr<sf::Text> _title;

		std::unique_ptr<sf::Text> _typeLabel;
		std::unique_ptr<sf::Text> _xLabel;
		std::unique_ptr<sf::Text> _yLabel;
		std::unique_ptr<sf::Text> _wLabel;
		std::unique_ptr<sf::Text> _hLabel;

		std::shared_ptr<TextInput> _type;

		std::shared_ptr<NumberInput> _x;
		std::shared_ptr<NumberInput> _y;
		std::shared_ptr<NumberInput> _w;
		std::shared_ptr<NumberInput> _h;

		ColliderPanel(sf::Vector2i margin);
		~ColliderPanel();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};
}