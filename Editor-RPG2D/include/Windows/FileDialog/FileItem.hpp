#pragma once
#include <SFML/Graphics.hpp>
#include "GUIManager.hpp"
#include <functional>
#include "Controls/Button.hpp"

enum class FileItemType { Empty, Drive, Directory, File };

class FileItem : public Button {
public:
	std::wstring _name;
	std::filesystem::path _path;

	FileItemType _type;
	std::unique_ptr<sf::Text> _text;

	FileItem();
	~FileItem();

	void setSize(sf::Vector2i size);
	void setPosition(sf::Vector2i position);
	void setFile(std::wstring name, std::filesystem::path path);

	virtual void cursorHover();
	virtual void draw();
};