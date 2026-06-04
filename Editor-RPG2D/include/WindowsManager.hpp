#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "GUIManager.hpp"
#include "Controls/ButtonWithSprite.hpp"

namespace Main {

	enum class WindowState { Idle, Moving };

	class Window : public Element {
	public:

		static int border;

		std::wstring _titleStr;
		sf::IntRect _rect;

		sf::IntRect _titleRect;
		std::unique_ptr<sf::Text> _titleText;
		std::shared_ptr<ButtonWithSprite> _closeButton;

		sf::IntRect _contentRect;

		WindowState _state;
		sf::Vector2i _offset;	// to moving the window

		Window(std::wstring title);
		~Window();

		virtual void init();
		virtual void setPosition(sf::Vector2i position);
		virtual void cursorHover();
		virtual void handleEvent(const sf::Event& event);
		virtual void update();
		virtual void draw();
	};

	class WindowsManager {
	public:
		std::vector<std::shared_ptr<Window>> _windows;

		WindowsManager();
		~WindowsManager();

		void push_back(std::shared_ptr<Window> window);
		void pop_back();
		std::shared_ptr<Window> get_back();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();
	};

	extern std::shared_ptr<WindowsManager> windows_manager;
}