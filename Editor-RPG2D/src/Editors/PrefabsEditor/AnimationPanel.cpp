#include "Editors/PrefabsEditor/AnimationPanel.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "Theme.hpp"
#include "ShaderManager.hpp"
#include "Window.hpp"

AnimationPanel::AnimationPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 800), sf::Vector2i(420 + margin.x, prefabs_editor->_main_menu->getSize().y + margin.y)) {
	_title = std::make_unique<sf::Text>(basicFont, L"Preview", 20);
	_title->setFillColor(basic_text_color);
	_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));

	_animator = std::make_shared<Animator>(prefabs_editor->_object->getAnimations(), 0.2f);

	_first = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\first.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\first_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\first_press.png"));

	_prev = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\prev.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\prev_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\prev_press.png"));
	
	_play = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\play.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\play_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\play_press.png"));
	
	_pause = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\pause.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\pause_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\pause_press.png"));
	
	_next = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\next.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\next_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\next_press.png"));
	
	_last = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\last.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\last_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\preview_panel\\last_press.png"));

	int padding = 10;
	int total_width = _rect.size.x - padding * 2;
	int btn_w = 64;
	int buttons_count = 5;
	int buttons_width = buttons_count * btn_w;
	int margin_between_buttons = (total_width - buttons_width) / buttons_count / 4;
	int all_width = buttons_width + margin_between_buttons * (buttons_count-1);

	sf::Vector2f startPosition;
	startPosition.x = _rect.position.x + padding + (total_width - all_width) / 2;
	startPosition.y = _rect.position.y +_title->getFont().getLineSpacing(20) + 24 + 16 + 256 + 16;

	_first->setPosition(sf::Vector2i(startPosition.x, startPosition.y));
	_prev->setPosition(sf::Vector2i(_first->getPosition().x + btn_w + margin_between_buttons, _first->getPosition().y));
	_play->setPosition(sf::Vector2i(_prev->getPosition().x + btn_w + margin_between_buttons, _prev->getPosition().y));
	_pause->setPosition(sf::Vector2i(_play->getPosition().x, _play->getPosition().y));
	_next->setPosition(sf::Vector2i(_pause->getPosition().x + btn_w + margin_between_buttons, _pause->getPosition().y));
	_last->setPosition(sf::Vector2i(_next->getPosition().x + btn_w + margin_between_buttons, _next->getPosition().y));

	_first->_onclick_func = [this]() {
		if (_animator)
			_animator->setFrame(0);
		};

	_prev->_onclick_func = [this]() {
		if (_animator)
			_animator->prevFrame();
		};


	_play->_onclick_func = [this]() {
		if (_animator)
			_animator->play();
	};

	_pause->_onclick_func = [this]() {
		if (_animator)
			_animator->pause();
		};

	_next->_onclick_func = [this]() {
		if (_animator)
			_animator->nextFrame();
		};

	_last->_onclick_func = [this]() {
		if (_animator)
			_animator->lastFrame();
		};
}

AnimationPanel::~AnimationPanel() {

}

void AnimationPanel::cursorHover() {
	Panel::cursorHover();

	_first->cursorHover();
	_prev->cursorHover();
	(_animator == nullptr || _animator->_isPlaying) ? _pause->cursorHover() : _play->cursorHover();
	_next->cursorHover();
	_last->cursorHover();

}

void AnimationPanel::handleEvent(const sf::Event& event) {
	Panel::handleEvent(event);

	_first->handleEvent(event);
	_prev->handleEvent(event);
	(_animator == nullptr || _animator->_isPlaying) ? _pause->handleEvent(event) : _play->handleEvent(event);
	_next->handleEvent(event);
	_last->handleEvent(event);
}

void AnimationPanel::update() {
	Panel::update();

	_first->update();
	_prev->update();
	_play->update();
	_pause->update();
	_next->update();
	_last->update();

	_animator->update();
}

void AnimationPanel::draw() {
	Panel::draw();

	window->draw(*_title);

	// draw checkerboard
	sf::RectangleShape rect(sf::Vector2f(sf::Vector2f(256, 256)));
	rect.setFillColor(sf::Color::Red);
	rect.setPosition(sf::Vector2f(getPosition().x + (getSize().x-256)/2, getPosition().y + _title->getFont().getLineSpacing(20) + 24 + 16));

	sf::RenderStates states;
	states.shader = &*checkerboard_shader;

	checkerboard_shader->setUniform("rectPos", rect.getPosition());
	checkerboard_shader->setUniform("rectSize", rect.getSize());

	window->draw(rect, states);

	// draw animation
	std::shared_ptr<Animations> animations = _animator->getAnimations();
	sf::IntRect frameRect = animations->getFrameRect(_animator->_animation, _animator->_frame);

	sf::Sprite sprite(*animations->getTexture()->_texture);
	sprite.setTextureRect(frameRect);
	sprite.setScale(sf::Vector2f(256.f / (float)frameRect.size.x, 256.f / (float)frameRect.size.y));
	sprite.setPosition(sf::Vector2f(rect.getPosition().x, rect.getPosition().y));
	window->draw(sprite);

	// draw buttons
	_first->draw();
	_prev->draw();
	(_animator == nullptr || _animator->_isPlaying)? _pause->draw() : _play->draw();
	_next->draw();
	_last->draw();

}