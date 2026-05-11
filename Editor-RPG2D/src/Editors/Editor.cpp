#include "Editors/Editor.hpp"
#include <iostream>

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

EditorManager::EditorManager() {
	_editors.clear();
}

EditorManager::~EditorManager() {
	_editors.clear();
}

void EditorManager::push_back(std::shared_ptr<Editor> editor) {
	_editors.push_back(editor);
}

void EditorManager::pop_back() {
	_editors.pop_back();
}

std::shared_ptr<Editor> EditorManager::get_back() {
	return _editors.back();
}

void EditorManager::cursorHover() {
	if (get_back())
		get_back()->cursorHover();
}

void EditorManager::handleEvent(const sf::Event& event) {
	if(get_back())
		get_back()->handleEvent(event);
}

void EditorManager::update() {
	if(get_back())
		get_back()->update();
}

void EditorManager::draw() {
	if(get_back())
		get_back()->draw();
}

std::shared_ptr<EditorManager> editor_manager = nullptr;