#include "Objects/Monster.hpp"
#include "Time.hpp"
#include "RenderWindow.hpp"
#include <iostream>
#include "Editors/MapEditor/Editor.hpp"

MonsterPrefab::MonsterPrefab(std::wstring name, std::shared_ptr<Animations> animations, sf::Vector2i origin, int stepSize, std::shared_ptr<Collider> collider) : GameObject(name, animations, origin, collider) {
	_type = ObjectType::Monster;
	_stepSize = stepSize;
}

MonsterPrefab::~MonsterPrefab() {

}

Monster::Monster(std::shared_ptr<GameObject> prefab) : GameObjectOnMap(prefab) {

	_type = ObjectType::Monster;

	_state = MonsterState::Moving;

	_basePosition = sf::Vector2i(0, 0);
	_direction = Direction::Down;

	_animator->setRandFrame();

	_path = std::make_shared<Path>();
	_path->setStartPoint(_position);
	_path->setEndPoint(_position);
	_path->generatePath();
}

Monster::~Monster() {

}

void Monster::setPosition(sf::Vector2i position) {
	GameObjectOnMap::setPosition(position);

	_basePosition = position;

	_path->setStartPoint(_position);
	_path->setEndPoint(_position);
	_path->generatePath();
}

void Monster::update() {
	
	if (_state == MonsterState::Moving) {
		_animator->_timer += deltaTime.asSeconds();

		if (_animator->_timer >= _animator->_interval) {

			_animator->_timer = 0.0f;

			if (_path->isEmpty() || _position == _path->getEndPoint()) {
				_path->setStartPoint(_position);
				sf::Vector2i endPoint = _basePosition;
				int r = 8;
				endPoint.x += (rand() % (2 * r + 1) - r) * Tile::tileSize;
				endPoint.y += (rand() % (2 * r + 1) - r) * Tile::tileSize;
				_path->setEndPoint(endPoint);
				_path->generatePath();

				_state = MonsterState::Moving;
				return;
			}

			if (_position != _path->getEndPoint()) {

				sf::Vector2i target = _path->getStartPoint();
				int stepSize = std::dynamic_pointer_cast<MonsterPrefab>(_prefab)->_stepSize;

				if (_position.y < target.y) {
					_position.y += std::min(target.y - _position.y, stepSize);
					_direction = Direction::Down;
				}
				else if (_position.y > target.y) {
					_position.y -= std::min(_position.y - target.y, stepSize);
					_direction = Direction::Up;
				}
				else if (_position.x < target.x) {
					_position.x += std::min(target.x - _position.x, stepSize);
					_direction = Direction::Right;
				}
				else if (_position.x > target.x) {
					_position.x -= std::min(_position.x - target.x, stepSize);
					_direction = Direction::Left;
				}

				if (_position == target) {
					_path->nextPoint();
				}

			}

			if ((int)_direction != _animator->_animation)
				_animator->setAnimation((int)_direction);

			_animator->nextFrame();

		}
	}
	
}

void Monster::draw() {

	if (MapEditor::editor->_main_menu->_render_paths->_checkbox->_value == 1) {
		_path->draw();
	}
		

	bool renderAllColliders = MapEditor::editor->_main_menu->_render_colliders->_checkbox->_value == 1;

	if (renderAllColliders || _prefab->_collider->cursorHover(MapEditor::editor->_cursor_on_map->_position + _prefab->getOrigin(), _position)) {
		_prefab->getCollider()->draw(_position);
	}
	

	std::shared_ptr<Animations> anim = _animator->getAnimations();
	
	if (!anim) {
		drawFrame();
		return;
	}

	sf::IntRect frameRect = anim->getFrameRect(_animator->_animation, _animator->_frame);

	if (MapEditor::editor->_main_menu->_render_sprites_outline->_checkbox->_value == 1) {
		drawFrame();
	}

	sf::Sprite sprite(*anim->getTexture()->_texture);
	sprite.setPosition(sf::Vector2f(_position));
	sprite.setOrigin(sf::Vector2f(_prefab->getOrigin()));
	sprite.setTextureRect(frameRect);
	Main::render_window->draw(sprite);
}