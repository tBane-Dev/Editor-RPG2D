#include "Editors/PrefabsEditor/ColliderPanel.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "Editors/PrefabsEditor/PrefabsEditor.hpp"
#include "ShaderManager.hpp"
#include "Theme.hpp"
#include "Window.hpp"

ColliderPanel::ColliderPanel(sf::Vector2i margin) : Panel(sf::Vector2i(420, 475), sf::Vector2i(840 + margin.x, prefabs_editor->_main_menu->getSize().y + margin.y)) {
	_title = std::make_unique<sf::Text>(basicFont, L"Collider", 20);
	_title->setFillColor(basic_text_color);
	_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));

	// text inputs
	_type = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"type", 24, 18);
	_type->setPosition(sf::Vector2i(_rect.position.x + 84, _rect.position.y + margin.y + 192 + 16));
	_type->setText(L"");

	_x = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"x", 24, 18);
	_x->setPosition(sf::Vector2i(_rect.position.x + 84, _type->getPosition().y + _type->getSize().y + 8));
	_x->setText(L"95");

	_y = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"y", 24, 18);
	_y->setPosition(sf::Vector2i(_rect.position.x + 84, _x->getPosition().y + _x->getSize().y + 8));
	_y->setText(L"145");

	_w = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"width", 24, 18);
	_w->setPosition(sf::Vector2i(_rect.position.x + 84, _y->getPosition().y + _y->getSize().y + 8));
	_w->setText(L"100");

	_h = std::make_shared<TextInput>(sf::Vector2i(256, 30), L"height", 24, 18);
	_h->setPosition(sf::Vector2i(_rect.position.x + 84, _w->getPosition().y + _w->getSize().y + 8));
	_h->setText(L"70");

	// texts labels
	int x = _x->getPosition().x - 32;

	_typeLabel = std::make_unique<sf::Text>(basicFont, L"t", 18);
	_typeLabel->setFillColor(basic_text_color);
	_typeLabel->setPosition(sf::Vector2f(x - _typeLabel->getGlobalBounds().size.x / 2, _type->getPosition().y));

	_xLabel = std::make_unique<sf::Text>(basicFont, L"x", 18);
	_xLabel->setFillColor(basic_text_color);
	_xLabel->setPosition(sf::Vector2f(x	- _xLabel->getGlobalBounds().size.x/2, _x->getPosition().y));

	_yLabel = std::make_unique<sf::Text>(basicFont, L"y", 18);
	_yLabel->setFillColor(basic_text_color);
	_yLabel->setPosition(sf::Vector2f(x - _yLabel->getGlobalBounds().size.x/2, _y->getPosition().y));

	_wLabel = std::make_unique<sf::Text>(basicFont, L"w", 18);
	_wLabel->setFillColor(basic_text_color);
	_wLabel->setPosition(sf::Vector2f(x - _wLabel->getGlobalBounds().size.x/2, _w->getPosition().y));

	_hLabel = std::make_unique<sf::Text>(basicFont, L"h", 18);
	_hLabel->setFillColor(basic_text_color);
	_hLabel->setPosition(sf::Vector2f(x - _hLabel->getGlobalBounds().size.x/2, _h->getPosition().y));
}

ColliderPanel::~ColliderPanel() {

}

void ColliderPanel::cursorHover() {
	Panel::cursorHover();

	_type->cursorHover();
	_x->cursorHover();
	_y->cursorHover();
	_w->cursorHover();
	_h->cursorHover();
}

void ColliderPanel::handleEvent(const sf::Event& event) {
	Panel::handleEvent(event);

	_type->handleEvent(event);
	_x->handleEvent(event);
	_y->handleEvent(event);
	_w->handleEvent(event);
	_h->handleEvent(event);
}

void ColliderPanel::update() {
	Panel::update();

	_type->update();
	_x->update();
	_y->update();
	_w->update();
	_h->update();
}

void ColliderPanel::draw() {
	Panel::draw();
	window->draw(*_title);

	// draw checkerboard
	sf::RectangleShape rect(sf::Vector2f(sf::Vector2f(192, 192)));
	rect.setFillColor(sf::Color::Red);
	rect.setPosition(sf::Vector2f(getPosition().x + (getSize().x - 192) / 2, getPosition().y + _title->getFont().getLineSpacing(20) + 24 + 16));

	sf::RenderStates states;
	states.shader = &*checkerboard_shader;

	checkerboard_shader->setUniform("rectPos", rect.getPosition());

	window->draw(rect, states);

	// draw animation
	std::shared_ptr<Animator>& animator = prefabs_editor->_animator;
	if (animator) {
		
		sf::CircleShape circle(std::stoi(_w->getText())/2);
		circle.setFillColor(sf::Color(255, 0, 0, 128));
		circle.setScale(sf::Vector2f(1, (float)std::stoi(_h->getText()) / (float)std::stoi(_w->getText())));
		circle.setOrigin(sf::Vector2f(std::stoi(_w->getText()) / 2, std::stoi(_w->getText()) / 2));
		circle.setPosition(sf::Vector2f(rect.getPosition().x + std::stoi(_x->getText()), rect.getPosition().y + std::stoi(_y->getText())));	
		window->draw(circle);

		std::shared_ptr<Animations> animations = animator->getAnimations();
		sf::IntRect frameRect = animations->getFrameRect(0, 0);

		sf::Sprite sprite(*animations->getTexture()->_texture);
		sprite.setTextureRect(frameRect);
		sprite.setScale(sf::Vector2f(rect.getSize().x / (float)frameRect.size.x, rect.getSize().y / (float)frameRect.size.y));
		sprite.setPosition(sf::Vector2f(rect.getPosition().x, rect.getPosition().y));
		window->draw(sprite);
	}

	// texts labels
	window->draw(*_typeLabel);
	window->draw(*_xLabel);
	window->draw(*_yLabel);
	window->draw(*_wLabel);
	window->draw(*_hLabel);

	// text inputs
	_type->draw();
	_x->draw();
	_y->draw();
	_w->draw();
	_h->draw();
	
}