#include "Objects/Monster.hpp"
#include "Time.hpp"
#include "RenderWindow.hpp"
#include <iostream>
#include "Editors/MapEditor/Editor.hpp"
#include "DebugLog.hpp"

MonsterPrefab::MonsterPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, int stepSize, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh) : GameObject(name, animations, origin, collider, mesh) {
	_type = ObjectType::Monster;
	_stepSize = stepSize;
}

MonsterPrefab::~MonsterPrefab() {

}

Monster::Monster(std::weak_ptr<GameObject> prefab) : GameObjectOnMap(prefab) {

	_type = ObjectType::Monster;

	_state = MonsterState::Moving;

	_basePosition = sf::Vector2i(0, 0);
	_direction = Direction::Down;

	_animator->setRandFrame();
	_animator->_interval = 0.125f; // monsters are fasters than static animated objects

	_path = std::make_shared<Path>();
	_path->setStartPoint(_position);
	_path->setEndPoint(_position);
	_path->generatePath();
}

Monster::~Monster() {

}

sf::Vector2i Monster::getPosition() {
	return _basePosition;
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
				int stepSize = std::dynamic_pointer_cast<MonsterPrefab>(_prefab.lock())->_stepSize;

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

	sf::IntRect colliderRect;
	if (_prefab.lock()->getCollider()->_type == ColliderType::Rectangular) {
		std::shared_ptr<RectangularCollider> c = std::dynamic_pointer_cast<RectangularCollider>(_prefab.lock()->getCollider());
		colliderRect = c->_rect;
		colliderRect.position += _position;
	}
	else {
		std::shared_ptr<CircularCollider> c = std::dynamic_pointer_cast<CircularCollider>(_prefab.lock()->getCollider());
		colliderRect = sf::IntRect(sf::Vector2i(_position.x - c->_radiusX, _position.y - c->_radiusY), sf::Vector2i(2 * c->_radiusX, 2 * c->_radiusY));
	}

	if (MapEditor::editor->_camera->_visibleRect.findIntersection(colliderRect)) {
		std::shared_ptr<Chunk> chunk = MapEditor::editor->_map->getChunkByGlobalPosition(_position);
		chunk->_isVisible = true;
	}
	
}

void Monster::draw() {

	if (_prefab.expired())
		return;

	if (MapEditor::editor->_main_menu->_render_paths->_checkbox->_value == 1) {
		_path->draw();
	}

	bool renderAllColliders = MapEditor::editor->_main_menu->_render_colliders->_checkbox->_value == 1;

	std::shared_ptr<Collider> collider = _prefab.lock()->getCollider();

	if (collider != nullptr && (renderAllColliders || collider->cursorHover(MapEditor::editor->_cursor_on_map->_position + _prefab.lock()->getOrigin(), _position))) {
		_prefab.lock()->getCollider()->draw(_position);
	}
	

	std::shared_ptr<Animations> anim = _animator->getAnimations().lock();
	
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
	sprite.setOrigin(sf::Vector2f(_prefab.lock()->getOrigin()));
	sprite.setTextureRect(frameRect);
	Main::render_window->draw(sprite);
}