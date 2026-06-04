#include "EditorsManager.hpp"
#include <iostream>

namespace Main {

	Editor::Editor() {

	}

	Editor::~Editor() {

	}

	void Editor::init() {

	}

	void Editor::cursorHover() {

	}

	void Editor::handleEvent(const sf::Event& event) {

	}

	void Editor::update() {

	}

	void Editor::draw() {

	}

	EditorsManager::EditorsManager() {
		_editors.clear();
	}

	EditorsManager::~EditorsManager() {
		_editors.clear();
	}

	void EditorsManager::push_back(std::shared_ptr<Editor> editor) {
		_editors.push_back(editor);
	}

	void EditorsManager::pop_back() {
		_editors.pop_back();
	}

	std::shared_ptr<Editor> EditorsManager::get_back() {
		return _editors.back();
	}

	void EditorsManager::cursorHover() {
		if (get_back())
			get_back()->cursorHover();
	}

	void EditorsManager::handleEvent(const sf::Event& event) {
		if (get_back())
			get_back()->handleEvent(event);
	}

	void EditorsManager::update() {
		if (get_back())
			get_back()->update();
	}

	void EditorsManager::draw() {
		if (get_back())
			get_back()->draw();
	}

	std::shared_ptr<EditorsManager> editor_manager = nullptr;
}