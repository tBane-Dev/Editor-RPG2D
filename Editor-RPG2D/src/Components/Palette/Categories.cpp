#include "Components/Palette/Categories.hpp"
#include "Window.hpp"
#include "Theme.hpp"
#include "DebugLog.hpp"

CategoryButton::CategoryButton(ObjectType type, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> categoryTexture, sf::Vector2i position) : ButtonWithSprite(texture, hoverTexture, pressTexture, position) {
	_type = type;
	_categoryTexture = categoryTexture;
}

CategoryButton::~CategoryButton() {

}

void CategoryButton::cursorHover() {
	if (_categoryTexture)
		ButtonWithSprite::cursorHover();
}

void CategoryButton::handleEvent(const sf::Event& event) {
	if (_categoryTexture)
		ButtonWithSprite::handleEvent(event);
}

void CategoryButton::update() {
	ButtonWithSprite::update();
}

void CategoryButton::draw() {

	ButtonWithSprite::draw();

	if (_categoryTexture) {
		sf::Sprite sprite(*_categoryTexture->_texture);
		sprite.setPosition(sf::Vector2f(_rect.position));
		window->draw(sprite);
	}
}

Categories::Categories() {

	_main_margin = 16;
	_top_margin = 22;
	_outer_margin = 16;
	
	_text = std::make_unique<sf::Text>(basicFont, L"Categories", 20);
	_text->setFillColor(basic_text_color);

	_prev = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\prev.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\prev_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\prev_press.png")
	);

	_next = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\next.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\next_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\categories\\next_press.png")
	);

	_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - _prev->getSize().x - _next->getSize().x - 5 * 64) / 8;
	_rect.size = sf::Vector2i(600 - 2 * _main_margin, 64 + 2 * _main_margin + _top_margin + _inner_margin);

	_selectedCategory = nullptr;
	_selectedType = ObjectType::None;
}

Categories::~Categories() {

}

sf::Vector2i Categories::getPosition() {
	return _rect.position;
}

sf::Vector2i Categories::getSize() {
	return _rect.size;
}

void Categories::addCategory(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> categoryTexture, ObjectType type, std::function<void()> function) {
	std::shared_ptr<CategoryButton> categoryButton = std::make_shared<CategoryButton>(type, texture, hoverTexture, pressTexture, categoryTexture);
	categoryButton->_onclick_func = function;
	_categories.push_back(categoryButton);
}

std::shared_ptr<CategoryButton> Categories::getCategory(ObjectType type) {
	for (auto& category : _categories) {
		if(category->_type == type)
			return category;
	}
	return nullptr;
}

void Categories::setCategory(ObjectType type) {

	if (_selectedCategory) {
		_selectedCategory->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png");
		_selectedCategory->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png");
		_selectedCategory->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png");
	}

	_selectedType = type;
	
	for (auto& category : _categories) {
		category->setSelect(category->_type == type);
	}
	
	_selectedCategory = getCategory(type);

	_selectedCategory->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\categories\\selected.png");
	_selectedCategory->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\categories\\selected_hover.png");
	_selectedCategory->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\categories\\selected_press.png");
}

void Categories::setPosition(sf::Vector2i position) {

	_rect.position = position + sf::Vector2i(_main_margin, _main_margin);
	
	int x = _rect.position.x + _outer_margin;
	int y = _rect.position.y + _outer_margin;
	
	_text->setPosition(sf::Vector2f(x, y));

	y += _top_margin + _inner_margin;

	_prev->setPosition(sf::Vector2i(x, y));
	
	x += _prev->getSize().x + _inner_margin;

	for (auto& category : _categories) {
		category->setPosition(sf::Vector2i(x, y));
		x += category->_rect.size.x + _inner_margin;
	}

	_next->setPosition(sf::Vector2i(x, y));

}

void Categories::cursorHover() {

	_prev->cursorHover();
	_next->cursorHover();

	for (auto& category : _categories) {
		category->cursorHover();
	}
}

void Categories::handleEvent(const sf::Event& event) {

	_prev->handleEvent(event);
	_next->handleEvent(event);

	for (auto& category : _categories) {
		category->handleEvent(event);
	}
}

void Categories::update() {

	_prev->update();
	_next->update();

	for (auto& category : _categories) {
		category->update();
	}
}

void Categories::draw() {

	int border = 2;
	sf::RectangleShape background(sf::Vector2f(_rect.size.x - 2 * border, _rect.size.y - 2 * border));
	background.setPosition(sf::Vector2f(_rect.position.x + border, _rect.position.y + border));
	background.setFillColor(sf::Color(31, 31, 31));
	background.setOutlineThickness(2);
	background.setOutlineColor(sf::Color(63, 63, 63));
	window->draw(background);

	window->draw(*_text);

	_prev->draw();
	_next->draw();

	for (auto& category : _categories) {
		category->draw();
	}
}

