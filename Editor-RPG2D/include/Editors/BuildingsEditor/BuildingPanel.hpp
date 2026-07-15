#pragma once
#include "Components/Panel.hpp"

namespace BuildingsEditor {
	class BuildingPanel : public Components::Panel {
	public:

		BuildingPanel(sf::Vector2i margin);
		~BuildingPanel();

	};

}