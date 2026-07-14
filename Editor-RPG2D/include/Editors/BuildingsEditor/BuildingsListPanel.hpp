#pragma once
#include "Components/ListPanel.hpp"

namespace BuildingsEditor {
	class BuildingsListPanel : public Components::ListPanel {
	public:

		BuildingsListPanel(sf::Vector2i margin);
		~BuildingsListPanel();
		
		virtual void loadList();

	};

}