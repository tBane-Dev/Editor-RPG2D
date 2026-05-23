#include "Editors/PrefabsEditor/AnimationPanel.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "Theme.hpp"
#include "ShaderManager.hpp"
#include "Window.hpp"

AnimationPanel::AnimationPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 800), sf::Vector2i(420 + margin.x, prefabs_editor->_main_menu->getSize().y + margin.y)) {
	_title = std::make_unique<sf::Text>(basicFont, L"Animation", 20);
	_title->setFillColor(basic_text_color);
	_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));

	_animator = std::make_shared<Animator>(prefabs_editor->_object->getAnimations(), 0.2f);

	// buttons
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

	// stats rect
	int m = 8;
	_statsRect = sf::IntRect(
		sf::Vector2i(
			_first->getPosition().x-16,
			_first->getPosition().y + _first->getSize().y + 16
		),
		sf::Vector2i(
			_last->getPosition().x + _last->getSize().x - _first->getPosition().x + 32,
			6 * (basicFont.getLineSpacing(basic_text_size)+m) + m)
	);

	_animations_name = std::make_unique<sf::Text>(basicFont, L"Name", basic_text_size);
	_animations_current = std::make_unique<sf::Text>(basicFont, L"Current Anim", basic_text_size);
	_animations_count = std::make_unique<sf::Text>(basicFont, L"Anim Count", basic_text_size);
	_frame = std::make_unique<sf::Text>(basicFont, L"Current Frame", basic_text_size);
	_frames_count = std::make_unique<sf::Text>(basicFont, L"Frames Count", basic_text_size);
	_frame_size = std::make_unique<sf::Text>(basicFont, L"Frame Size", basic_text_size);

	_animations_name->setFillColor(basic_text_color);
	_animations_current->setFillColor(basic_text_color);
	_animations_count->setFillColor(basic_text_color);
	_frame->setFillColor(basic_text_color);
	_frames_count->setFillColor(basic_text_color);
	_frame_size->setFillColor(basic_text_color);

	_animations_name->setPosition(sf::Vector2f(_statsRect.position.x + m, _statsRect.position.y + m));
	_animations_current->setPosition(sf::Vector2f(_statsRect.position.x + m, _statsRect.position.y + m + basicFont.getLineSpacing(basic_text_size) + m));
	_animations_count->setPosition(sf::Vector2f(_statsRect.position.x + m, _statsRect.position.y + m + 2 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frame->setPosition(sf::Vector2f(_statsRect.position.x + m, _statsRect.position.y + m + 3 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frames_count->setPosition(sf::Vector2f(_statsRect.position.x + m, _statsRect.position.y + m + 4 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frame_size->setPosition(sf::Vector2f(_statsRect.position.x + m, _statsRect.position.y + m + 5 * (basicFont.getLineSpacing(basic_text_size) + m)));

	_animations_name_value = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	_animations_current_value = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	_animations_count_value = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	_frame_value = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	_frames_count_value = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	_frame_size_value = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);

	_animations_name_value->setFillColor(basic_text_color);
	_animations_current_value->setFillColor(basic_text_color);
	_animations_count_value->setFillColor(basic_text_color);
	_frame_value->setFillColor(basic_text_color);
	_frames_count_value->setFillColor(basic_text_color);
	_frame_size_value->setFillColor(basic_text_color);

	int s = _statsRect.position.x + _statsRect.size.x;
	_animations_name_value->setPosition(sf::Vector2f(s - _animations_name_value->getGlobalBounds().size.x - m, _statsRect.position.y + m));
	_animations_current_value->setPosition(sf::Vector2f(s - _animations_current_value->getGlobalBounds().size.x - m, _statsRect.position.y + m + basicFont.getLineSpacing(basic_text_size) + m));
	_animations_count_value->setPosition(sf::Vector2f(s - _animations_count_value->getGlobalBounds().size.x - m, _statsRect.position.y + m + 2 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frame_value->setPosition(sf::Vector2f(s - _frames_count_value->getGlobalBounds().size.x - m, _statsRect.position.y + m + 3 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frames_count_value->setPosition(sf::Vector2f(s - _frames_count_value->getGlobalBounds().size.x - m, _statsRect.position.y + m + 4 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frame_size_value->setPosition(sf::Vector2f(s - _frame_size_value->getGlobalBounds().size.x - m, _statsRect.position.y + m + 5 * (basicFont.getLineSpacing(basic_text_size) + m)));

	loadStatsValues();
}

AnimationPanel::~AnimationPanel() {

}

void AnimationPanel::loadStatsValues() {

	std::shared_ptr<Animations> animations = _animator->getAnimations();

	std::wstring name = animations->_path;
	name = name.substr(name.find_first_of(L"\\/") + 1);
	_animations_name_value->setString(name);

	_animations_current_value->setString(std::to_wstring(_animator->_animation));
	_animations_count_value->setString(std::to_wstring(animations->_animationsCount));

	_frame_value->setString(std::to_wstring(_animator->_frame));
	_frames_count_value->setString(std::to_wstring(animations->_framesCount));

	sf::IntRect frameRect = animations->getFrameRect(_animator->_animation, _animator->_frame);
	_frame_size_value->setString(std::to_wstring(frameRect.size.x) + L" x " + std::to_wstring(frameRect.size.y));
	
	int m = 8;
	int s = _statsRect.position.x + _statsRect.size.x;

	_animations_name_value->setPosition(sf::Vector2f(s - _animations_name_value->getGlobalBounds().size.x - m, _statsRect.position.y + m));
	_animations_current_value->setPosition(sf::Vector2f(s - _animations_current_value->getGlobalBounds().size.x - m, _statsRect.position.y + m + basicFont.getLineSpacing(basic_text_size) + m));
	_animations_count_value->setPosition(sf::Vector2f(s - _animations_count_value->getGlobalBounds().size.x - m, _statsRect.position.y + m + 2 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frame_value->setPosition(sf::Vector2f(s - _frame_value->getGlobalBounds().size.x - m, _statsRect.position.y + m + 3 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frames_count_value->setPosition(sf::Vector2f(s - _frames_count_value->getGlobalBounds().size.x - m, _statsRect.position.y + m + 4 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frame_size_value->setPosition(sf::Vector2f(s - _frame_size_value->getGlobalBounds().size.x - m, _statsRect.position.y + m + 5 * (basicFont.getLineSpacing(basic_text_size) + m)));
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
	loadStatsValues();
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

	// draw stats rect
	int m = 2;
	sf::RectangleShape statsRectShape(sf::Vector2f(_statsRect.size + sf::Vector2i(2*m, 2*m)));
	statsRectShape.setPosition(sf::Vector2f(_statsRect.position - sf::Vector2i(m, m)));
	statsRectShape.setFillColor(sf::Color(24, 24, 24, 255));
	statsRectShape.setOutlineThickness(m);
	statsRectShape.setOutlineColor(sf::Color(0, 0, 0, 255));
	window->draw(statsRectShape);

	window->draw(*_animations_name);
	window->draw(*_animations_current);
	window->draw(*_animations_count);
	window->draw(*_frame);
	window->draw(*_frames_count);
	window->draw(*_frame_size);

	window->draw(*_animations_name_value);
	window->draw(*_animations_current_value);
	window->draw(*_animations_count_value);
	window->draw(*_frame_value);
	window->draw(*_frames_count_value);
	window->draw(*_frame_size_value);
}