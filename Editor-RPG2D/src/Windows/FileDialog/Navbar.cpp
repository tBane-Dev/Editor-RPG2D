#include "Windows/FileDialog/NavBar.hpp" 
#include "Windows/FileDialog/FileSystemFunctions.hpp"
#include "Theme.hpp"
#include "Cursor.hpp"
#include "RenderWindow.hpp"

NavBarItem::NavBarItem(std::filesystem::path path, sf::Vector2i position) : Button() {

	_path = path;

	_rect.position = position;

	// set the text
	std::wstring text = L"";
	if (_path == std::filesystem::path())
		text = L"This PC";
	else{
		if (isDrive(_path)) {
			text = L"Disc (" + _path.wstring();
			text.pop_back();
			text += L")";
		}
		else
			text = _path.filename().wstring();
	}
		
	_text = std::make_unique<sf::Text>(basicFont, text, small_text_size);
	_text->setFillColor(basic_text_color);

	// set the rect size
	int iconWidth = 0;
	if (path == std::filesystem::path())
		iconWidth = 32;

	_rect = sf::IntRect(position + sf::Vector2i(iconWidth, 0), sf::Vector2i(_text->getGlobalBounds().size.x + 8, small_text_rect_height));
	_text->setPosition(sf::Vector2f(_rect.position + sf::Vector2i(4, 2)));

	// set the rect colors
	setRectColors(
		sf::Color::Transparent, sf::Color(75, 75, 75), sf::Color(50, 50, 50),
		sf::Color::Transparent, sf::Color(75, 75, 75), sf::Color(50, 50, 50),
		sf::Color::Transparent

	);

	_arrow = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\arrow.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\arrow_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\arrow_press.png"),
		_rect.position + sf::Vector2i(_rect.size.x, 0)
	);
	
}

NavBarItem::~NavBarItem() {

}

void NavBarItem::setPosition(sf::Vector2i position) {
	int iconWidth = 0;
	if (_path == std::filesystem::path())
		iconWidth = 32;
	_rect.position = position + sf::Vector2i(iconWidth, 0);;
	_text->setPosition(sf::Vector2f(_rect.position + sf::Vector2i(4, 2)));
	_arrow->setPosition(_rect.position + sf::Vector2i(_rect.size.x, 0));
}

sf::Vector2i NavBarItem::getSize() {
	int minX = std::min(_rect.position.x, _arrow->getPosition().x);
	int minY = std::min(_rect.position.y, _arrow->getPosition().y);
	int maxX = std::max(_rect.position.x + _rect.size.x, _arrow->getPosition().x + _arrow->getSize().x);
	int maxY = std::max(_rect.position.y + _rect.size.y, _arrow->getPosition().y + _arrow->getSize().y);

	if (_path == std::filesystem::path()) {
		minX -= 32;
	}
		
	return sf::Vector2i(maxX - minX, maxY - minY);
}

void NavBarItem::cursorHover() {
	Button::cursorHover();
	_arrow->cursorHover();
}

void NavBarItem::handleEvent(const sf::Event& event) {
	Button::handleEvent(event);
	_arrow->handleEvent(event);
}

void NavBarItem::update() {
	Button::update();
	_arrow->update();
}

void NavBarItem::draw() {

	if(_path == std::filesystem::path()) {
		sf::Sprite icon(*textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\navbar_pc.png")->_texture);
		icon.setPosition(sf::Vector2f(_rect.position - sf::Vector2i(32, 4)));
		Main::render_window->draw(icon);
	}

	Button::draw();
	Main::render_window->draw(*_text);

	_arrow->draw();

}

NavBar::NavBar(sf::Vector2i position, sf::Vector2i size) {
	_rect = sf::IntRect(position, size);

	
}

NavBar::~NavBar() {

}

void NavBar::clear() {
	_items.clear();
}

void NavBar::add(std::filesystem::path path, std::function<void()> onclick_func) {
	int posX = 0;

	for (auto& item : _items) {
		posX += item->getSize().x;
	}

	sf::Vector2i position = _rect.position + sf::Vector2i(posX, (_rect.size.y - small_text_rect_height) / 2);

	std::shared_ptr<NavBarItem> item = std::make_shared<NavBarItem>(path, position);
	item->_onclick_func = onclick_func;

	_items.push_back(item);
}

void NavBar::pop_back() {
	if (!_items.empty())
		_items.pop_back();
}

void NavBar::setPosition(sf::Vector2i position) {
	_rect.position = position;

	int posX = 0;
	for (auto& item : _items) {
		item->setPosition(_rect.position + sf::Vector2i(posX, (_rect.size.y - small_text_rect_height) / 2));
		posX += item->getSize().x;
	}
}

void NavBar::cursorHover() {

	for (auto& item : _items) {
		item->cursorHover();
	}
}

void NavBar::handleEvent(const sf::Event& event) {

	for (auto& item : _items) {
		item->handleEvent(event);
	}

}

void NavBar::update() {

	for (auto& item : _items) {
		if (item->_state == ButtonState::Pressed) {
			item->update();
			break;
		}
		item->update();
		
	}
}

void NavBar::draw() {
	int border = 1;
	sf::RectangleShape rect(sf::Vector2f(_rect.size - sf::Vector2i(border * 2, border * 2)));
	rect.setPosition(sf::Vector2f(_rect.position + sf::Vector2i(border, border)));
	rect.setFillColor(sf::Color(15,15,15));
	Main::render_window->draw(rect);

	for(auto& item : _items) {
		item->draw();
	}
}