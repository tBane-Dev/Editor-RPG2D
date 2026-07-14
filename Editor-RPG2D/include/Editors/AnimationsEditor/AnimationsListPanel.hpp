#pragma once
#include "Components/ListPanel.hpp"

namespace AnimationsEditor {
	class AnimationsListPanel : public Components::ListPanel {
	public:

		AnimationsListPanel(sf::Vector2i margin);
		~AnimationsListPanel();
		
		virtual void loadList();

	};

}