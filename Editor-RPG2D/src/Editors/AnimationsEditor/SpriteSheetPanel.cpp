#include "Editors/AnimationsEditor/SpriteSheetPanel.hpp"
#include "Editors/AnimationsEditor/Editor.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"
#include "Objects/Monster.hpp"
#include "PrefabsManager.hpp"
#include "Windows/FileDialog/FileDialog.hpp"

namespace AnimationsEditor {

	SpriteSheetPanel::SpriteSheetPanel(sf::Vector2i margin) : Panel(sf::Vector2i(912, 712 + 60), sf::Vector2i(420 + margin.x, AnimationsEditor::editor->_main_menu->getSize().y + margin.y + 128)) {

		_title = std::make_unique<sf::Text>(basicFont, L"Sprite Sheet", 20);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));
	
		_loadBtn = std::make_shared<ButtonWithTextAndSprite>(
			L"Load Spritesheet",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\mediumButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\mediumButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\mediumButton_press.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\mediumButton_inactive.png"),
			sf::Vector2i(_rect.position.x + 512 + 64, _rect.position.y + 48)
		);

		_loadBtn->_onclick_func = [this]() {

			std::shared_ptr<FileDialog> fileDialog = std::make_shared<FileDialog>(L"Load Spritesheet");
			fileDialog->setPosition(sf::Vector2i(160, 128));

			std::function<void()> function = [this, fileDialog]() {

				std::shared_ptr<sf::Image> image = std::make_shared<sf::Image>();
				
				if (!image->loadFromFile(fileDialog->getPathFile())) {
					std::wcout << L"Nie udało się wczytać obrazka: "
						<< fileDialog->getPathFile() << std::endl;
					return;
				}

				std::shared_ptr<Texture> texture = std::make_shared<Texture>(fileDialog->getPathFile(), image);

				if (!editor->_animations) {
					editor->_animations = std::make_shared<Animations>(fileDialog->getPathFile(), texture, sf::Vector2i(texture->getSize()), 1, 1);
				}
				
				if (!editor->_tempAnimations) {
					editor->_tempAnimations = std::make_shared<Animations>(editor->_animations->_path, texture, sf::Vector2i(texture->getSize()), 1, 1);
				}
				else {
					editor->_tempAnimations->_texture = texture;
					editor->_tempAnimations->_offsetX = 0;
					editor->_tempAnimations->_offsetY = 0;
					editor->_tempAnimations->_frameSize = texture->getSize();
					editor->_tempAnimations->_animationsCount = 1;
					editor->_tempAnimations->_framesCount = 1;
				}

				editor->_animator = std::make_shared<Animator>(editor->_tempAnimations, 0.2f);

				AnimationsEditor::editor->_sprite_sheet_panel->setTextInputsRange();
				AnimationsEditor::editor->_sprite_sheet_panel->loadAnimations();
				AnimationsEditor::editor->_actions_panel->setButtonsActivity();
				AnimationsEditor::editor->_actions_panel->setTooltips();
				AnimationsEditor::editor->_preview_panel->loadAnimations();
				AnimationsEditor::editor->_preview_panel->setButtonsActivity();
				AnimationsEditor::editor->_preview_panel->setTooltips();


				
			};

			fileDialog->setFunction(function);
			Main::windows_manager->push_back(fileDialog);
		};

		// text inputs
		sf::Vector2i startPosition(_rect.position.x + 512 + 96, _rect.position.y + 64 + 64);
		int distance = 16;

		_x = std::make_shared<NumberInput>(sf::Vector2i(256, 30), L"--", 24, 18);
		_x->setPosition(sf::Vector2i(startPosition.x, startPosition.y));
		_x->setRange(0, 0);

		_y = std::make_shared<NumberInput>(sf::Vector2i(256, 30), L"--", 24, 18);
		_y->setPosition(sf::Vector2i(startPosition.x, _x->getPosition().y + _x->getSize().y + distance));
		_y->setRange(0, 0);

		_w = std::make_shared<NumberInput>(sf::Vector2i(256, 30), L"--", 24, 18);
		_w->setPosition(sf::Vector2i(startPosition.x, _y->getPosition().y + _y->getSize().y + distance));
		_w->setRange(0, 0);

		_h = std::make_shared<NumberInput>(sf::Vector2i(256, 30), L"--", 24, 18);
		_h->setPosition(sf::Vector2i(startPosition.x, _w->getPosition().y + _w->getSize().y + distance));
		_h->setRange(0, 0);

		_a = std::make_shared<NumberInput>(sf::Vector2i(256, 30), L"--", 24, 18);
		_a->setPosition(sf::Vector2i(startPosition.x, _h->getPosition().y + _h->getSize().y + distance));
		_a->setRange(0, 0);

		_f = std::make_shared<NumberInput>(sf::Vector2i(256, 30), L"--", 24, 18);
		_f->setPosition(sf::Vector2i(startPosition.x, _a->getPosition().y + _a->getSize().y + distance));
		_f->setRange(0, 0);

		// texts labels
		int x = _x->getPosition().x - 32;

		_xLabel = std::make_unique<sf::Text>(basicFont, L"x", 18);
		_xLabel->setFillColor(basic_text_color);
		_xLabel->setPosition(sf::Vector2f(x - _xLabel->getGlobalBounds().size.x / 2, _x->getPosition().y));

		_yLabel = std::make_unique<sf::Text>(basicFont, L"y", 18);
		_yLabel->setFillColor(basic_text_color);
		_yLabel->setPosition(sf::Vector2f(x - _yLabel->getGlobalBounds().size.x / 2, _y->getPosition().y));

		_wLabel = std::make_unique<sf::Text>(basicFont, L"w", 18);
		_wLabel->setFillColor(basic_text_color);
		_wLabel->setPosition(sf::Vector2f(x - _wLabel->getGlobalBounds().size.x / 2, _w->getPosition().y));

		_hLabel = std::make_unique<sf::Text>(basicFont, L"h", 18);
		_hLabel->setFillColor(basic_text_color);
		_hLabel->setPosition(sf::Vector2f(x - _hLabel->getGlobalBounds().size.x / 2, _h->getPosition().y));

		_aLabel = std::make_unique<sf::Text>(basicFont, L"a", 18);
		_aLabel->setFillColor(basic_text_color);
		_aLabel->setPosition(sf::Vector2f(x - _aLabel->getGlobalBounds().size.x / 2, _a->getPosition().y));

		_fLabel = std::make_unique<sf::Text>(basicFont, L"f", 18);
		_fLabel->setFillColor(basic_text_color);
		_fLabel->setPosition(sf::Vector2f(x - _fLabel->getGlobalBounds().size.x / 2, _f->getPosition().y));

		// text inputs functions
		_x->_onEditedFunction = [this]() {
			if (editor->_animations) {
				int x = _x->getNumber();
				if (x >= 0 && x < (!editor->_tempAnimations->getTexture())? 0 : editor->_tempAnimations->getTexture()->getSize().x) {
					editor->_tempAnimations->_offsetX = x;
					editor->_preview_panel->loadAnimations();
					editor->_actions_panel->setButtonsActivity();
				}
			}
		};

		_y->_onEditedFunction = [this]() {
			if (editor->_animations) {
				int y = _y->getNumber();
				if (y >= 0 && y < (!editor->_tempAnimations->getTexture()) ? 0 : editor->_tempAnimations->getTexture()->getSize().y) {
					editor->_tempAnimations->_offsetY = y;
					editor->_preview_panel->loadAnimations();
					editor->_actions_panel->setButtonsActivity();
				}
			}
		};

		
		_w->_onEditedFunction = [this]() {
			if (editor->_animations) {
				int w = _w->getNumber();
				if (editor->_tempAnimations->getTexture() && w > 0 && w <= editor->_tempAnimations->getTexture()->getSize().x) {
					editor->_tempAnimations->_frameSize.x = w;
					editor->_preview_panel->loadAnimations();
					editor->_actions_panel->setButtonsActivity();
				}	
			}
		};

		_h->_onEditedFunction = [this]() {
			if (editor->_animations) {
				int h = _h->getNumber();
				if (editor->_tempAnimations->getTexture() && h > 0 && h <= editor->_tempAnimations->getTexture()->getSize().y) {
					editor->_tempAnimations->_frameSize.y = h;
					editor->_preview_panel->loadAnimations();
					editor->_actions_panel->setButtonsActivity();
				}
			}
		};

		_a->_onEditedFunction = [this]() {
			if (editor->_animations) {
				int a = _a->getNumber();
				if (a > 0 && a <= editor->_tempAnimations->getTexture()->getSize().y) {
					editor->_tempAnimations->_animationsCount = a;
					editor->_preview_panel->loadAnimations();
					editor->_actions_panel->setButtonsActivity();
				}
			}
		};

		_f->_onEditedFunction = [this]() {
			if (editor->_animations) {
				int f = _f->getNumber();
				if (f > 0 && f <= editor->_tempAnimations->getTexture()->getSize().y) {
					editor->_tempAnimations->_framesCount = f;
					editor->_preview_panel->loadAnimations();
					editor->_actions_panel->setButtonsActivity();
				}
			}
		};


		loadAnimations();
	}

	SpriteSheetPanel::~SpriteSheetPanel() {

	}

	void SpriteSheetPanel::loadAnimations() {

		_x->setText(L"");
		_y->setText(L"");
		_w->setText(L"");
		_h->setText(L"");
		_a->setText(L"");
		_f->setText(L"");

		if (!editor->_tempAnimations)
			return;

		_x->setText(std::to_wstring(editor->_tempAnimations->_offsetX));
		_y->setText(std::to_wstring(editor->_tempAnimations->_offsetY));
		_w->setText(std::to_wstring(editor->_tempAnimations->_frameSize.x));
		_h->setText(std::to_wstring(editor->_tempAnimations->_frameSize.y));
		_a->setText(std::to_wstring(editor->_tempAnimations->_animationsCount));
		_f->setText(std::to_wstring(editor->_tempAnimations->_framesCount));
	}

	void SpriteSheetPanel::setTextInputsRange() {

		if (!editor->_tempAnimations || !editor->_tempAnimations->_texture) return;

		_x->setMaxValue(editor->_tempAnimations->_texture->getSize().x);
		_y->setMaxValue(editor->_tempAnimations->_texture->getSize().y);
		_w->setMaxValue(editor->_tempAnimations->_texture->getSize().x);
		_h->setMaxValue(editor->_tempAnimations->_texture->getSize().y);
		_a->setMaxValue(editor->_tempAnimations->_texture->getSize().y);
		_f->setMaxValue(editor->_tempAnimations->_texture->getSize().x);
	}

	void SpriteSheetPanel::cursorHover() {
		Components::Panel::cursorHover();

		_loadBtn->cursorHover();

		_x->cursorHover();
		_y->cursorHover();
		_w->cursorHover();
		_h->cursorHover();
		_a->cursorHover();
		_f->cursorHover();
	}

	void SpriteSheetPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);

		_loadBtn->handleEvent(event);

		_x->handleEvent(event);
		_y->handleEvent(event);
		_w->handleEvent(event);
		_h->handleEvent(event);
		_a->handleEvent(event);
		_f->handleEvent(event);
	}

	void SpriteSheetPanel::update() {
		Components::Panel::update();

		_loadBtn->update();

		_x->update();
		_y->update();
		_w->update();
		_h->update();
		_a->update();
		_f->update();
	}

	void SpriteSheetPanel::draw() {
		Components::Panel::draw();
		
		// draw title
		Main::render_window->draw(*_title);

		// draw load button
		_loadBtn->draw();

		// draw checkerboard
		sf::RectangleShape rect(sf::Vector2f(sf::Vector2f(512, 512)));
		rect.setFillColor(sf::Color::White);
		rect.setPosition(sf::Vector2f(getPosition().x + 32, getPosition().y + _title->getFont().getLineSpacing(20) + 24 + 16));
		sf::RenderStates states;
		states.shader = &*checkerboard_shader;
		checkerboard_shader->setUniform("rectPos", rect.getPosition());
		Main::render_window->draw(rect, states);

		// draw sprite sheet
		float scale = 1;
		if (editor->_tempAnimations && editor->_tempAnimations->getTexture()) {

			int width = editor->_tempAnimations->getTexture()->_texture->getSize().x;
			int height = editor->_tempAnimations->getTexture()->_texture->getSize().y;

			float scaleX = rect.getSize().x / width;
			float scaleY = rect.getSize().y / height;

			scale = std::min(scaleX, scaleY);

			sf::Sprite sprite(*editor->_tempAnimations->getTexture()->_texture);
			sprite.setScale(sf::Vector2f(scale, scale));
			sprite.setPosition(rect.getPosition());
			Main::render_window->draw(sprite);
		}

		// draw grid
		float gridWidth = 1.0f;
		float mainGridMultiplier = 4.0f;
		sf::Vector2f position_rect = rect.getPosition();
		if (_x->getNumber() >= 0) position_rect.x += (float)_x->getNumber() * scale;
		if (_y->getNumber() >= 0) position_rect.y += (float)_y->getNumber() * scale;

		// TO-DO - must be a number input
		sf::Vector2f size_rect;
		if (_f->getNumber() >= 0 && _w->getNumber() >= 0) size_rect.x = ((float)_f->getNumber() * (float)_w->getNumber()) * scale;
		if (_a->getNumber() >= 0 && _h->getNumber() >= 0) size_rect.y = ((float)_a->getNumber() * (float)_h->getNumber()) * scale;
		//
		
		// TO-DO - must be a number input
		if (size_rect.x > 512 - (_x->getNumber() >= 0 ? (float)_x->getNumber() * scale : 0)) size_rect.x = 512 - (_x->getNumber() >= 0 ? (float)_x->getNumber() * scale : 0);
		if (size_rect.y > 512 - (_y->getNumber() >= 0 ? (float)_y->getNumber() * scale : 0)) size_rect.y = 512 - (_y->getNumber() >= 0 ? (float)_y->getNumber() * scale : 0);
		//

		grid2_shader->setUniform("rectPosition", position_rect);
		grid2_shader->setUniform("rectSize", size_rect);
		grid2_shader->setUniform("gridSize", sf::Vector2f(16.0f * scale, 16.0f * scale));
		grid2_shader->setUniform("chunkSize", 
			sf::Vector2f(
				// TO-DO
				(editor->_sprite_sheet_panel->_w->getNumber() > 0) ? (float)editor->_sprite_sheet_panel->_w->getNumber() * scale : 128,
				(editor->_sprite_sheet_panel->_h->getNumber() > 0) ? (float)editor->_sprite_sheet_panel->_h->getNumber() * scale : 128
			)
		);
		grid2_shader->setUniform("gridWidth", gridWidth);
		grid2_shader->setUniform("mainGridMultiplier", mainGridMultiplier);
		grid2_shader->setUniform("gridMainColor", sf::Glsl::Vec4(sf::Color(127, 7, 7, 255)));
		grid2_shader->setUniform("gridSecondColor", sf::Glsl::Vec4(sf::Color(47, 47, 47, 255)));

		sf::RenderStates rs;
		rs.shader = &*grid2_shader;

		sf::RectangleShape gridRect(size_rect);
		gridRect.setPosition(position_rect);
		gridRect.setFillColor(sf::Color(127, 47, 47, 127));
		Main::render_window->draw(gridRect, rs);

		// text inputs
		_x->draw();
		_y->draw();
		_w->draw();
		_h->draw();
		_a->draw();
		_f->draw();

		// text labels
		Main::render_window->draw(*_xLabel);
		Main::render_window->draw(*_yLabel);
		Main::render_window->draw(*_wLabel);
		Main::render_window->draw(*_hLabel);
		Main::render_window->draw(*_aLabel);
		Main::render_window->draw(*_fLabel);

		
	}
}