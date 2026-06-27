#pragma once
#include "Editors/MapEditor/Map/GameObjectOnMap.hpp"
#include "RenderWindow.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "DebugLog.hpp"
#include "Animator.hpp"
#include "Theme.hpp"

GameObjectOnMap::GameObjectOnMap(std::weak_ptr<GameObject> prefab) : Object() {
	_prefab = prefab;
	_animator = std::make_shared<Animator>(prefab.lock()->getAnimations(), 0.2f);
	_animator->play();
	_position = sf::Vector2i(0, 0);
}

GameObjectOnMap::~GameObjectOnMap() {

}

void GameObjectOnMap::drawFrame() {

	int x, y, w, h;

	if (!_prefab.lock()->getAnimations().expired()) {
		w = _prefab.lock()->getAnimations().lock()->getFrameRect(0, 0).size.x;
		h = _prefab.lock()->getAnimations().lock()->getFrameRect(0, 0).size.y;
		x = _position.x;
		y = _position.y;

		if (_prefab.lock()->_type == ObjectType::Monster) {
			x -= _prefab.lock()->getOrigin().x;
			y -= _prefab.lock()->getOrigin().y;
		}
	}
	else {

		x = _position.x;
		y = _position.y;
		w = 96;
		h = 96;

		if (_prefab.lock()->getCollider()->_type == ColliderType::Circular) {
			std::shared_ptr<CircularCollider> circularCollider = std::dynamic_pointer_cast<CircularCollider>(_prefab.lock()->getCollider());
			w = circularCollider->_radiusX * 2;
			w += circularCollider->_radiusY;
			if (w < 96) w = 96;
			h = w;
			x = _position.x - w / 2;
			y += circularCollider->_radiusY - h;
		}
		else {
			std::shared_ptr<RectangularCollider> rectangularCollider = std::dynamic_pointer_cast<RectangularCollider>(_prefab.lock()->getCollider());
			w = rectangularCollider->_rect.size.x;
			w += rectangularCollider->_rect.size.y;
			if (w < 96) w = 96;
			h = w;
			x = _position.x - w / 2;
			y += rectangularCollider->_rect.size.y / 2 - h;
		}
	}

	float thickness = 2.f;
	float paddingLeft = 14.f;
	float textPadding = 6.f;

	sf::Color color = sf::Color(255, 30, 45);

	sf::Text text(basicFont, _prefab.lock()->getName(), 14);
	text.setFillColor(color);
	text.setStyle(sf::Text::Bold);
	text.setPosition(sf::Vector2f(x + paddingLeft, y - 10));

	auto bounds = text.getLocalBounds();

	float gapStart = paddingLeft - textPadding;
	float gapEnd = paddingLeft + bounds.size.x + textPadding;

	sf::RectangleShape topLeft(sf::Vector2f(gapStart, thickness));
	topLeft.setPosition(sf::Vector2f(x, y));
	topLeft.setFillColor(color);

	sf::RectangleShape topRight(sf::Vector2f(w - gapEnd, thickness));
	topRight.setPosition(sf::Vector2f(x + gapEnd, y));
	topRight.setFillColor(color);

	sf::RectangleShape left(sf::Vector2f(thickness, h));
	left.setPosition(sf::Vector2f(x, y));
	left.setFillColor(color);

	sf::RectangleShape right(sf::Vector2f(thickness, h));
	right.setPosition(sf::Vector2f(x + w - thickness, y));
	right.setFillColor(color);

	sf::RectangleShape bottom(sf::Vector2f(w, thickness));
	bottom.setPosition(sf::Vector2f(x, y + h - thickness));
	bottom.setFillColor(color);

	Main::render_window->draw(topLeft);
	Main::render_window->draw(topRight);
	Main::render_window->draw(left);
	Main::render_window->draw(right);
	Main::render_window->draw(bottom);
	Main::render_window->draw(text);
}

void GameObjectOnMap::setPosition(sf::Vector2i position) {
	_position = position;
}

void GameObjectOnMap::update() {
	_animator->update();
}

void GameObjectOnMap::draw() {

	bool renderAllColliders = MapEditor::editor->_main_menu->_render_colliders->_checkbox->_value == 1;

	if (renderAllColliders || _prefab.lock()->getCollider()->cursorHover(MapEditor::editor->_cursor_on_map->_position, _position)) {
		_prefab.lock()->getCollider()->draw(_position + _prefab.lock()->getOrigin());
	}

	std::weak_ptr<Animations> animations = _animator->getAnimations();

	if (animations.expired())
		return;

	sf::IntRect frameRect = animations.lock()->getFrameRect(_animator->_animation, _animator->_frame);

	if (MapEditor::editor->_main_menu->_render_sprites_outline->_checkbox->_value == 1) {
		drawFrame();
	}

	sf::Sprite sprite(*animations.lock()->getTexture()->_texture);
	sprite.setPosition(sf::Vector2f(_position));
	sprite.setTextureRect(frameRect);
	Main::render_window->draw(sprite);
}