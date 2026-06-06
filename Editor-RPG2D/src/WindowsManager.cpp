#include "WindowsManager.hpp"
#include "Theme.hpp"
#include "Cursor.hpp"
#include "RenderWindow.hpp"
#include "Time.hpp"
#include "DebugLog.hpp"

namespace Main {

	int Window::border = 1;
	int Window::padding = 12;
	sf::Music Window::errorSound;

	Window::Window(std::wstring title, sf::Vector2i size, sf::Vector2i position) : Element() {
		_titleStr = title;
		_rect = sf::IntRect(position, size);

		// title rect
		_titleRect = sf::IntRect(sf::Vector2i(_rect.position.x + border, _rect.position.y + border), sf::Vector2i(_rect.size.x - 2*border, basic_text_rect_height));

		_titleText = std::make_unique<sf::Text>(basicFont, title, 20);
		_titleText->setFillColor(basic_text_color);
		_titleText->setPosition(sf::Vector2f(_rect.position) + sf::Vector2f(10, 4));

		_closeButton = std::make_shared<ButtonWithSprite>(
			textures_manager->getTexture(L"assets\\tex\\windows\\close.png"),
			textures_manager->getTexture(L"assets\\tex\\windows\\close_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\windows\\close_press.png"),
			_rect.position + sf::Vector2i(_rect.size.x - 32, + border)
			);

		_closeButton->_onclick_func = []() { Main::windows_manager->pop_back(); };

		// content rect
		_contentRect = sf::IntRect(sf::Vector2i(_rect.position.x + border, _rect.position.y + border + basic_text_rect_height), sf::Vector2i(_rect.size.x - 2 * border, _rect.size.y - 2 * border - basic_text_rect_height));

		_state = WindowState::Idle;
		_offset = sf::Vector2i(0, 0);

		// animation when clicking outside the window
		_animationClickedOutside = false;
		_animationStartTime = currentTime;

		if (!errorSound.openFromFile("C:\\Windows\\Media\\Windows Background.wav")) {
			DebugError(L"Failed to load error sound");
		}

	}

	Window::~Window() {

	}

	void Window::setSize(sf::Vector2i size) {

		_rect.size = size;

		_titleRect.size = sf::Vector2i(
			_rect.size.x - 2 * border,
			basic_text_rect_height
		);

		_closeButton->setPosition(
			_rect.position + sf::Vector2i(_rect.size.x - 32, border)
		);

		_contentRect.size = sf::Vector2i(
			_rect.size.x - 2 * border,
			_rect.size.y - 2 * border - basic_text_rect_height
		);

		setPosition(_rect.position);
	}

	sf::Vector2i Window::getContentPosition() {
		return _contentRect.position;
	}

	sf::Vector2i Window::getContentSize() {
		return _contentRect.size	;
	}

	

	void Window::setPosition(sf::Vector2i position) {
		_rect.position = position;

		_titleRect.position = _rect.position + sf::Vector2i(border, border);
		_titleText->setPosition(sf::Vector2f(_rect.position) + sf::Vector2f(10, 4));
		_closeButton->setPosition(_rect.position + sf::Vector2i(_rect.size.x - 32, border));

		_contentRect.position = _rect.position + sf::Vector2i(border, border + basic_text_rect_height);
	}

	void Window::cursorHover() {
		
		if (_rect.contains(Main::cursor->_position))
			GUI_manager->Element_hovered = this->shared_from_this();

		_closeButton->cursorHover();
	}

	void Window::handleEvent(const sf::Event& event) {

		if ((event.getIf<sf::Event::MouseButtonReleased>() || event.getIf<sf::Event::MouseButtonPressed>()) && GUI_manager->Element_pressed == nullptr && !_rect.contains(Main::cursor->_position)) {
			_animationClickedOutside = true;
			_animationStartTime = currentTime;
			errorSound.play();
			return;
		}

		if (_state == WindowState::Moving) {
			if (const auto* mv = event.getIf<sf::Event::MouseMoved>(); mv) {
				setPosition(Main::cursor->_position-_offset);
				GUI_manager->Element_pressed = this->shared_from_this();
			}

			if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
				
				_state = WindowState::Idle;

				if(GUI_manager->Element_pressed.get() == this)
					GUI_manager->Element_pressed = nullptr;
			}

		}

		_closeButton->handleEvent(event);

		if(GUI_manager->Element_pressed == _closeButton)
			return;

		if (_titleRect.contains(Main::cursor->_position)) {
			if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
				_state = WindowState::Moving;
				_offset = Main::cursor->_position - _rect.position;
				GUI_manager->Element_pressed = this->shared_from_this();
			}
		}

		
	}

	void Window::update() {
		_closeButton->update();

		if (_animationClickedOutside) {
			if ((currentTime - _animationStartTime).asSeconds() > 0.4f) {
				_animationClickedOutside = false;
			}
		}
	}

	void Window::draw() {

		int shadow_margin = 8;
		int shadow_border = 8;
		int cycles = 2;
		int animation = (_animationClickedOutside) ? (int)((std::sin((currentTime - _animationStartTime).asSeconds() * (float)cycles * 3.14159265f * 2.0f) * 0.5f + 0.5f) * 40.0f) : 0;

		sf::RectangleShape shadow_rect(sf::Vector2f(_rect.size) + sf::Vector2f(2 * shadow_margin, 2 * shadow_margin));
		shadow_rect.setPosition(sf::Vector2f(_rect.position) - sf::Vector2f(shadow_margin, shadow_margin));
		shadow_rect.setFillColor(sf::Color(0, 0, 0, 22 + animation));
		shadow_rect.setOutlineThickness(shadow_border);
		shadow_rect.setOutlineColor(sf::Color(0, 0, 0, 10 + animation));
		Main::render_window->draw(shadow_rect);

		sf::RectangleShape rect(sf::Vector2f(_rect.size - sf::Vector2i(2*border,2*border)));
		rect.setOutlineThickness(border);
		rect.setOutlineColor(sf::Color(63, 63, 63));
		rect.setPosition(sf::Vector2f(_rect.position + sf::Vector2i(border, border)));
		//rect.setFillColor(sf::Color(23,23,23));
		rect.setFillColor(sf::Color::Transparent);
		Main::render_window->draw(rect);

		// title rect
		sf::RectangleShape title_rect(sf::Vector2f(_titleRect.size));
		title_rect.setPosition(sf::Vector2f(_titleRect.position));
		title_rect.setFillColor(sf::Color(31, 31, 31));
		Main::render_window->draw(title_rect);

		// title text
		sf::Color title_text_color = basic_text_color - sf::Color(0,0,0, animation*3);
		_titleText->setFillColor(title_text_color);
		Main::render_window->draw(*_titleText);

		// close button
		_closeButton->draw();

		// content rect
		sf::RectangleShape content_rect(sf::Vector2f(_contentRect.size));
		content_rect.setOutlineThickness(border);
		content_rect.setOutlineColor(sf::Color(63, 63, 63));
		content_rect.setPosition(sf::Vector2f(_contentRect.position));
		content_rect.setFillColor(sf::Color(23, 23, 23));
		Main::render_window->draw(content_rect);
	}

	WindowsManager::WindowsManager() {
		_windows.clear();
	}

	WindowsManager::~WindowsManager() {
		_windows.clear();
	}

	void WindowsManager::push_back(std::shared_ptr<Window> window) {
		_windows.push_back(window);
	}

	void WindowsManager::pop_back() {
		_windows.pop_back();
	}

	std::shared_ptr<Window> WindowsManager::get_back() {

		if (_windows.empty())
			return nullptr;

		return _windows.back();
	}

	void WindowsManager::cursorHover() {
		if (get_back())
			get_back()->cursorHover();
	}

	void WindowsManager::handleEvent(const sf::Event& event) {
		if (get_back())
			get_back()->handleEvent(event);
	}

	void WindowsManager::update() {
		if (get_back())
			get_back()->update();
	}

	void WindowsManager::draw() {
		for(auto& window : _windows)
			window->draw();
	}

	std::shared_ptr<WindowsManager> windows_manager = nullptr;
}
