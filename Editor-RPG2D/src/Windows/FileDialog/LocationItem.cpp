#include "Windows/FileDialog/LocationItem.hpp"
#include "Windows/FileDialog/FileSystemFunctions.hpp"
#include "Theme.hpp"
#include "Cursor.hpp"
#include "GUIManager.hpp"
#include "RenderWindow.hpp"

LocationItem::LocationItem() : Button() {
	_rect = sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(256, basic_text_rect_height));
	
	_name = L"";
	_location = nullptr;

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

	sf::Vector2i pos = _rect.position;

	if(_location)
		pos.x += 16 * _location->_depth;

	if (_opener)
		_opener->setPosition(pos);

	_text->setPosition(sf::Vector2f(pos + sf::Vector2i(64, 2)));
}

void LocationItem::setFile(std::wstring name, std::shared_ptr<Location> location) {

	_name = name;
	_location = location;

	if (!_location)
		_type = LocationItemType::Empty;
	else if (*_location->_path == std::filesystem::path())
		_type = LocationItemType::PC;
	else if(isDrive(_location->_path->wstring()))
		_type = LocationItemType::Drive;
	else if(std::filesystem::is_directory(*_location->_path))
		_type = LocationItemType::Directory;
	else
		_type = LocationItemType::Empty;


	if (_location && (*_location->_path.get() == std::filesystem::path() || hasChildren(*_location->_path))) { // std::filesystem::path() (This PC) or directory with children
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
	if(!_location)
		return;

	Button::cursorHover();

	if (_opener != nullptr) {
		_opener->cursorHover();
	}
}

void LocationItem::handleEvent(const sf::Event& event) {
	if(!_location)
		return;

	Button::handleEvent(event);

	if (_opener != nullptr) {
		_opener->handleEvent(event);
	}
}

void LocationItem::update() {
	if(!_location)
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

	case LocationItemType::PC:
		switch (_state) {
		case ButtonState::Pressed:
			icon = textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\pc_press.png");
			break;
		case ButtonState::Hover:
			icon = textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\pc_hover.png");
			break;
		default:
			icon = textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\pc.png");
			break;
		}
		break;
	}


	if (icon) {
		sf::Sprite sprite(*icon->_texture);
		sf::Vector2i pos = _rect.position + sf::Vector2i(32, 0);
		if (_location) pos.x += 16 * _location->_depth;
		sprite.setPosition(sf::Vector2f(pos));
		Main::render_window->draw(sprite);
	}

	Main::render_window->draw(*_text);
}