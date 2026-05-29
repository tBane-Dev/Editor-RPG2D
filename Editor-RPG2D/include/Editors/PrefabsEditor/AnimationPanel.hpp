#pragma once
#include "Components/Panel.hpp"
#include "Animator.hpp"
#include "Controls/ButtonWithSprite.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"

namespace PrefabsEditor {
	class AnimationPanel : public Components::Panel {
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
		std::unique_ptr<sf::Text> _animations_name_label;
		std::unique_ptr<sf::Text> _animations_current_label;
		std::unique_ptr<sf::Text> _animations_count_label;
		std::unique_ptr<sf::Text> _frame_label;
		std::unique_ptr<sf::Text> _frames_count_label;
		std::unique_ptr<sf::Text> _frame_size_label;

		std::unique_ptr<sf::Text> _animations_name;
		std::unique_ptr<sf::Text> _animations_current;
		std::unique_ptr<sf::Text> _animations_count;
		std::unique_ptr<sf::Text> _frame;
		std::unique_ptr<sf::Text> _frames_count;
		std::unique_ptr<sf::Text> _frame_size;

		std::shared_ptr<ButtonWithTextAndSprite> _set_animation;

		AnimationPanel(sf::Vector2i margin);
		~AnimationPanel();

		void loadStatsValues();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();

	};
}
