#include "Editors/PrefabsEditor/MeshEditor.hpp"
#include "ShadersManager.hpp"
#include "Cursor.hpp"
#include "Editors/PrefabsEditor/Editor.hpp"
#include "RenderWindow.hpp"

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
}

MeshEditor::~MeshEditor() {

}

void MeshEditor::setPosition(sf::Vector2i position) {
	Main::Window::setPosition(position);

	_innerRect.position = sf::Vector2i(getContentPosition().x + margin, getContentPosition().y + margin);
	_canvasRect.position = sf::Vector2i(_innerRect.position.x + (_innerRect.size.x - 768) / 2, _innerRect.position.y + (_innerRect.size.y - 768) / 2);
	_spriteRect.position = sf::Vector2i(_canvasRect.position.x + (_canvasRect.size.x - _animations->getFrameRect(0,0).size.x * _spriteScale) / 2, _canvasRect.position.y + (_canvasRect.size.y - _animations->getFrameRect(0,0).size.y * _spriteScale) / 2);
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

	// draw point at cursor position if inside canvas
	int cursorMargin = 8;
	sf::IntRect rectForCursor = sf::IntRect(_spriteRect.position - sf::Vector2i(cursorMargin, cursorMargin), _spriteRect.size + sf::Vector2i(2 * cursorMargin, 2 * cursorMargin));

	if (rectForCursor.contains(Main::cursor->_position)) {
		int radius = 8;
		sf::CircleShape point(radius);
		point.setFillColor(sf::Color(127, 47, 127));
		point.setOrigin(sf::Vector2f(radius, radius));

		sf::Vector2f position;
		position.x = (float)_canvasRect.position.x + std::round((Main::cursor->_position.x - _canvasRect.position.x) / (_tileSize * _spriteScale)) * _tileSize * _spriteScale;
		position.y = (float)_canvasRect.position.y + std::round((Main::cursor->_position.y - _canvasRect.position.y) / (_tileSize * _spriteScale)) * _tileSize * _spriteScale;
		
		sf::Vector2f centerPoint = position; // because outer margin is 4, and we want to keep center the point inside grid
		if (centerPoint.x < _canvasRect.position.x + 2) centerPoint.x = (float)_canvasRect.position.x + 2;
		if (centerPoint.y < _canvasRect.position.y + 2) centerPoint.y = (float)_canvasRect.position.y + 2;
		if (centerPoint.x > _canvasRect.position.x + _canvasRect.size.x - 2) centerPoint.x = (float)_canvasRect.position.x + _canvasRect.size.x - 2;
		if (centerPoint.y > _canvasRect.position.y + _canvasRect.size.y - 2) centerPoint.y = (float)_canvasRect.position.y + _canvasRect.size.y - 2;

		point.setPosition(centerPoint);
		Main::render_window->draw(point);
	}
}