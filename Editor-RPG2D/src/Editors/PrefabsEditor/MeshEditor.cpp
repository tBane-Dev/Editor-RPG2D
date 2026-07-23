#include "Editors/PrefabsEditor/MeshEditor.hpp"
#include "ShadersManager.hpp"
#include "Cursor.hpp"
#include "Editors/PrefabsEditor/Editor.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"

MeshEditor::MeshEditor() : Main::Window(L"Mesh Editor", sf::Vector2i(1120, 920), sf::Vector2i(256, 64)) {

	margin = 32;
	_innerRect = sf::IntRect(sf::Vector2i(getContentPosition() + sf::Vector2i(margin, margin)), sf::Vector2i(getContentSize() - sf::Vector2i(2 * margin, 2 * margin)));
	_canvasRect = sf::IntRect(sf::Vector2i(_innerRect.position + sf::Vector2i((_innerRect.size.x - 768) / 2, (_innerRect.size.y - 768) / 2)), sf::Vector2i(768, 768));

	_tileSize = 8;

	// basic
	_animations = PrefabsEditor::editor->_animator->_animations.lock();
	_spriteScale = std::min((float)_canvasRect.size.x / (float)_animations->getFrameRect(0,0).size.x, (float)_canvasRect.size.y / (float)_animations->getFrameRect(0,0).size.y);
	_spriteOutlineThickness = 6.0f;

	_spriteRect = sf::IntRect(sf::Vector2i(_canvasRect.position + sf::Vector2i((_canvasRect.size.x - _animations->getFrameRect(0,0).size.x * _spriteScale) / 2, (_canvasRect.size.y - _animations->getFrameRect(0,0).size.y * _spriteScale) / 2)), sf::Vector2i(_animations->getFrameRect(0,0).size.x * _spriteScale, _animations->getFrameRect(0,0).size.y * _spriteScale));

	if (PrefabsEditor::editor->_mesh == nullptr)
		PrefabsEditor::editor->_mesh = std::make_shared<Mesh>(2.0f, 1.0f);

	PrefabsEditor::editor->_mesh->setRadius(2.0f * _spriteScale);
	PrefabsEditor::editor->_mesh->setScale(_spriteScale);

	_activeShape = PrefabsEditor::editor->_mesh->getLastShape();

}

MeshEditor::~MeshEditor() {
	PrefabsEditor::editor->_main_panel->setButtonsActivity();
	PrefabsEditor::editor->_main_panel->setTooltips();
}

void MeshEditor::setPosition(sf::Vector2i position) {
	Main::Window::setPosition(position);

	_innerRect.position = sf::Vector2i(getContentPosition().x + margin, getContentPosition().y + margin);
	_canvasRect.position = sf::Vector2i(_innerRect.position.x + (_innerRect.size.x - 768) / 2, _innerRect.position.y + (_innerRect.size.y - 768) / 2);
	_spriteRect.position = sf::Vector2i(_canvasRect.position.x + (_canvasRect.size.x - _animations->getFrameRect(0,0).size.x * _spriteScale) / 2, _canvasRect.position.y + (_canvasRect.size.y - _animations->getFrameRect(0,0).size.y * _spriteScale) / 2);
}

void MeshEditor::cursorHover() {
	Main::Window::cursorHover();
}

void MeshEditor::handleEvent(const sf::Event& event) {
	Main::Window::handleEvent(event);

	_cursorPosition = sf::Vector2i(-1, -1);

	// draw point at cursor position if inside canvas
	int cursorMargin = 8;
	sf::IntRect rectForCursor = sf::IntRect(_spriteRect.position - sf::Vector2i(cursorMargin, cursorMargin), _spriteRect.size + sf::Vector2i(2 * cursorMargin, 2 * cursorMargin));

	if (rectForCursor.contains(Cursors::cursor->_position)) {
		int radius = 8;
		_cursorPosition.x = (float)_spriteRect.position.x + std::round((Cursors::cursor->_position.x - _spriteRect.position.x) / (_tileSize * _spriteScale)) * _tileSize * _spriteScale;
		_cursorPosition.y = (float)_spriteRect.position.y + std::round((Cursors::cursor->_position.y - _spriteRect.position.y) / (_tileSize * _spriteScale)) * _tileSize * _spriteScale;

		sf::Vector2i point;
		point.x = std::round((_cursorPosition.x - _spriteRect.position.x) / _spriteScale);
		point.y = std::round((_cursorPosition.y - _spriteRect.position.y) / _spriteScale);


		if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {

			if (PrefabsEditor::editor->_mesh->_shapes.empty()) {
				PrefabsEditor::editor->_mesh->addShape(std::make_shared<Shape>());
				_activeShape = PrefabsEditor::editor->_mesh->getShape(0);
			}

			for (int i = PrefabsEditor::editor->_mesh->_shapes.size() - 1; i >= 0; i -= 1) {

				std::shared_ptr<Shape > shape = PrefabsEditor::editor->_mesh->getShape(i);


				if (shape->hasPoint(point) && shape->insidePoint(point, PrefabsEditor::editor->_mesh->_radius, PrefabsEditor::editor->_mesh->_scale) >= 0) {
					_activeShape = shape;
					return;
				}
			}

			for (int i = PrefabsEditor::editor->_mesh->_shapes.size() - 1; i >= 0; i -= 1) {
				std::shared_ptr<Shape > shape = PrefabsEditor::editor->_mesh->getShape(i);
				if (shape->hasPoint(point) && shape->_points.size() >= 3 && shape->pointInShape(point)) {
					_activeShape = shape;
					return;
				}
			}

			if (!_activeShape.expired()) {
				_activeShape.lock()->addPoint(point, -1, _spriteScale);
			}
			else {
				std::shared_ptr<Shape> shape = std::make_shared<Shape>();
				shape->addPoint(point, -1, _spriteScale);
				PrefabsEditor::editor->_mesh->addShape(shape);
				_activeShape = shape;
			}

		}

		if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Right) {

			std::shared_ptr<Mesh> mesh = PrefabsEditor::editor->_mesh;

			if (!mesh) return;

			for (int i = static_cast<int>(mesh->_shapes.size()) - 1; i >= 0; --i) {
				std::shared_ptr<Shape> shape = mesh->getShape(i);

				if (!shape) continue;

				int pointIndex = shape->insidePoint(point, mesh->_radius, mesh->_scale);
				if (pointIndex < 0) continue;

				std::shared_ptr<Shape> activeShape = _activeShape.lock();

				if (activeShape != shape) {
					_activeShape = shape;
					return;
				}

				shape->removePoint(shape->_points[pointIndex]);

				if (shape->_points.empty()) {
					mesh->removeShape(shape);
					_activeShape = std::weak_ptr<Shape>();
				}

				return;
			}

			_activeShape = std::weak_ptr<Shape>();
		}
	}

}

void MeshEditor::update() {
	Main::Window::update();
}

void MeshEditor::draw() {
	GUI_manager->setView();
	Main::Window::draw();

	sf::RectangleShape innerRectShape(sf::Vector2f(_innerRect.size));
	innerRectShape.setPosition(sf::Vector2f(_innerRect.position));
	innerRectShape.setFillColor(sf::Color(31, 31, 31));
	innerRectShape.setOutlineThickness(1.0f);
	innerRectShape.setOutlineColor(sf::Color(63, 63, 63));
	Main::render_window->draw(innerRectShape);

	// canvas background
	sf::RectangleShape canvasRectShape(sf::Vector2f(_canvasRect.size));
	canvasRectShape.setFillColor(sf::Color(47, 47, 47));
	canvasRectShape.setPosition(sf::Vector2f(_canvasRect.position));
	Main::render_window->draw(canvasRectShape);


	// draw checkerboard
	sf::RectangleShape rect(sf::Vector2f(_spriteRect.size));
	rect.setFillColor(sf::Color::Red);
	rect.setPosition(sf::Vector2f(_spriteRect.position));

	sf::RenderStates states;
	states.shader = &*checkerboard_shader;
	checkerboard_shader->setUniform("rectPos", sf::Vector2f(_spriteRect.position));
	Main::render_window->draw(rect, states);

	// sprite
	sf::Sprite sprite(*_animations->getTexture()->_texture);
	sprite.setTextureRect(_animations->getFrameRect(0, 0));
	sprite.setScale(sf::Vector2f(_spriteScale, _spriteScale));
	sprite.setPosition(sf::Vector2f(_canvasRect.position + sf::Vector2i((_canvasRect.size.x - _animations->getFrameRect(0,0).size.x * _spriteScale) / 2, (_canvasRect.size.y - _animations->getFrameRect(0,0).size.y * _spriteScale) / 2)));
	Main::render_window->draw(sprite);

	// draw grid
	float gridWidth = 2.0f;
	grid2_shader->setUniform("rectPosition", sf::Vector2f(_canvasRect.position));
	grid2_shader->setUniform("rectSize", sf::Vector2f(_canvasRect.size));
	grid2_shader->setUniform("gridSize", sf::Vector2f(_tileSize*_spriteScale, _tileSize *_spriteScale));
	grid2_shader->setUniform("chunkSize", sf::Vector2f(_tileSize*16.f*_spriteScale, _tileSize*16.f*_spriteScale));
	grid2_shader->setUniform("gridWidth", gridWidth);
	grid2_shader->setUniform("mainGridMultiplier", gridWidth * 2.0f);
	grid2_shader->setUniform("gridMainColor", sf::Glsl::Vec4(sf::Color(127, 7, 7, 255)));
	grid2_shader->setUniform("gridSecondColor", sf::Glsl::Vec4(sf::Color(127, 47, 47, 255)));

	sf::RenderStates rs;
	rs.shader = &*grid2_shader;

	sf::RectangleShape gridRect(sf::Vector2f(_canvasRect.size));
	gridRect.setPosition(sf::Vector2f(_canvasRect.position));
	gridRect.setFillColor(sf::Color(127, 47, 47, 127));
	Main::render_window->draw(gridRect, rs);

	// sprite outline 
	sf::RectangleShape spriteOutline(sf::Vector2f(_spriteRect.size - sf::Vector2i(2 * (int)_spriteOutlineThickness - gridWidth, 2 * (int)_spriteOutlineThickness - gridWidth)));
	spriteOutline.setPosition(sf::Vector2f(_spriteRect.position + sf::Vector2i((int)_spriteOutlineThickness - gridWidth / 2, (int)_spriteOutlineThickness - gridWidth / 2)));
	spriteOutline.setFillColor(sf::Color::Transparent);
	spriteOutline.setOutlineThickness(_spriteOutlineThickness);
	spriteOutline.setOutlineColor(sf::Color(23, 23, 23));
	Main::render_window->draw(spriteOutline);

	if (PrefabsEditor::editor->_mesh)
		PrefabsEditor::editor->_mesh->draw(_spriteRect.position, _spriteScale, sf::Color(255, 0, 0), true, _activeShape.lock());

	// draw point at cursor position if inside canvas
	int cursorMargin = 8;
	sf::IntRect rectForCursor = sf::IntRect(_spriteRect.position - sf::Vector2i(cursorMargin, cursorMargin), _spriteRect.size + sf::Vector2i(2 * cursorMargin, 2 * cursorMargin));

	if (_cursorPosition != sf::Vector2i(-1,-1)) {
		
		int radius = 2.f * _spriteScale;
		if (radius > 16.f)
			radius = 16.f;

		sf::CircleShape point(radius);
		point.setFillColor(sf::Color(255, 0, 255));
		point.setOrigin(sf::Vector2f(radius, radius));

		sf::Vector2f centerPoint = sf::Vector2f(_cursorPosition); // because outer margin is 4, and we want to keep center the point inside grid
		if (centerPoint.x < _canvasRect.position.x + 2) centerPoint.x = (float)_canvasRect.position.x + 2;
		if (centerPoint.y < _canvasRect.position.y + 2) centerPoint.y = (float)_canvasRect.position.y + 2;
		if (centerPoint.x > _canvasRect.position.x + _canvasRect.size.x - 2) centerPoint.x = (float)_canvasRect.position.x + _canvasRect.size.x - 2;
		if (centerPoint.y > _canvasRect.position.y + _canvasRect.size.y - 2) centerPoint.y = (float)_canvasRect.position.y + _canvasRect.size.y - 2;

		point.setPosition(centerPoint);
		Main::render_window->draw(point);
	}
}