#include "Windows/FileDialog/LocationItem.hpp"
#include "Windows/FileDialog/FileSystemFunctions.hpp"
#include "Theme.hpp"
#include "Cursor.hpp"
#include "GUIManager.hpp"
#include "RenderWindow.hpp"


LocationItem::LocationItem() : Button() {
	_rect = sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(256, basic_text_rect_height));
	
	_name = L"";
	_path = std::filesystem::path();

	_type = LocationItemType::Empty;
	_opener = nullptr;
	_text = std::make_unique<sf::Text>(basicFont, L"", basic_text_size);
	_text->setFillColor(basic_text_color);

	_onclick_func = {};
}

LocationItem::~LocationItem() {

}

void LocationItem::setSize(sf::Vector2i size) {
	_rect.size = size;
}

void LocationItem::setPosition(sf::Vector2i position) {
	_rect.position = position;

	if (_opener)
		_opener->setPosition(_rect.position);

	_text->setPosition(sf::Vector2f(_rect.position + sf::Vector2i(32+32, 2)));
}

void LocationItem::setFile(std::wstring name, std::filesystem::path path) {
	_name = name;
	_path = path;

	if(isDrive(_path.wstring()))
		_type = LocationItemType::Drive;
	else if(std::filesystem::is_directory(_path))
		_type = LocationItemType::Directory;
	else
		_type = LocationItemType::Empty;

	if (hasChildren(_path)) {
		_opener = std::make_shared<Checkbox>(
			textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\directoryToOpen.png"),
			textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\directoryToOpen_hover.png")
		);

		_opener->addValue(
			textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\directoryToClose.png"),
			textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\directoryToClose_hover.png")
		);

		_opener->setValue(0);

	}
	else {
		_opener = nullptr;
	}

	_text->setString(_name);
	_text->setPosition(sf::Vector2f(_rect.position + sf::Vector2i(32+32, 2)));
}

void LocationItem::cursorHover() {
	if(_path.empty())
		return;

	Button::cursorHover();

	if (_opener != nullptr) {
		_opener->cursorHover();
	}
}

void LocationItem::handleEvent(const sf::Event& event) {
	if(_path.empty())
		return;

	Button::handleEvent(event);

	if (_opener != nullptr) {
		_opener->handleEvent(event);
	}
}

void LocationItem::update() {
	if(_path.empty())
		return;

	Button::update();

	if (_opener != nullptr) {
		_opener->update();
	}
}

void LocationItem::draw() {

	// draw rect
	sf::RectangleShape rect(sf::Vector2f(_rect.size));
	rect.setPosition(sf::Vector2f(_rect.position));
	if(GUI_manager->Element_hovered.get() == this || (_opener && GUI_manager->Element_hovered == _opener)) 
		rect.setFillColor(sf::Color(71, 71, 71));
	else
		rect.setFillColor(sf::Color::Transparent);
	Main::render_window->draw(rect);

	if (_opener) {
		_opener->draw();
	}

	// draw icon
	std::shared_ptr<Texture> icon = nullptr;

	switch (_type) {
	case LocationItemType::Empty:
		icon = nullptr;
		break;
	case LocationItemType::Drive:
		switch (_state) {
			case ButtonState::Pressed:
				icon = textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\harddrive_press.png");
				break;
			case ButtonState::Hover:
				icon = textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\harddrive_hover.png");
				break;
			default:
				icon = textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\harddrive.png");
				break;
		}
		break;
	case LocationItemType::Directory:
		switch (_state) {
			case ButtonState::Pressed:
				icon = textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\directory_press.png");
				break;
			case ButtonState::Hover:
				icon = textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\directory_hover.png");
				break;
			default:
				icon = textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\directory.png");
				break;
		}
		break;
	}

	if (icon) {
		sf::Sprite sprite(*icon->_texture);
		sprite.setPosition(sf::Vector2f(_rect.position + sf::Vector2i(32,0)));
		Main::render_window->draw(sprite);
	}

	Main::render_window->draw(*_text);
}