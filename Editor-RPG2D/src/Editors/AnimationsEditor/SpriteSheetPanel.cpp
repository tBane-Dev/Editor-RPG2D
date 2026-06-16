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
			sf::Vector2i(_rect.position.x + 512 + 64, _rect.position.y + 48)
		);

		_loadBtn->_onclick_func = [this]() {

			std::shared_ptr<FileDialog> fileDialog = std::make_shared<FileDialog>(L"Load Spritesheet");
			
			std::function<void()> function = [this, fileDialog]() {

				std::shared_ptr<sf::Image> image = std::make_shared<sf::Image>();
				
				if (!image->loadFromFile(fileDialog->getPathFile())) {
					std::wcout << L"Nie udało się wczytać obrazka: "
						<< fileDialog->getPathFile() << std::endl;
					return;
				}

				std::shared_ptr<Texture> texture = std::make_shared<Texture>(fileDialog->getPathFile(), image);

				if (editor->_animations.expired()) {
					editor->_animations = std::make_shared<Animations>(fileDialog->getPathFile(), texture, 1, 1);
				}
				else {
					editor->_animations.lock()->_texture = texture;
					editor->_animations.lock()->_framesCount = 1;
					editor->_animations.lock()->_animationsCount = 1;
				}
					

				loadAnimations();

				if (editor->_preview_panel) {
					editor->_preview_panel->loadAnimations();
				}

			};

			fileDialog->setFunction(function);
			Main::windows_manager->push_back(fileDialog);
		};

		// text inputs
		sf::Vector2i startPosition(_rect.position.x + 512 + 96, _rect.position.y + 64 + 64);
		int distance = 16;

		_x = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"--", 24, 18);
		_x->setPosition(sf::Vector2i(startPosition.x, startPosition.y));
		_x->setText(L"0");

		_y = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"--", 24, 18);
		_y->setPosition(sf::Vector2i(startPosition.x, _x->getPosition().y + _x->getSize().y + distance));
		_y->setText(L"0");

		_w = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"--", 24, 18);
		_w->setPosition(sf::Vector2i(startPosition.x, _y->getPosition().y + _y->getSize().y + distance));
		_w->setText(L"0");

		_h = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"--", 24, 18);
		_h->setPosition(sf::Vector2i(startPosition.x, _w->getPosition().y + _w->getSize().y + distance));
		_h->setText(L"0");

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

		// text inputs functions
		_x->_onEditedFunction = [this]() {
			if (!editor->_animations.expired()) {
				DebugLog(L"ta funkcja jeszcze nie działa");
			}
		};

		_y->_onEditedFunction = [this]() {
			if (!editor->_animations.expired()) {
				DebugLog(L"ta funkcja jeszcze nie działa");
			}
		};

		
		_w->_onEditedFunction = [this]() {
			if (!editor->_animations.expired()) {
				int w = std::stoi(_w->getText());
				if (w > 0 && w <= editor->_animations.lock()->getTexture()->getSize().x) {
					editor->_animations.lock()->_framesCount = editor->_animations.lock()->getTexture()->getSize().x / w; // TO-DO - to delete
					editor->_preview_panel->loadAnimations();
				}
					
			}
		};

		_h->_onEditedFunction = [this]() {
			if (!editor->_animations.expired()) {
				int h = std::stoi(_h->getText());
				if (h > 0 && h <= editor->_animations.lock()->getTexture()->getSize().y) {
					editor->_animations.lock()->_animationsCount = editor->_animations.lock()->getTexture()->getSize().y / h; // TO-DO - to delete
					editor->_preview_panel->loadAnimations();
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

		if (!editor->_animator)
			return;

		if(editor->_animations.expired())
			return;

		_x->setText(L"0");
		_y->setText(L"0");
		_w->setText(std::to_wstring(editor->_animations.lock()->getTexture()->getSize().x / editor->_animations.lock()->_framesCount));
		_h->setText(std::to_wstring(editor->_animations.lock()->getTexture()->getSize().y / editor->_animations.lock()->_animationsCount));
	}

	void SpriteSheetPanel::cursorHover() {
		Components::Panel::cursorHover();

		_loadBtn->cursorHover();

		_x->cursorHover();
		_y->cursorHover();
		_w->cursorHover();
		_h->cursorHover();
	}

	void SpriteSheetPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);

		_loadBtn->handleEvent(event);

		_x->handleEvent(event);
		_y->handleEvent(event);
		_w->handleEvent(event);
		_h->handleEvent(event);
	}

	void SpriteSheetPanel::update() {
		Components::Panel::update();

		_loadBtn->update();

		_x->update();
		_y->update();
		_w->update();
		_h->update();
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
		if (!editor->_animations.expired()) {

			int width = editor->_animations.lock()->getTexture()->_texture->getSize().x;
			int height = editor->_animations.lock()->getTexture()->_texture->getSize().y;

			float scaleX = rect.getSize().x / width;
			float scaleY = rect.getSize().y / height;

			scale = std::min(scaleX, scaleY);

			sf::Sprite sprite(*editor->_animations.lock()->getTexture()->_texture);
			sprite.setScale(sf::Vector2f(scale, scale));
			sprite.setPosition(rect.getPosition());
			Main::render_window->draw(sprite);
		}

		// draw grid
		float gridWidth = 1.0f;
		float mainGridMultiplier = 4.0f;
		sf::Vector2f position_rect = rect.getPosition();
		sf::Vector2f size_rect = rect.getSize();


		grid2_shader->setUniform("rectPosition", position_rect);
		grid2_shader->setUniform("rectSize", sf::Vector2f(rect.getSize()));
		grid2_shader->setUniform("gridSize", sf::Vector2f(16.0f * scale, 16.0f * scale));
		grid2_shader->setUniform("chunkSize", 
			sf::Vector2f(
				(!editor->_animations.expired())?(float)editor->_animations.lock()->getFrameRect(0, 0).size.x * scale : 128,
				(!editor->_animations.expired())?(float)editor->_animations.lock()->getFrameRect(0, 0).size.y * scale : 128
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

		// text labels
		Main::render_window->draw(*_xLabel);
		Main::render_window->draw(*_yLabel);
		Main::render_window->draw(*_wLabel);
		Main::render_window->draw(*_hLabel);

		
	}
}