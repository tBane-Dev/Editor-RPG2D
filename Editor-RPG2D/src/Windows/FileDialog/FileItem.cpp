#include "Windows/FileDialog/FileItem.hpp"
#include "Windows/FileDialog/FileSystemFunctions.hpp"
#include "Theme.hpp"
#include "Cursor.hpp"
#include "GUIManager.hpp"
#include "RenderWindow.hpp"


FileItem::FileItem() : Button() {
	_rect = sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(256, basic_text_rect_height));
	
	_name = L"";
	_path = std::filesystem::path();

	_type = FileItemType::Empty;
	_text = std::make_unique<sf::Text>(basicFont, L"", basic_text_size);
	_text->setFillColor(basic_text_color);

	_rectIdleColor = sf::Color::Transparent;
	_rectHoverColor = sf::Color(71, 71, 71);
	_rectPressColor = sf::Color(71, 71, 71);

	_onclick_func = {};
}

FileItem::~FileItem() {

}

void FileItem::setSize(sf::Vector2i size) {
	_rect.size = size;
}

void FileItem::setPosition(sf::Vector2i position) {
	_rect.position = position;
	_text->setPosition(sf::Vector2f(position + sf::Vector2i(32, 2)));
}

void FileItem::setFile(std::wstring name, std::filesystem::path path) {
	_name = name;
	_path = path;

	if(isDrive(_path.wstring()))
		_type = FileItemType::Drive;
	else if(std::filesystem::is_directory(_path))
		_type = FileItemType::Directory;
	else if(_path != std::filesystem::path())
		_type = FileItemType::File;
	else
		_type = FileItemType::Empty;

	_text->setString(name);
	_text->setPosition(sf::Vector2f(_rect.position + sf::Vector2i(32, 2)));
}

void FileItem::cursorHover() {
	if(_path.empty())
		return;

	Button::cursorHover();
}

void FileItem::draw() {

	Button::draw();

	// draw icon
	std::shared_ptr<Texture> icon = nullptr;
	switch (_type) {
	case FileItemType::Empty:
		icon = nullptr;
		break;
	case FileItemType::Drive:
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
	case FileItemType::Directory:
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
	case FileItemType::File:
		switch (_state) {
			case ButtonState::Pressed:
				icon = textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\file_press.png");
				break;
			case ButtonState::Hover:
				icon = textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\file_hover.png");
				break;
			default:
				icon = textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\file.png");
				break;
		}
		break;
	}

	if (icon) {
		sf::Sprite sprite(*icon->_texture);
		sprite.setPosition(sf::Vector2f(_rect.position));
		Main::render_window->draw(sprite);
	}

	Main::render_window->draw(*_text);
}