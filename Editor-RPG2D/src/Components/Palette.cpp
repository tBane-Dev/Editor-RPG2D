#include "Components/Palette.hpp"
#include "Editors/MapEditor/MapEditor.hpp"
#include "Editors/MapEditor/Objects/Monster.hpp"
#include "PrefabsManager.hpp"

#include "Window.hpp"
#include <Cursor.hpp>
#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "DebugLog.hpp"



Slot::Slot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position) : ButtonWithSprite(texture, hoverTexture, pressTexture, position) {
	_object = nullptr;
	_animator = nullptr;
}

Slot::~Slot() {

}

void Slot::setObject(std::shared_ptr<Object> object) {
	
	_object = object;
	
	_animator = nullptr;
	_texture = nullptr;
	_sprite = nullptr;

	if(_object != nullptr && dynamic_pointer_cast<GameObject>(_object) != nullptr) {
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

GroupButton::GroupButton(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, ObjectType group, std::shared_ptr<Texture> groupTexture) : ButtonWithSprite(texture, hoverTexture, pressTexture) {
	_group = group;
	_groupTexture = groupTexture;
}

GroupButton::~GroupButton() {

}

void GroupButton::cursorHover() {
	if (_group == ObjectType::None)
		return;

	ButtonWithSprite::cursorHover();
}

void GroupButton::handleEvent(const sf::Event& event) {
	
	if (_group == ObjectType::None)
		return;
	
	ButtonWithSprite::handleEvent(event);
}

void GroupButton::draw() {
	ButtonWithSprite::draw();

	if (_groupTexture) {
		sf::Sprite sprite(*_groupTexture->_texture);
		sprite.setPosition(sf::Vector2f(_rect.position));
		window->draw(sprite);
	}
	
}

Palette::Palette() : Element() {
	sf::Vector2i size;
	size.x = 480 + 32; // 16 - width of scrollbar
	size.y = window->getSize().y - map_editor->_main_menu->getSize().y;
	_rect = sf::IntRect(sf::Vector2i(window->getSize().x - size.x, map_editor->_main_menu->getSize().y), size);

	_prev = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\palette\\prev.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\prev_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\prev_hover.png"),
		sf::Vector2i(_rect.position.x, _rect.position.y)
	);

	_next = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\palette\\next.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\next_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\next_hover.png"),
		sf::Vector2i(_rect.position.x + size.x - 56, _rect.position.y)
	);

	{
		std::shared_ptr<GroupButton> groupButton = std::make_shared<GroupButton>(
			textures_manager->getTexture(L"assets\\tex\\palette\\group.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\group_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\group_hover.png"),
			ObjectType::Terrain,
			textures_manager->getTexture(L"assets\\tex\\palette\\terrain.png")
		);
		_groups.push_back(groupButton);
	}

	{
		std::shared_ptr<GroupButton> groupButton = std::make_shared<GroupButton>(
			textures_manager->getTexture(L"assets\\tex\\palette\\group.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\group_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\group_hover.png"),
			ObjectType::Nature,
			textures_manager->getTexture(L"assets\\tex\\palette\\natures.png")
		);
		_groups.push_back(groupButton);
	}

	{
		std::shared_ptr<GroupButton> groupButton = std::make_shared<GroupButton>(
			textures_manager->getTexture(L"assets\\tex\\palette\\group.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\group_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\group_hover.png"),
			ObjectType::Monster,
			textures_manager->getTexture(L"assets\\tex\\palette\\monsters.png")
		);
		_groups.push_back(groupButton);
	}

	for (int i = 0; i < 7; i++) {
		std::shared_ptr<GroupButton> groupButton = std::make_shared<GroupButton>(
			textures_manager->getTexture(L"assets\\tex\\palette\\group.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\group_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\group_hover.png"),
			ObjectType::None,
			nullptr
		);
		_groups.push_back(groupButton);
	}
	
	for (int i = 0; i < _groups.size(); i+=1) {
		std::shared_ptr<GroupButton>&group = _groups[i];
		group->_onclick_func = [this, group]() { setGroup(group); };

		sf::Vector2i position;
		position.x = _rect.position.x + 56 + (i % 5) * 80;
		position.y = _rect.position.y + (i / 5) * 52;
		group->setPosition(position);
	}
	

	_slots.clear();
	_slots.reserve(13 * 6);

	for (int y = 0; y < 13; y++) {
		for (int x = 0; x < 6; x++) {

			_slots.emplace_back(
				std::make_shared<Slot>(
					textures_manager->getTexture(L"assets\\tex\\palette\\slot.png"),
					textures_manager->getTexture(L"assets\\tex\\palette\\slot_hover.png"),
					textures_manager->getTexture(L"assets\\tex\\palette\\slot_press.png"),
					sf::Vector2i(_rect.position.x + x * 80, _rect.position.y + 104 + y * 80)
				)
			);
		}

	}

	setGroup(_groups[2]);

}

Palette::~Palette() {

}


sf::Vector2i Palette::getSize() {
	return _rect.size;
}

void Palette::setGroup(std::shared_ptr<GroupButton> group) {

	if (group == nullptr)
		return;

	if (group->_group == ObjectType::None)
		return;

	if (_currentGroup!= nullptr && _currentGroup->_group == group->_group)
		return;

	// deactive old group
	if (_currentGroup != nullptr) {
		_currentGroup->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\group.png");
		_currentGroup->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\group_hover.png");
		_currentGroup->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\group_hover.png");
	}

	// set new group
	_currentGroup = group;

	_currentGroup->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\group_selected.png");
	_currentGroup->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\group_selected_hover.png");
	_currentGroup->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\group_selected_hover.png");

	for(int i = 0; i < _slots.size(); i++) {
		_slots[i]->setObject(nullptr);
	}

	std::vector<std::shared_ptr<GameObject>> allPrefabs = prefabs_manager->getAllPrefabs();

	int doubles = 12;
	int id = 0;
	for (int i = 0; i < allPrefabs.size(); i++) {
		
		if(group->_group == allPrefabs[i]->_type) {

			for (int d = 0; d < doubles; d++) {
				std::shared_ptr<Slot> slot = _slots[id];
				slot->setObject(allPrefabs[i]);
				slot->_onclick_func = [this, slot]() {
					map_editor->_cursor_on_map->_object = slot->_object;
					};

				id++;
			}

		}
	}
}


void Palette::setPosition(sf::Vector2i position) {


}

void Palette::cursorHover() {
	if (map_editor->_main_menu->_tool_palette->_checkbox->_value != 0)
		return;

	if (_rect.contains(cursor->_position)) {
		GUI_manager->Element_hovered = this->shared_from_this();
	}

	_next->cursorHover();
	_prev->cursorHover();

	for(auto& group : _groups) {
		group->cursorHover();
	}

	for(auto& slot : _slots) {
		slot->cursorHover();
	}

}

void Palette::handleEvent(const sf::Event& event) {

	if (map_editor->_main_menu->_tool_palette->_checkbox->_value != 0)
		return;

	_next->handleEvent(event);
	_prev->handleEvent(event);

	for(auto& group : _groups) {
		group->handleEvent(event);
	}

	for (auto& slot : _slots) {
		slot->handleEvent(event);
	}
}

void Palette::update() {

	_next->update();
	_prev->update();

	for(auto& group : _groups) {
		group->update();
	}

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
	
		_prev->draw();
		_next->draw();

		for(auto& group : _groups) {
			group->draw();
		}

		for(auto& slot : _slots) {
			slot->draw();
		}
	}


	

}
