#pragma once
#include "Components/Panel.hpp"
#include "Controls/TextInput.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"

namespace AnimationsEditor {
	class PreviewPanel : public Components::Panel {
	public:

		std::unique_ptr<sf::Text> _title;

		std::shared_ptr<ButtonWithSprite> _first;
		std::shared_ptr<ButtonWithSprite> _prev;
		std::shared_ptr<ButtonWithSprite> _play;
		std::shared_ptr<ButtonWithSprite> _pause;
		std::shared_ptr<ButtonWithSprite> _next;
		std::shared_ptr<ButtonWithSprite> _last;

		std::shared_ptr<ButtonWithSprite> _anim_prev;
		std::shared_ptr<ButtonWithSprite> _anim_next;

		sf::IntRect _statsRect;
		std::unique_ptr<sf::Text> _animations_current_label;
		std::unique_ptr<sf::Text> _animations_count_label;
		std::unique_ptr<sf::Text> _frame_label;
		std::unique_ptr<sf::Text> _frames_count_label;
		std::unique_ptr<sf::Text> _frame_size_label;

		std::unique_ptr<sf::Text> _animations_current;
		std::unique_ptr<sf::Text> _animations_count;
		std::unique_ptr<sf::Text> _frame;
		std::unique_ptr<sf::Text> _frames_count;
		std::unique_ptr<sf::Text> _frame_size;

		PreviewPanel(sf::Vector2i margin);
		~PreviewPanel();

		void loadAnimations();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};
}
