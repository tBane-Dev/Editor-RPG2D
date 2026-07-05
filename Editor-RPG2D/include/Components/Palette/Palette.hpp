#pragma once
#include "GUIManager.hpp"
#include "Controls/ButtonWithSprite.hpp"
#include "Objects/Object.hpp"
#include "Animator.hpp"
#include "Controls/Scrollbar.hpp"
#include "Components/Palette/Categories.hpp"
#include "Components/Palette/Tools.hpp"
#include "Components/Palette/Slots.hpp"
#include "Objects/Terrain.hpp"

namespace Components {
	class Palette : public Element {
	public:

		static std::vector<std::shared_ptr<Terrain>> terrains;

		sf::IntRect _rect;
		std::shared_ptr<Categories> _categories;
		std::shared_ptr<Tools> _tools;
		std::shared_ptr<Slots> _slots;

		int _minBrushSize = 1;
		int _maxBrushSize = 4;
		int _brushSize = 1;

		

		Palette();
		~Palette();

		sf::Vector2i getSize();
		virtual void loadAll(ObjectType type);
		void setPosition(sf::Vector2i position);

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		virtual void draw();
	};
}
