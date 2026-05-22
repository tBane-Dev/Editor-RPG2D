#pragma once
#include "Components/Panel.hpp"
#include "Controls/ButtonWithSprite.hpp"
#include "Animator.hpp"

class PreviewPanel : public Panel {
public:

	std::unique_ptr<sf::Text> _title;

	std::shared_ptr<Animator> _animator;

	std::shared_ptr<ButtonWithSprite> _first;
	std::shared_ptr<ButtonWithSprite> _prev;
	std::shared_ptr<ButtonWithSprite> _play;
	std::shared_ptr<ButtonWithSprite> _pause;
	std::shared_ptr<ButtonWithSprite> _next;
	std::shared_ptr<ButtonWithSprite> _last;

	PreviewPanel(sf::Vector2i margin);
	~PreviewPanel();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();

};