#pragma once
#include "WindowsManager.hpp"
#include "Windows/FileDialog/FileSystemFunctions.hpp"
#include "Windows/FileDialog/NavBar.hpp"
#include "Windows/FileDialog/FileItem.hpp"
#include "Windows/FileDialog/LocationItem.hpp"
#include "Controls/Scrollbar.hpp"
#include "Controls/TextInput.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"

class FileDialog : public Main::Window {
public:
	static int _padding;
	int _leftRectWidth = 224;
	int _bottomRectHeight = 96;
	int _scrollbarWidth = 24;

	// main rects
	sf::IntRect _topRect;
	sf::IntRect _leftRect;
	sf::IntRect _rightRect;
	sf::IntRect _bottomRect;

	// inner rects
	std::shared_ptr<sf::IntRect> _topInnerRect;
	std::shared_ptr<sf::IntRect> _leftInnerRect;
	std::shared_ptr<sf::IntRect> _rightInnerRect;
	std::shared_ptr<sf::IntRect> _bottomInnerRect;

	// main data
	std::filesystem::path _currentPath;
	std::wstring _acceptableExtension;

	// top rect
	std::shared_ptr<ButtonWithSprite> _prevButton;
	std::shared_ptr<ButtonWithSprite> _nextButton;
	std::shared_ptr<ButtonWithSprite> _upButton;
	std::shared_ptr<NavBar> _navbar;
	
	// right rect
	std::vector<std::filesystem::path> _filesPaths;
	std::vector<std::shared_ptr<FileItem>> _files;

	// left rect
	std::vector<std::filesystem::path> _locationsPaths;
	std::vector<std::shared_ptr<LocationItem>> _locations;

	// scrollbars
	std::shared_ptr<Scrollbar> _leftScrollbar;
	std::shared_ptr<Scrollbar> _rightScrollbar;

	// bottomn rect
	std::unique_ptr<sf::Text> _fileNameText;
	std::shared_ptr<TextInput> _fileNameInput;
	std::shared_ptr<ButtonWithTextAndSprite> _confirmButton;
	std::shared_ptr<ButtonWithTextAndSprite> _cancelButton;

	FileDialog(std::wstring title);
	~FileDialog();

	void createTop();
	void setTheLocationsToNavbar();

	void loadDirectory();
	void createFiles();
	void setTheFilesSize();
	void setTheFiles();

	void createLocations();
	void setTheLocationsSize();
	void setTheLocations();

	void createBottom();

	virtual void setPosition(sf::Vector2i position);

	sf::FloatRect getFilesRect();
	sf::FloatRect getLocationsRect();

	void drawTop();
	void drawFiles();
	void drawLocations();
	void drawBottom();

	

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};