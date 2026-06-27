
#include "Editors/PrefabsEditor/AnimationPanel.hpp"
#include "Editors/PrefabsEditor/Editor.hpp"
#include "Theme.hpp"
#include "ShaderManager.hpp"
#include "RenderWindow.hpp"
#include "WindowsManager.hpp"
#include "Editors/PrefabsEditor/AnimationsListDialog.hpp"

namespace PrefabsEditor {

	AnimationPanel::AnimationPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 850), sf::Vector2i(420 + margin.x, PrefabsEditor::editor->_main_menu->getSize().y + margin.y)) {
		_title = std::make_unique<sf::Text>(basicFont, L"Animation", 20);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));

		// buttons
		_first = std::make_shared<ButtonWithSprite>(
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\first.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\first_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\first_press.png"));

		_prev = std::make_shared<ButtonWithSprite>(
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\prev.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\prev_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\prev_press.png"));

		_play = std::make_shared<ButtonWithSprite>(
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\play.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\play_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\play_press.png"));

		_pause = std::make_shared<ButtonWithSprite>(
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\pause.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\pause_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\pause_press.png"));

		_next = std::make_shared<ButtonWithSprite>(
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\next.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\next_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\next_press.png"));

		_last = std::make_shared<ButtonWithSprite>(
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\last.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\last_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\last_press.png"));

		_anim_prev = std::make_shared<ButtonWithSprite>(
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\anim_prev.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\anim_prev_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\anim_prev_press.png"));

		_anim_next = std::make_shared<ButtonWithSprite>(
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\anim_next.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\anim_next_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\anim_next_press.png"));

		int padding = 10;
		int total_width = _rect.size.x - padding * 2;
		int btn_w = 64;
		int buttons_count = 5;
		int buttons_width = buttons_count * btn_w;
		int margin_between_buttons = (total_width - buttons_width) / buttons_count / 4;
		int all_width = buttons_width + margin_between_buttons * (buttons_count - 1);

		sf::Vector2f startPosition;
		startPosition.x = _rect.position.x + padding + (total_width - all_width) / 2;
		startPosition.y = _rect.position.y + _title->getFont().getLineSpacing(20) + 24 + 16 + 256 + 16;

		_first->setPosition(sf::Vector2i(startPosition.x, startPosition.y));
		_prev->setPosition(sf::Vector2i(_first->getPosition().x + btn_w + margin_between_buttons, _first->getPosition().y));
		_play->setPosition(sf::Vector2i(_prev->getPosition().x + btn_w + margin_between_buttons, _prev->getPosition().y));
		_pause->setPosition(sf::Vector2i(_play->getPosition().x, _play->getPosition().y));
		_next->setPosition(sf::Vector2i(_pause->getPosition().x + btn_w + margin_between_buttons, _pause->getPosition().y));
		_last->setPosition(sf::Vector2i(_next->getPosition().x + btn_w + margin_between_buttons, _next->getPosition().y));

		_anim_prev->setPosition(sf::Vector2i(_first->getPosition().x, _first->getPosition().y + btn_w + margin_between_buttons));
		_anim_next->setPosition(sf::Vector2i(_last->getPosition().x + btn_w - 80, _anim_prev->getPosition().y));

		_first->_onclick_func = [this]() {
			if (editor->_animator)
				editor->_animator->setFrame(0);
			};

		_prev->_onclick_func = [this]() {
			if (editor->_animator)
				editor->_animator->prevFrame();
			};


		_play->_onclick_func = [this]() {
			if (editor->_animator)
				editor->_animator->play();
			};

		_pause->_onclick_func = [this]() {
			if (editor->_animator)
				editor->_animator->pause();
			};

		_next->_onclick_func = [this]() {
			if (editor->_animator)
				editor->_animator->nextFrame();
			};

		_last->_onclick_func = [this]() {
			if (editor->_animator)
				editor->_animator->lastFrame();
			};

		_anim_prev->_onclick_func = [this]() {
			if (editor->_animator)
				editor->_animator->prevAnimation();
			};

		_anim_next->_onclick_func = [this]() {
			if (editor->_animator)
				editor->_animator->nextAnimation();
			};

		// stats rect
		int m = 8;
		_statsRect = sf::IntRect(
			sf::Vector2i(
				_anim_prev->getPosition().x - 16,
				_anim_prev->getPosition().y + _anim_prev->getSize().y + 16
			),
			sf::Vector2i(
				_anim_next->getPosition().x + _anim_next->getSize().x - _anim_prev->getPosition().x + 32,
				6 * (basicFont.getLineSpacing(basic_text_size) + m) + m)
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
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\bottomButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\bottomButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\bottomButton_press.png"),
			sf::Vector2i(_statsRect.position.x + _statsRect.size.x / 2 - 192 / 2, _statsRect.position.y + _statsRect.size.y + 32)
		);

		

		_set_animation->_onclick_func = [this]() {

			std::shared_ptr<AnimationsListDialog> dialog = std::make_shared<AnimationsListDialog>();

			std::function<void()> func = [this, dialog]() {
				if (!dialog->_fileNameInput->getText().empty()) {
					std::wstring animName = dialog->_fileNameInput->getText();
					if (!animations_manager->getAnimations(animName).expired()) {

						if(!editor->_animator)
							editor->_animator = std::make_shared<Animator>(animations_manager->getAnimations(animName), 0.2f);
						else
							editor->_animator->_animations = animations_manager->getAnimations(animName);
						
						Main::windows_manager->pop_back();

					}
				}
				};

			dialog->_confirmButton->_onclick_func = func;

			Main::windows_manager->push_back(dialog);
		};
	}

	AnimationPanel::~AnimationPanel() {

	}

	void AnimationPanel::loadStatsValues() {

		if (!editor) return;

		std::shared_ptr<Animator> animator = editor->_animator;
		std::weak_ptr<Animations> animations = (animator) ? animator->getAnimations() : std::weak_ptr<Animations>();

		if (!animator || animations.expired()) {
			_animations_name->setString(L"--");
			_animations_current->setString(L"--");
			_animations_count->setString(L"--");
			_frame->setString(L"--");
			_frames_count->setString(L"--");
			_frame_size->setString(L"--");
		}
		else {

			std::wstring name = animations.lock()->_path;
			name = name.substr(name.find_first_of(L"\\/") + 1);
			_animations_name->setString(name);

			_animations_current->setString(std::to_wstring(animator->_animation));
			_animations_count->setString(std::to_wstring(animations.lock()->_animationsCount));

			_frame->setString(std::to_wstring(animator->_frame));
			_frames_count->setString(std::to_wstring(animations.lock()->_framesCount));

			sf::IntRect frameRect = animations.lock()->getFrameRect(animator->_animation, animator->_frame);
			_frame_size->setString(std::to_wstring(frameRect.size.x) + L" x " + std::to_wstring(frameRect.size.y));
		}



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
		Components::Panel::cursorHover();

		_first->cursorHover();
		_prev->cursorHover();
		(editor->_animator == nullptr || editor->_animator->_isPlaying) ? _pause->cursorHover() : _play->cursorHover();
		_next->cursorHover();
		_last->cursorHover();

		_anim_prev->cursorHover();
		_anim_next->cursorHover();

		_set_animation->cursorHover();
	}

	void AnimationPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);

		_first->handleEvent(event);
		_prev->handleEvent(event);
		(editor->_animator == nullptr || editor->_animator->_isPlaying) ? _pause->handleEvent(event) : _play->handleEvent(event);
		_next->handleEvent(event);
		_last->handleEvent(event);

		_anim_prev->handleEvent(event);
		_anim_next->handleEvent(event);


		_set_animation->handleEvent(event);
	}

	void AnimationPanel::update() {
		if (!editor)
			return;

		Components::Panel::update();

		_first->update();
		_prev->update();
		_play->update();
		_pause->update();
		_next->update();
		_last->update();

		_anim_prev->update();
		_anim_next->update();

		if (editor->_animator)
			editor->_animator->update();

		loadStatsValues();

		_set_animation->update();
	}

	void AnimationPanel::draw() {
		Components::Panel::draw();

		Main::render_window->draw(*_title);

		// draw checkerboard
		sf::RectangleShape rect(sf::Vector2f(sf::Vector2f(256, 256)));
		rect.setFillColor(sf::Color::Red);
		rect.setPosition(sf::Vector2f(getPosition().x + (getSize().x - 256) / 2, getPosition().y + _title->getFont().getLineSpacing(20) + 24 + 16));

		sf::RenderStates states;
		states.shader = &*checkerboard_shader;
		checkerboard_shader->setUniform("rectPos", rect.getPosition());
		Main::render_window->draw(rect, states);

		// draw animation
		std::shared_ptr<Animator> animator = editor->_animator;
		if (animator) {
			std::weak_ptr<Animations> animations = animator->getAnimations();
			if (!animations.expired()) {
				sf::IntRect frameRect = animations.lock()->getFrameRect(animator->_animation, animator->_frame);

				sf::Sprite sprite(*animations.lock()->getTexture()->_texture);
				sprite.setTextureRect(frameRect);
				sprite.setScale(sf::Vector2f(rect.getSize().x / (float)frameRect.size.x, rect.getSize().y / (float)frameRect.size.y));
				sprite.setPosition(sf::Vector2f(rect.getPosition().x, rect.getPosition().y));
				Main::render_window->draw(sprite);
			}
		}
			

		// draw buttons
		_first->draw();
		_prev->draw();
		(animator == nullptr || animator->_isPlaying) ? _pause->draw() : _play->draw();
		_next->draw();
		_last->draw();

		_anim_prev->draw();
		_anim_next->draw();

		// draw stats rect
		int m = 2;
		sf::RectangleShape statsRectShape(sf::Vector2f(_statsRect.size + sf::Vector2i(2 * m, 2 * m)));
		statsRectShape.setPosition(sf::Vector2f(_statsRect.position - sf::Vector2i(m, m)));
		statsRectShape.setFillColor(sf::Color(24, 24, 24, 255));
		statsRectShape.setOutlineThickness(m);
		statsRectShape.setOutlineColor(sf::Color(0, 0, 0, 255));
		Main::render_window->draw(statsRectShape);

		// texts labels
		Main::render_window->draw(*_animations_name_label);
		Main::render_window->draw(*_animations_current_label);
		Main::render_window->draw(*_animations_count_label);
		Main::render_window->draw(*_frame_label);
		Main::render_window->draw(*_frames_count_label);
		Main::render_window->draw(*_frame_size_label);

		// text inputs
		Main::render_window->draw(*_animations_name);
		Main::render_window->draw(*_animations_current);
		Main::render_window->draw(*_animations_count);
		Main::render_window->draw(*_frame);
		Main::render_window->draw(*_frames_count);
		Main::render_window->draw(*_frame_size);

		// set animation button
		_set_animation->draw();
	}
}