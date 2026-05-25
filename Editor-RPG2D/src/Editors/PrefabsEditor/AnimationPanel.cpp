#include "Editors/PrefabsEditor/AnimationPanel.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "Theme.hpp"
#include "ShaderManager.hpp"
#include "Window.hpp"

AnimationPanel::AnimationPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 850), sf::Vector2i(420 + margin.x, prefabs_editor->_main_menu->getSize().y + margin.y)) {
	_title = std::make_unique<sf::Text>(basicFont, L"Animation", 20);
	_title->setFillColor(basic_text_color);
	_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));

	// buttons
	_first = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\first.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\first_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\first_press.png"));

	_prev = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\prev.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\prev_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\prev_press.png"));
	
	_play = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\play.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\play_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\play_press.png"));
	
	_pause = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\pause.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\pause_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\pause_press.png"));
	
	_next = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\next.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\next_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\next_press.png"));
	
	_last = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\last.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\last_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\last_press.png"));

	_anim_prev = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\anim_prev.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\anim_prev_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\anim_prev_press.png"));

	_anim_next = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\anim_next.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\anim_next_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\anim_next_press.png"));

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

	_anim_prev->setPosition(sf::Vector2i(_first->getPosition().x, _first->getPosition().y + btn_w + margin_between_buttons));
	_anim_next->setPosition(sf::Vector2i(_last->getPosition().x + btn_w - 80, _anim_prev->getPosition().y));

	_first->_onclick_func = [this]() {
		if (prefabs_editor->_animator)
			prefabs_editor->_animator->setFrame(0);
		};

	_prev->_onclick_func = [this]() {
		if (prefabs_editor->_animator)
			prefabs_editor->_animator->prevFrame();
		};


	_play->_onclick_func = [this]() {
		if (prefabs_editor->_animator)
			prefabs_editor->_animator->play();
	};

	_pause->_onclick_func = [this]() {
		if (prefabs_editor->_animator)
			prefabs_editor->_animator->pause();
		};

	_next->_onclick_func = [this]() {
		if (prefabs_editor->_animator)
			prefabs_editor->_animator->nextFrame();
		};

	_last->_onclick_func = [this]() {
		if (prefabs_editor->_animator)
			prefabs_editor->_animator->lastFrame();
		};

	_anim_prev->_onclick_func = [this]() {
		if (prefabs_editor->_animator)
			prefabs_editor->_animator->prevAnimation();
		};

	_anim_next->_onclick_func = [this]() {
		if (prefabs_editor->_animator)
			prefabs_editor->_animator->nextAnimation();
		};

	// stats rect
	int m = 8;
	_statsRect = sf::IntRect(
		sf::Vector2i(
			_anim_prev->getPosition().x-16,
			_anim_prev->getPosition().y + _anim_prev->getSize().y + 16
		),
		sf::Vector2i(
			_anim_next->getPosition().x + _anim_next->getSize().x - _anim_prev->getPosition().x + 32,
			6 * (basicFont.getLineSpacing(basic_text_size)+m) + m)
	);

	_animations_name_label = std::make_unique<sf::Text>(basicFont, L"Name", basic_text_size);
	_animations_current_label = std::make_unique<sf::Text>(basicFont, L"Current Anim", basic_text_size);
	_animations_count_label = std::make_unique<sf::Text>(basicFont, L"Anim Count", basic_text_size);
	_frame_label = std::make_unique<sf::Text>(basicFont, L"Current Frame", basic_text_size);
	_frames_count_label = std::make_unique<sf::Text>(basicFont, L"Frames Count", basic_text_size);
	_frame_size_label = std::make_unique<sf::Text>(basicFont, L"Frame Size", basic_text_size);

	_animations_name_label->setFillColor(basic_text_color);
	_animations_current_label->setFillColor(basic_text_color);
	_animations_count_label->setFillColor(basic_text_color);
	_frame_label->setFillColor(basic_text_color);
	_frames_count_label->setFillColor(basic_text_color);
	_frame_size_label->setFillColor(basic_text_color);

	_animations_name_label->setPosition(sf::Vector2f(_statsRect.position.x + m, _statsRect.position.y + m));
	_animations_current_label->setPosition(sf::Vector2f(_statsRect.position.x + m, _statsRect.position.y + m + basicFont.getLineSpacing(basic_text_size) + m));
	_animations_count_label->setPosition(sf::Vector2f(_statsRect.position.x + m, _statsRect.position.y + m + 2 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frame_label->setPosition(sf::Vector2f(_statsRect.position.x + m, _statsRect.position.y + m + 3 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frames_count_label->setPosition(sf::Vector2f(_statsRect.position.x + m, _statsRect.position.y + m + 4 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frame_size_label->setPosition(sf::Vector2f(_statsRect.position.x + m, _statsRect.position.y + m + 5 * (basicFont.getLineSpacing(basic_text_size) + m)));

	_animations_name = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	_animations_current = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	_animations_count = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	_frame = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	_frames_count = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	_frame_size = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	
	_animations_name->setFillColor(basic_text_color);
	_animations_current->setFillColor(basic_text_color);
	_animations_count->setFillColor(basic_text_color);
	_frame->setFillColor(basic_text_color);
	_frames_count->setFillColor(basic_text_color);
	_frame_size->setFillColor(basic_text_color);

	int s = _statsRect.position.x + _statsRect.size.x;
	_animations_name->setPosition(sf::Vector2f(s - _animations_name->getGlobalBounds().size.x - m, _statsRect.position.y + m));
	_animations_current->setPosition(sf::Vector2f(s - _animations_current->getGlobalBounds().size.x - m, _statsRect.position.y + m + basicFont.getLineSpacing(basic_text_size) + m));
	_animations_count->setPosition(sf::Vector2f(s - _animations_count->getGlobalBounds().size.x - m, _statsRect.position.y + m + 2 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frame->setPosition(sf::Vector2f(s - _frames_count->getGlobalBounds().size.x - m, _statsRect.position.y + m + 3 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frames_count->setPosition(sf::Vector2f(s - _frames_count->getGlobalBounds().size.x - m, _statsRect.position.y + m + 4 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frame_size->setPosition(sf::Vector2f(s - _frame_size->getGlobalBounds().size.x - m, _statsRect.position.y + m + 5 * (basicFont.getLineSpacing(basic_text_size) + m)));

	loadStatsValues();

	_set_animation = std::make_shared<ButtonWithTextAndSprite>(
		L"Set Animation",
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\bottomButton.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\bottomButton_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\prefabs_editor\\bottomButton_press.png"),
		sf::Vector2i(_statsRect.position.x + _statsRect.size.x/2 - 192/2, _statsRect.position.y + _statsRect.size.y + 32)
	);
}

AnimationPanel::~AnimationPanel() {

}

void AnimationPanel::loadStatsValues() {

	if (!prefabs_editor)
		return;

	std::shared_ptr<Animator> animator = prefabs_editor->_animator;
	std::shared_ptr<Animations> animations = animator->getAnimations();

	std::wstring name = animations->_path;
	name = name.substr(name.find_first_of(L"\\/") + 1);
	_animations_name->setString(name);

	_animations_current->setString(std::to_wstring(animator->_animation));
	_animations_count->setString(std::to_wstring(animations->_animationsCount));

	_frame->setString(std::to_wstring(animator->_frame));
	_frames_count->setString(std::to_wstring(animations->_framesCount));

	sf::IntRect frameRect = animations->getFrameRect(animator->_animation, animator->_frame);
	_frame_size->setString(std::to_wstring(frameRect.size.x) + L" x " + std::to_wstring(frameRect.size.y));
	
	int m = 8;
	int s = _statsRect.position.x + _statsRect.size.x;

	_animations_name->setPosition(sf::Vector2f(s - _animations_name->getGlobalBounds().size.x - m, _statsRect.position.y + m));
	_animations_current->setPosition(sf::Vector2f(s - _animations_current->getGlobalBounds().size.x - m, _statsRect.position.y + m + basicFont.getLineSpacing(basic_text_size) + m));
	_animations_count->setPosition(sf::Vector2f(s - _animations_count->getGlobalBounds().size.x - m, _statsRect.position.y + m + 2 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frame->setPosition(sf::Vector2f(s - _frame->getGlobalBounds().size.x - m, _statsRect.position.y + m + 3 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frames_count->setPosition(sf::Vector2f(s - _frames_count->getGlobalBounds().size.x - m, _statsRect.position.y + m + 4 * (basicFont.getLineSpacing(basic_text_size) + m)));
	_frame_size->setPosition(sf::Vector2f(s - _frame_size->getGlobalBounds().size.x - m, _statsRect.position.y + m + 5 * (basicFont.getLineSpacing(basic_text_size) + m)));
}

void AnimationPanel::cursorHover() {
	Panel::cursorHover();

	_first->cursorHover();
	_prev->cursorHover();
	(prefabs_editor->_animator == nullptr || prefabs_editor->_animator->_isPlaying) ? _pause->cursorHover() : _play->cursorHover();
	_next->cursorHover();
	_last->cursorHover();

	_anim_prev->cursorHover();
	_anim_next->cursorHover();

	_set_animation->cursorHover();
}

void AnimationPanel::handleEvent(const sf::Event& event) {
	Panel::handleEvent(event);

	_first->handleEvent(event);
	_prev->handleEvent(event);
	(prefabs_editor->_animator == nullptr || prefabs_editor->_animator->_isPlaying) ? _pause->handleEvent(event) : _play->handleEvent(event);
	_next->handleEvent(event);
	_last->handleEvent(event);

	_anim_prev->handleEvent(event);
	_anim_next->handleEvent(event);


	_set_animation->handleEvent(event);
}

void AnimationPanel::update() {
	if (!prefabs_editor)
		return;

	Panel::update();

	_first->update();
	_prev->update();
	_play->update();
	_pause->update();
	_next->update();
	_last->update();

	_anim_prev->update();
	_anim_next->update();

	if(prefabs_editor->_animator)
		prefabs_editor->_animator->update();

	loadStatsValues();

	_set_animation->update();
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
	std::shared_ptr<Animator> animator = prefabs_editor->_animator;
	if (animator) {
		std::shared_ptr<Animations> animations = animator->getAnimations();
		sf::IntRect frameRect = animations->getFrameRect(animator->_animation, animator->_frame);

		sf::Sprite sprite(*animations->getTexture()->_texture);
		sprite.setTextureRect(frameRect);
		sprite.setScale(sf::Vector2f(rect.getSize().x / (float)frameRect.size.x, rect.getSize().y / (float)frameRect.size.y));
		sprite.setPosition(sf::Vector2f(rect.getPosition().x, rect.getPosition().y));
		window->draw(sprite);
	}

	// draw buttons
	_first->draw();
	_prev->draw();
	(animator == nullptr || animator->_isPlaying)? _pause->draw() : _play->draw();
	_next->draw();
	_last->draw();

	_anim_prev->draw();
	_anim_next->draw();

	// draw stats rect
	int m = 2;
	sf::RectangleShape statsRectShape(sf::Vector2f(_statsRect.size + sf::Vector2i(2*m, 2*m)));
	statsRectShape.setPosition(sf::Vector2f(_statsRect.position - sf::Vector2i(m, m)));
	statsRectShape.setFillColor(sf::Color(24, 24, 24, 255));
	statsRectShape.setOutlineThickness(m);
	statsRectShape.setOutlineColor(sf::Color(0, 0, 0, 255));
	window->draw(statsRectShape);

	// texts labels
	window->draw(*_animations_name_label);
	window->draw(*_animations_current_label);
	window->draw(*_animations_count_label);
	window->draw(*_frame_label);
	window->draw(*_frames_count_label);
	window->draw(*_frame_size_label);

	// text inputs
	window->draw(*_animations_name);
	window->draw(*_animations_current);
	window->draw(*_animations_count);
	window->draw(*_frame);
	window->draw(*_frames_count);
	window->draw(*_frame_size);

	// set animation button
	_set_animation->draw();
}