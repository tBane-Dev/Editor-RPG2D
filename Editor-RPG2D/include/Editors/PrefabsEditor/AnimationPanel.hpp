#pragma once
#include "Components/Panel.hpp"
#include "Controls/ButtonWithSprite.hpp"
#include "Animator.hpp"

class AnimationPanel : public Panel {
public:

	std::unique_ptr<sf::Text> _title;

	std::shared_ptr<Animator> _animator;

	std::shared_ptr<ButtonWithSprite> _first;
	std::shared_ptr<ButtonWithSprite> _prev;
	std::shared_ptr<ButtonWithSprite> _play;
	std::shared_ptr<ButtonWithSprite> _pause;
	std::shared_ptr<ButtonWithSprite> _next;
	std::shared_ptr<ButtonWithSprite> _last;

	sf::IntRect _statsRect;
	std::unique_ptr<sf::Text> _animations_name;
	std::unique_ptr<sf::Text> _animations_current;
	std::unique_ptr<sf::Text> _animations_count;
	std::unique_ptr<sf::Text> _frame;
	std::unique_ptr<sf::Text> _frames_count;
	std::unique_ptr<sf::Text> _frame_size;

	std::unique_ptr<sf::Text> _animations_name_value;
	std::unique_ptr<sf::Text> _animations_current_value;
	std::unique_ptr<sf::Text> _animations_count_value;
	std::unique_ptr<sf::Text> _frame_value;
	std::unique_ptr<sf::Text> _frames_count_value;
	std::unique_ptr<sf::Text> _frame_size_value;

	AnimationPanel(sf::Vector2i margin);
	~AnimationPanel();

	void loadStatsValues();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};