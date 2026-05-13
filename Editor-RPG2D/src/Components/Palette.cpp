#include "Components/Palette.hpp"
#include "Editors/MapEditor/MapEditor.hpp"
#include "Editors/MapEditor/Objects/Monster.hpp"
#include "PrefabsManager.hpp"

#include "Window.hpp"
#include <Cursor.hpp>
#include "Editors/MapEditor/Map/CursorOnMap.hpp"


Slot::Slot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position) : ButtonWithSprite(texture, hoverTexture, pressTexture, position) {
	_object = nullptr;
	_animator = nullptr;
}

Slot::~Slot() {

}

void Slot::setObject(std::shared_ptr<Object> object) {
	_object = object;

	if(dynamic_pointer_cast<GameObject>(_object) != nullptr) {
		_animator = std::make_shared<Animator>(dynamic_pointer_cast<GameObject>(_object)->getAnimations(), 0.2f);
		_animator->setRandFrame();
		_animator->play();
		return;
	}
	
}

void Slot::update() {

	ButtonWithSprite::update();

	if (_animator != nullptr) {

		_animator->update();

		std::shared_ptr<Animations> animations = _animator->getAnimations();
		sf::IntRect frameRect = animations->getFrameRect(_animator->_animation, _animator->_frame);

		_texture = animations->getTexture();

		_sprite = std::make_shared<sf::Sprite>(*_texture->_texture);
		_sprite->setTextureRect(frameRect);

		float frameWidth = (float)(_texture->getSize().x / animations->_framesCount);
		float frameHeight = (float)(_texture->getSize().y / animations->_animationsCount);

		_sprite->setScale(sf::Vector2f(80.f / frameWidth, 80.f / frameHeight));
		_sprite->setPosition(sf::Vector2f(_rect.position) + sf::Vector2f(40, 40));
		_sprite->setOrigin(sf::Vector2f(frameWidth / 2.f, frameHeight / 2.f));
		return;
	}
}

void Slot::draw() {
	ButtonWithSprite::draw();
	if (_sprite != nullptr) {
		window->draw(*_sprite);
	}
}


Palette::Palette() : Element() {
	sf::Vector2i size;
	size.x = 480 + 32; // 16 - width of scrollbar
	size.y = window->getSize().y - map_editor->_main_menu->getSize().y;
	_rect = sf::IntRect(sf::Vector2i(window->getSize().x - size.x, map_editor->_main_menu->getSize().y), size);

	_slots.clear();
	_slots.reserve(13 * 6);

	for (int y = 0; y < 13; y++) {
		for (int x = 0; x < 6; x++) {

			_slots.emplace_back(
				std::make_shared<Slot>(
					textures_manager->getTexture(L"assets\\tex\\palette\\slot.png"),
					textures_manager->getTexture(L"assets\\tex\\palette\\slot_hover.png"),
					textures_manager->getTexture(L"assets\\tex\\palette\\slot_press.png"),
					sf::Vector2i(_rect.position.x + x * 80, _rect.position.y + y * 80)
				)
			);
		}

	}

	std::vector<std::shared_ptr<GameObject>>& prefabs = prefabs_manager->getAllPrefabs();

	for (auto& slot : _slots) {

		static int i = 0;
		int doubles = 4;
		if (i / doubles < prefabs.size()) {
			slot->setObject(prefabs[i++ / doubles]);
			slot->_onclick_func = [this, slot]() {
				map_editor->_cursor_on_map->_object = slot->_object;
				};
		}
	}

}

Palette::~Palette() {

}


sf::Vector2i Palette::getSize() {
	return _rect.size;
}

void Palette::setPosition(sf::Vector2i position) {


}

void Palette::cursorHover() {
	if (map_editor->_main_menu->_tool_palette->_checkbox->_value != 0)
		return;

	if (_rect.contains(cursor->_position)) {
		GUI_manager->Element_hovered = this->shared_from_this();
	}

	for(auto& slot : _slots) {
		slot->cursorHover();
	}

}

void Palette::handleEvent(const sf::Event& event) {

	if (map_editor->_main_menu->_tool_palette->_checkbox->_value != 0)
		return;

	for (auto& slot : _slots) {
		slot->handleEvent(event);
	}
}

void Palette::update() {
	for(auto& slot : _slots) {
		slot->update();
	}
}

void Palette::draw() {

	if (map_editor->_main_menu->_tool_palette->_checkbox->_value == 0) {
		sf::RectangleShape rect(sf::Vector2f(_rect.size));
		rect.setFillColor(sf::Color(95, 95, 95));
		rect.setPosition(sf::Vector2f(_rect.position));
		window->draw(rect);
	
		for(auto& slot : _slots) {
			slot->draw();
		}
	}


	

}
