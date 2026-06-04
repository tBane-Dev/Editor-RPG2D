#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

namespace Main {
	class Editor {
	public:

		Editor();
		~Editor();

		virtual void init();
		virtual void cursorHover();
		virtual void handleEvent(const sf::Event& event);
		virtual void update();
		virtual void draw();
	};

	class EditorsManager {
	public:
		std::vector<std::shared_ptr<Editor>> _editors;

		EditorsManager();
		~EditorsManager();

		void push_back(std::shared_ptr<Editor> editor);
		void pop_back();
		std::shared_ptr<Editor> get_back();

		void cursorHover();
		void handleEvent(const sf::Event& event);
		void update();
		void draw();
	};

	extern std::shared_ptr<EditorsManager> editor_manager;
}

