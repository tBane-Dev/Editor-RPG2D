#pragma once
#include "Editors/MapEditor/Map/GameObjectOnMap.hpp"
#include "RenderWindow.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "DebugLog.hpp"
#include "Animator.hpp"
#include "Theme.hpp"
#include "ShadersManager.hpp"

GameObjectOnMap::GameObjectOnMap(std::weak_ptr<GameObject> prefab) : Object() {
	_prefab = prefab;

	if(_prefab.expired()) {
		_type = ObjectType::None;
	}
	else {
		_type = _prefab.lock()->_type;
	}

	float animationInterval = 0.2f;
	if (_prefab.lock()->getName() == L"tree_1") animationInterval = 0.6f;
	if (_prefab.lock()->getName() == L"bush") animationInterval = 0.7f;
	if (_prefab.lock()->getName() == L"reed_1") animationInterval = 0.4f;
	if (_prefab.lock()->getName() == L"reed_2") animationInterval = 0.4f;

	_animator = std::make_shared<Animator>((!_prefab.expired())?prefab.lock()->getAnimations() : std::weak_ptr<Animations>(), animationInterval);


	_animator->setRandFrame();
	_animator->setRandTime();
	_animator->play();
	_position = sf::Vector2i(0, 0);

	_text = std::make_unique<sf::Text>(basicFont, (_prefab.expired()) ? L"Unknown" : _prefab.lock()->getName(), 14);
}

GameObjectOnMap::~GameObjectOnMap() {

}

void GameObjectOnMap::drawFrame() {

	auto prefab = _prefab.lock();
	if (!prefab)
		return;

	int x, y, w, h;

	float thickness = 2.f;
	float paddingLeft = 14.f;
	float textPadding = 6.f;

	sf::Vector2i objectPosition = _position;

	if (prefab->_type == ObjectType::Monster) {
		objectPosition.x -= prefab->getOrigin().x;
		objectPosition.y -= prefab->getOrigin().y;
	}

	auto animations = prefab->getAnimations().lock();

	if (animations) {
		auto frameRect = animations->getFrameRect(0, 0);

		x = objectPosition.x;
		y = objectPosition.y;
		w = frameRect.size.x;
		h = frameRect.size.y;
	}
	else {
		x = objectPosition.x;
		y = objectPosition.y;
		w = 96;
		h = 96;

		std::shared_ptr<Collider> collider = prefab->getCollider();

		if (collider) {
			sf::IntRect rect = collider->getRect();

			x = _position.x + rect.position.x - static_cast<int>(thickness);
			y = _position.y + rect.position.y - static_cast<int>(thickness);
			w = rect.size.x + static_cast<int>(2.f * thickness);
			h = rect.size.y + static_cast<int>(2.f * thickness);
		}
	}

	

	sf::Color color = sf::Color(255, 30, 45);

	_text->setString((_prefab.expired()) ? L"Unknown" : _prefab.lock()->getName());
	_text->setFillColor(color);
	_text->setStyle(sf::Text::Bold);
	_text->setPosition(sf::Vector2f(x + paddingLeft, y - 10));

	auto bounds = _text->getLocalBounds();

	float gapStart = paddingLeft - textPadding;
	float gapEnd = paddingLeft + bounds.size.x + textPadding;

	sf::RectangleShape topLeft(sf::Vector2f(gapStart, thickness));
	topLeft.setPosition(sf::Vector2f(x, y));
	topLeft.setFillColor(color);
	Main::render_window->draw(topLeft);

	if (gapEnd < w) {
		sf::RectangleShape topRight(sf::Vector2f(w - gapEnd, thickness));
		topRight.setPosition(sf::Vector2f(x + gapEnd, y));
		topRight.setFillColor(color);
		Main::render_window->draw(topRight);
	}

	sf::RectangleShape left(sf::Vector2f(thickness, h));
	left.setPosition(sf::Vector2f(x, y));
	left.setFillColor(color);
	Main::render_window->draw(left);

	sf::RectangleShape right(sf::Vector2f(thickness, h));
	right.setPosition(sf::Vector2f(x + w - thickness, y));
	right.setFillColor(color);
	Main::render_window->draw(right);

	sf::RectangleShape bottom(sf::Vector2f(w, thickness));
	bottom.setPosition(sf::Vector2f(x, y + h - thickness));
	bottom.setFillColor(color);
	Main::render_window->draw(bottom);

	Main::render_window->draw(*_text);
	
	
	
	
	
}

sf::Vector2i GameObjectOnMap::getPosition() {
	return _position;
}

void GameObjectOnMap::setPosition(sf::Vector2i position) {
	_position = position;
}

void GameObjectOnMap::cursorHover() {

	if (!_animator)
		return;

	if (_animator->_animations.expired())
		return;

	sf::IntRect rect = _animator->_animations.lock()->getFrameRect(0,0);
	rect.position.x += _position.x;
	rect.position.y += _position.y;

	if (rect.contains(MapEditor::editor->_cursor_on_map->_position)) {
		if (!_prefab.expired()) {
			std::shared_ptr<Mesh> mesh = _prefab.lock()->getMesh();
			if (mesh && mesh->isPointInside(MapEditor::editor->_cursor_on_map->_position, _position)) {
				MapEditor::editor->_game_objects->_hoveredGameObjectOnMap = shared_from_this();
			}
		}
			
	}
}

void GameObjectOnMap::update() {
	_animator->update();
}

void GameObjectOnMap::draw() {

	bool renderAllColliders = MapEditor::editor->_main_menu->_render_colliders->_checkbox->_value == 1;

	sf::Vector2i pos = _position;

	if (_prefab.expired()) return;

	std::shared_ptr<Collider> collider = _prefab.lock()->getCollider();

	if (collider->cursorHover(MapEditor::editor->_cursor_on_map->_position, pos) || renderAllColliders) {
		if (collider->_type == ColliderType::Rectangular) {
			collider->draw(_position);
		}
		else if (collider->_type == ColliderType::Circular) {
			collider->draw(_position + _prefab.lock()->getOrigin());
		}
	}

	if (MapEditor::editor->_main_menu->_render_sprites_outline->_checkbox->_value == 1) {
		drawFrame();
	}

	if(_animator->_animations.expired())
		return;

	std::shared_ptr<Animations> animations = _animator->getAnimations().lock();

	if (animations) {
		sf::IntRect frameRect = animations->getFrameRect(_animator->_animation, _animator->_frame);

		sf::Sprite sprite(*animations->getTexture()->_texture);
		sprite.setPosition(sf::Vector2f(_position));
		sprite.setTextureRect(frameRect);
		if (MapEditor::editor->_game_objects->_hoveredGameObjectOnMap.lock().get() == this)
			sprite.setColor(sf::Color::Red);
		Main::render_window->draw(sprite);
	}

	if(MapEditor::editor->_main_menu->_render_meshes->_checkbox->_value == 1) {
		if (_prefab.lock()->getMesh()) {
			_prefab.lock()->getMesh()->draw(_position, sf::Color::Red);
		}
	}

}