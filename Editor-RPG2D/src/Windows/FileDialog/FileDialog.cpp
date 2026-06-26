#include "Windows/FileDialog/FileDialog.hpp"
#include "Theme.hpp"
#include "Cursor.hpp"
#include "RenderWindow.hpp"
#include "Windows/ConfirmDialog.hpp" // TO-DO - to delete


int FileDialog::_padding = 8;

FileDialog::FileDialog(std::wstring title, std::function<void()> function) : Main::Window(title, sf::Vector2i(640, 440), sf::Vector2i(100, 100)) {
	
	_leftRectWidth = 224;
	_bottomRectHeight = 96+16+8;
	_scrollbarWidth = 24;

	// main rects
	_topRect = sf::IntRect(_contentRect.position, sf::Vector2i(_contentRect.size.x, 40));
	_leftRect = sf::IntRect(_contentRect.position + sf::Vector2i(0, _topRect.size.y), sf::Vector2i(_leftRectWidth, _contentRect.size.y - _topRect.size.y - _bottomRectHeight));
	_rightRect = sf::IntRect(_leftRect.position + sf::Vector2i(_leftRect.size.x - _padding, 0), sf::Vector2i(_contentRect.size.x - _leftRectWidth + _padding, _leftRect.size.y));
	_bottomRect = sf::IntRect(_contentRect.position + sf::Vector2i(0, _contentRect.size.y - _bottomRectHeight), sf::Vector2i(_contentRect.size.x, _bottomRectHeight));

	// inner rects
	_topInnerRect = std::make_shared<sf::IntRect>(_topRect.position + sf::Vector2i(_padding, 8), _topRect.size - sf::Vector2i(_padding * 2, 8));
	_leftInnerRect = std::make_shared<sf::IntRect>(_leftRect.position + sf::Vector2i(_padding, _padding), _leftRect.size - sf::Vector2i(_padding * 2 + _scrollbarWidth, _padding * 2));
	_rightInnerRect = std::make_shared<sf::IntRect>(_rightRect.position + sf::Vector2i(_padding, _padding), _rightRect.size - sf::Vector2i(_padding * 2 + _scrollbarWidth, _padding * 2));
	_bottomInnerRect = std::make_shared<sf::IntRect>(_bottomRect.position + sf::Vector2i(_padding, 0), _bottomRect.size - sf::Vector2i(_padding * 2, _padding));
	
	// current path = desktop
	const wchar_t* userProfile = _wgetenv(L"USERPROFILE");

	if (userProfile) {
		_currentPath = userProfile;
		_currentPath += L"\\Desktop";
	}
	else {
		_currentPath = L"C:\\";
	}

	createTop();

	createFiles();
	loadDirectory();
	setTheFilesSize();
	setTheFiles();

	createLocations();

	createVisibleLocations();
	setTheVisibleLocationsSize();
	setTheVisibleLocations();
	createBottom(function);

	_leftScrollbar = std::make_shared<Scrollbar>(
		_leftInnerRect->position.x + _leftInnerRect->size.x + 4,
		_leftInnerRect->position.y,
		_scrollbarWidth,
		_leftInnerRect->size.y,
		0,
		std::max(0, (int)_visibleLocations.size() * basic_text_rect_height - _leftInnerRect->size.y),
		std::ceil((float)_leftInnerRect->size.y / (float)basic_text_rect_height) * basic_text_rect_height,
		0
	);
	_leftScrollbar->_func = [this]() { setTheVisibleLocations(); };
	_leftScrollbar->setScrollArea(_leftInnerRect, (float)basic_text_rect_height*0.25f);

	_rightScrollbar = std::make_shared<Scrollbar>(
		_rightInnerRect->position.x + _rightInnerRect->size.x + 4,
		_rightInnerRect->position.y,
		_scrollbarWidth,
		_rightInnerRect->size.y,
		0,
		std::max(0, (int)_filesPaths.size() * basic_text_rect_height - _rightInnerRect->size.y),
		std::ceil((float)_rightInnerRect->size.y / (float)basic_text_rect_height) * basic_text_rect_height,
		0
	);
	_rightScrollbar->_func = [this]() { setTheFiles(); };
	_rightScrollbar->setScrollArea(_rightInnerRect, (float)basic_text_rect_height * 0.25f);

	
}

FileDialog::~FileDialog() {

}

void FileDialog::createTop() {

	int marginTop = (_topInnerRect->size.y - 32) / 2;
	int marginLeft = 4;

	_prevButton = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\back.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\back_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\back_press.png"),
		_topInnerRect->position + sf::Vector2i(marginLeft, marginTop)
	);

	_nextButton = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\next.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\next_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\next_press.png"),
		_topInnerRect->position + sf::Vector2i(32, 0) + sf::Vector2i(marginLeft, marginTop)
	);

	_upButton = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\up.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\up_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\up_press.png"),
		_topInnerRect->position + sf::Vector2i(64, 0) + sf::Vector2i(marginLeft, marginTop)
	);

	int navbarX = _upButton->getPosition().x + _upButton->getSize().x + marginLeft;
	_navbar = std::make_shared<NavBar>(sf::Vector2i(navbarX, _topInnerRect->position.y + marginTop), sf::Vector2i(_topInnerRect->position.x + _topInnerRect->size.x - navbarX, 32));

	setTheLocationsToNavbar();
}

void FileDialog::setTheLocationsToNavbar() {
	_navbar->clear();

	std::list<std::filesystem::path> locations;
	std::filesystem::path p = _currentPath;

	
	locations.clear();
	// full path to the current directory
	//while (p != std::filesystem::path()) {
	//	locations.insert(locations.begin(), p);
	//	
	//	if (isDrive(p))
	//		p = std::filesystem::path();
	//	else
	//		p = p.parent_path();
	//}
	//

	// only desktop
	locations.push_back(std::filesystem::path());
	locations.push_back(_currentPath); 
	//

	int posX = 0;
	for (auto& loc : locations) {
		std::function<void()> onclick_func = [this, loc]() {

			if (_currentPath != loc) {
				_currentPath = loc;
				while (!_navbar->_items.empty() && _navbar->_items.back()->_path != loc) {
					_navbar->pop_back();
				}
				loadDirectory();

				_rightScrollbar->setValue(0);
				_rightScrollbar->setMax(std::max(0, (int)_filesPaths.size() * basic_text_rect_height - _rightInnerRect->size.y));
				setTheFilesSize();
				setTheFiles();
			}
		};

		_navbar->add(loc, onclick_func);
	}
}

void FileDialog::loadDirectory() {
	//std::wcout << L"Loading directory: " << currentPath << std::endl;
	_filesPaths.clear();

	std::error_code ec;
	auto opts = std::filesystem::directory_options::skip_permission_denied;

	for (std::filesystem::directory_iterator it(_currentPath, opts, ec), end;
		it != end; it.increment(ec)) {
		if (ec) { ec.clear(); continue; }

		std::filesystem::path p = getPath(it->path());
		if (p.empty()) continue; // nieudany .lnk

		auto name = p.filename().wstring();
		if (name.empty() || onlyWhitespace(name) || name == L"." || name == L"..")
			continue;

		if (!(std::filesystem::is_directory(p) || _acceptableExtension.empty() || isExtension(name, _acceptableExtension)))
			continue;

		_filesPaths.push_back(p);
		//std::wcout << p.wstring() << std::endl;
	}

	std::sort(_filesPaths.begin(), _filesPaths.end(), sortkey);

	//std::wcout << L"Total files: " << _filesPaths.size() << std::endl;
		
}

void FileDialog::createFiles() {
	_files.clear();

	int countFiles = std::ceil((float)_rightInnerRect->size.y / (float)basic_text_rect_height) + 1;

	for (int i = 0; i < countFiles; i++) {
		std::shared_ptr<FileItem> file = std::make_shared<FileItem>();
		file->setFile(L"File " + std::to_wstring(i), std::filesystem::path());
		file->setPosition(_rightInnerRect->position + sf::Vector2i(0, i * basic_text_rect_height - ((_rightScrollbar) ? _rightScrollbar->getValue() % basic_text_rect_height : 0)));
		_files.push_back(file);
	}
}

void FileDialog::setTheFilesSize() {
	int scrollbarValue = (_rightScrollbar) ? _rightScrollbar->getValue() : 0;
	int startIndex = scrollbarValue / basic_text_rect_height;
	for (int i = 0; i < _files.size(); i++) {
		if (i + startIndex < _filesPaths.size()) {
			_files[i]->setSize(sf::Vector2i(_rightInnerRect->size.x, basic_text_rect_height));
		}
		else {
			_files[i]->setSize(sf::Vector2i(0, 0));
		}
	}
}

void FileDialog::setTheFiles() {

	int scrollbarValue = (_rightScrollbar) ? _rightScrollbar->getValue() : 0;
	int startIndex = scrollbarValue / basic_text_rect_height;

	for (int i = 0; i < _files.size(); i++) {
		if (i + startIndex < _filesPaths.size()) {
			std::wstring filename = _filesPaths[i + startIndex].filename().wstring();
			_files[i]->setFile(filename, _filesPaths[i+startIndex]);
			_files[i]->_onclick_func = [this, i]() {
				if (_files[i]->_type == FileItemType::Drive || _files[i]->_type == FileItemType::Directory) {
					_currentPath = _files[i]->_path;
					loadDirectory();

					std::filesystem::path navbarPath = _currentPath;
					_navbar->add(navbarPath, [this, navbarPath]() {
						_currentPath = navbarPath;
						while (!_navbar->_items.empty() && _navbar->_items.back()->_path != navbarPath) {
							_navbar->pop_back();
						}
						loadDirectory();
						_rightScrollbar->setValue(0);
						_rightScrollbar->setMax(std::max(0, (int)_filesPaths.size() * basic_text_rect_height - _rightInnerRect->size.y));
						setTheFilesSize();
						setTheFiles();
						}
					);
					_rightScrollbar->setValue(0);
					_rightScrollbar->setMax(std::max(0, (int)_filesPaths.size() * basic_text_rect_height - _rightInnerRect->size.y));
					setTheFilesSize();
					setTheFiles();
				}
				else {
					_fileNameInput->setText(_files[i]->_path.filename().wstring());
				}
				
				};
		}
		else {
			_files[i]->setFile(L"", std::filesystem::path());
			_files[i]->_onclick_func = nullptr;
		}

		sf::Vector2i pos = _rightInnerRect->position + sf::Vector2i(0, i * basic_text_rect_height);
		if(_rightScrollbar)
			pos -= sf::Vector2i(0, scrollbarValue%basic_text_rect_height);
		_files[i]->setPosition(pos);

		
	}

}

void FileDialog::addVisibleLocation(std::shared_ptr<Location> location) {
	if (!location)
		return;

	auto locationItem = std::make_shared<LocationItem>();

	std::wstring name;

	if (!location->_path) {
		// Separator
		locationItem->setFile(L"", nullptr);
	}
	else if (location->_path->empty()) {
		name = L"This PC";
		locationItem->setFile(name, location);
	}
	else if (isDrive(*location->_path)) {
		name = L"Disc (" + location->_path->wstring();
		name.pop_back(); // remove the trailing backslash
		name += L")";

		locationItem->setFile(name, location);
	}
	else {
		name = location->_path->filename().wstring();
		locationItem->setFile(name, location);
	}

	_visibleLocations.push_back(locationItem);

	if (location->_isOpen) {
		for (const auto& child : location->_children) {
			addVisibleLocation(child);
		}
	}
}

void FileDialog::createLocations() {
	std::vector<std::wstring> fav_paths;

	// favorite locations
	const wchar_t* userProfile = _wgetenv(L"USERPROFILE");
	std::wstring up(userProfile);
	fav_paths.push_back(up + L"\\AppData\\Roaming\\Microsoft\\Windows\\Recent");
	fav_paths.push_back(up + L"\\Documents");
	fav_paths.push_back(up + L"\\Music");
	fav_paths.push_back(up + L"\\Pictures");
	fav_paths.push_back(up + L"\\Downloads");
	fav_paths.push_back(up + L"\\Desktop");
	fav_paths.push_back(up + L"\\Videos");

	std::vector<std::wstring> drives_paths;
	// load the harddrivers
	DWORD drives = GetLogicalDrives();
	for (int i = 0; i < 32; i++)
		if ((drives >> i) & 1) {
			//printf("%c:\\\n", 'A' + i);
			drives_paths.push_back(std::wstring(1, L'A' + i) + L":\\");
		}

	_locationsPaths.clear();
	for (auto& path : fav_paths) {
		_locationsPaths.push_back(std::make_shared<std::filesystem::path>(std::filesystem::path(path)));
	}

	_locationsPaths.push_back(nullptr); // separator
	_locationsPaths.push_back(std::make_shared<std::filesystem::path>(std::filesystem::path())); // This PC

	// create the locations
	_locations.clear();
	for (auto& loc : _locationsPaths) {
		std::shared_ptr<Location> l = std::make_shared<Location>(loc, 0);
		_locations.push_back(l);
	}

	for (auto& d : drives_paths) {
		std::shared_ptr<Location> driveLocation = std::make_shared<Location>(std::make_shared<std::filesystem::path>(std::filesystem::path(d)), 1);
		_locations.back()->_children.push_back(driveLocation);
	}

	_locations.back()->_hasChildren = true;
	_locations.back()->open();
}

void FileDialog::createVisibleLocations() {

	_visibleLocations.clear();

	for (auto& location : _locations) {
		addVisibleLocation(location);
	}
}

void FileDialog::setTheVisibleLocationsSize() {
	for(auto& location : _visibleLocations) {
		location->setSize(sf::Vector2i(_leftInnerRect->size.x, basic_text_rect_height));
	}
}

void FileDialog::setTheVisibleLocations() {

	int scrollbarValue =
		_leftScrollbar ? _leftScrollbar->getValue() : 0;

	for (int i = 0; i < _visibleLocations.size(); i++) {

		auto locationItem = _visibleLocations[i];

		if (!locationItem)
			continue;

		auto location = locationItem->_location;

		locationItem->setPosition(
			_leftInnerRect->position +
			sf::Vector2i(
				0,
				i * basic_text_rect_height - scrollbarValue
			)
		);

		locationItem->setSize(
			sf::Vector2i(
				_leftInnerRect->size.x,
				basic_text_rect_height
			)
		);

		if (!location || !location->_path) {
			locationItem->_onclick_func = nullptr;
			continue;
		}

		std::filesystem::path path = *location->_path;

		locationItem->_onclick_func = [this, path]() {

			if (path.empty() || _currentPath == path)
				return;

			_currentPath = path;
			loadDirectory();

			_navbar->clear();

			auto addNavbarItem =
				[this](const std::filesystem::path& navbarPath) {

				_navbar->add(
					navbarPath,
					[this, navbarPath]() {

						if (_currentPath == navbarPath)
							return;

						_currentPath = navbarPath;

						while (
							!_navbar->_items.empty() &&
							_navbar->_items.back()->_path != navbarPath
							) {
							_navbar->pop_back();
						}

						loadDirectory();

						_rightScrollbar->setValue(0);
						_rightScrollbar->setMax(
							std::max(
								0,
								static_cast<int>(_filesPaths.size()) *
								basic_text_rect_height -
								_rightInnerRect->size.y
							)
						);

						setTheFilesSize();
						setTheFiles();
					}
				);
				};

			addNavbarItem(std::filesystem::path());
			addNavbarItem(path);

			_rightScrollbar->setValue(0);
			_rightScrollbar->setMax(
				std::max(
					0,
					static_cast<int>(_filesPaths.size()) *
					basic_text_rect_height -
					_rightInnerRect->size.y
				)
			);

			setTheFilesSize();
			setTheFiles();
			};

		if (locationItem->_opener) {

			locationItem->_opener->_onclick_func =
				[this, location]() {

				if (location->_isOpen)
					location->close();
				else
					location->open();

				createVisibleLocations();
				setTheVisibleLocationsSize();
				
				int newMax = std::max(0, (int)(_visibleLocations.size()) * basic_text_rect_height - _leftInnerRect->size.y);
				int scrollbarValue = std::min(_leftScrollbar->getValue(), newMax);

				_leftScrollbar->setMax(newMax);
				_leftScrollbar->setValue(scrollbarValue);
				_leftScrollbar->updateSliderSize();

				setTheVisibleLocations();

			};
		}
	}
}

void FileDialog::createBottom(std::function<void()> function) {
	_fileNameText = std::make_unique<sf::Text>(basicFont, L"Filename", basic_text_size);
	_fileNameText->setFillColor(basic_text_color);
	_fileNameText->setPosition(sf::Vector2f(_bottomInnerRect->position + sf::Vector2i(32, 16+4)));

	sf::Vector2i textInputSize;
	textInputSize.x = _bottomInnerRect->size.x - 32 - _fileNameText->getGlobalBounds().size.x - 32;
	textInputSize.y = 32;

	_fileNameInput = std::make_shared<TextInput>(textInputSize, L"", -1, basic_text_size);
	_fileNameInput->setColors(sf::Color(71, 71, 71), sf::Color(95, 95, 95), sf::Color(71, 71, 71), sf::Color(63,63,63));
	_fileNameInput->setPosition(_bottomInnerRect->position + sf::Vector2i(32 + _fileNameText->getGlobalBounds().size.x + 16, 16));

	sf::Vector2i position = _fileNameInput->getPosition() + sf::Vector2i(_fileNameInput->getSize().x - 2*128, 40);

	_confirmButton = std::make_shared<ButtonWithTextAndSprite>(
		L"select",
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\button.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\button_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\button_press.png"),
		position
	);

	_cancelButton = std::make_shared<ButtonWithTextAndSprite>(
		L"cancel",
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\button.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\button_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\button_press.png"),
		position + sf::Vector2i(128, 0)
	);

	_confirmButton->_onclick_func = [this, function]() {

		std::wstring filename = _fileNameInput->getText();

		if (filename != L"") {
			_closeButton->_onclick_func();

			if(function)
				function();
		}
	};
		

	_cancelButton->_onclick_func = [this]() {
		_closeButton->_onclick_func();
	};
}

void FileDialog::setFunction(std::function<void()> function) {
	_confirmButton->_onclick_func = [this, function]() {
		std::wstring filename = _fileNameInput->getText();
		if (filename != L"") {
			_closeButton->_onclick_func();

			if (function)
				function();
		}
	};
}

std::wstring FileDialog::getPathFile() {
	if(_fileNameInput->getText() == L"")
		return L"";

	return _currentPath.wstring() + L"\\" + _fileNameInput->getText();
}

void FileDialog::setPosition(sf::Vector2i position) {
	Main::Window::setPosition(position);

	_topRect.position = _contentRect.position;
	_leftRect.position = _contentRect.position + sf::Vector2i(0, _topRect.size.y);
	_rightRect.position = _leftRect.position + sf::Vector2i(_leftRect.size.x - _padding, 0);
	_bottomRect.position = _contentRect.position + sf::Vector2i(0, _contentRect.size.y - _bottomRectHeight);

	_topInnerRect->position = _topRect.position + sf::Vector2i(_padding, 8);
	_leftInnerRect->position = _leftRect.position + sf::Vector2i(_padding, _padding);
	_rightInnerRect->position = _rightRect.position + sf::Vector2i(_padding, _padding);
	_bottomInnerRect->position = _bottomRect.position + sf::Vector2i(_padding, 0);

	{
		int marginTop = (_topInnerRect->size.y - 32) / 2;
		int marginLeft = 4;
		_prevButton->setPosition(_topInnerRect->position + sf::Vector2i(marginLeft, marginTop));
		_nextButton->setPosition(_topInnerRect->position + sf::Vector2i(32, 0) + sf::Vector2i(marginLeft, marginTop));
		_upButton->setPosition(_topInnerRect->position + sf::Vector2i(64, 0) + sf::Vector2i(marginLeft, marginTop));
		
		int navbarX = _upButton->getPosition().x + _upButton->getSize().x + marginLeft;
		_navbar->setPosition(sf::Vector2i(navbarX, _topInnerRect->position.y + marginTop));
	}

	{
		int scrollbarValue = _leftScrollbar ? _leftScrollbar->getValue() : 0;

		for (int i = 0; i < _visibleLocations.size(); i++) {
			_visibleLocations[i]->setPosition(_leftInnerRect->position + sf::Vector2i(0, i * basic_text_rect_height - scrollbarValue));
		}
	}

	{
		int rightScrollbarValue = (_rightScrollbar) ? _rightScrollbar->getValue() : 0;
		int startPosition = rightScrollbarValue % basic_text_rect_height;

		for (int i = 0; i < _files.size(); i++) {
			_files[i]->setPosition(_rightInnerRect->position + sf::Vector2i(0, i * basic_text_rect_height - startPosition));
		}
	}

	_leftScrollbar->setPosition(sf::Vector2i(_leftInnerRect->position.x + _leftInnerRect->size.x + 4, _leftInnerRect->position.y));
	_rightScrollbar->setPosition(sf::Vector2i(_rightInnerRect->position.x + _rightInnerRect->size.x + 4, _rightInnerRect->position.y));

	{
		_fileNameText->setPosition(sf::Vector2f(_bottomInnerRect->position + sf::Vector2i(32, 16 + 4)));

		sf::Vector2i textInputSize;
		textInputSize.x = _bottomInnerRect->size.x - 32 - _fileNameText->getGlobalBounds().size.x - 32;
		textInputSize.y = 32;
		_fileNameInput->setPosition(_bottomInnerRect->position + sf::Vector2i(32 + _fileNameText->getGlobalBounds().size.x + 16, 16));

		sf::Vector2i position = _fileNameInput->getPosition() + sf::Vector2i(_fileNameInput->getSize().x - 2 * 128, 40);
		_confirmButton->setPosition(position);
		_cancelButton->setPosition(position + sf::Vector2i(128, 0));
	}
}

sf::FloatRect FileDialog::getFilesRect() {
	return sf::FloatRect(
		sf::Vector2f((float)_rightInnerRect->position.x, (float)_rightInnerRect->position.y),
		sf::Vector2f((float)_rightInnerRect->size.x, (float)(_rightInnerRect->size.y))
	);
}

sf::FloatRect FileDialog::getVisibleLocationsRect() {
	return sf::FloatRect(
		sf::Vector2f((float)_leftInnerRect->position.x, (float)_leftInnerRect->position.y),
		sf::Vector2f((float)_leftInnerRect->size.x, (float)(_leftInnerRect->size.y))
	);
}

void FileDialog::drawTop() {
	_prevButton->draw();
	_nextButton->draw();
	_upButton->draw();
}

void FileDialog::drawFiles() {
	sf::FloatRect filesRect = getFilesRect();
	sf::View view(filesRect);

	sf::FloatRect vp(
		sf::Vector2f(
			filesRect.position.x / GUI_manager->_view.getSize().x,
			filesRect.position.y / GUI_manager->_view.getSize().y
		),

		sf::Vector2f(
			filesRect.size.x / GUI_manager->_view.getSize().x,
			filesRect.size.y / GUI_manager->_view.getSize().y
		)
	);

	view.setViewport(vp);
	Main::render_window->setView(view);

	for (auto& file : _files) {
		file->draw();
	}
}

void FileDialog::drawVisibleLocations() {
	sf::FloatRect locationsRect = getVisibleLocationsRect();
	sf::View view(locationsRect);

	sf::FloatRect vp(
		sf::Vector2f(
			locationsRect.position.x / GUI_manager->_view.getSize().x,
			locationsRect.position.y / GUI_manager->_view.getSize().y
		),

		sf::Vector2f(
			locationsRect.size.x / GUI_manager->_view.getSize().x,
			locationsRect.size.y / GUI_manager->_view.getSize().y
		)
	);

	view.setViewport(vp);
	Main::render_window->setView(view);

	int scrollbarValue = _leftScrollbar ? _leftScrollbar->getValue() : 0;
	int startIndex = scrollbarValue / basic_text_rect_height;
	int visibleCount = (std::ceil((float)(_leftInnerRect->size.y) / basic_text_rect_height)) + 1;
	int endIndex = std::min(startIndex + visibleCount, (int)(_visibleLocations.size()));

	for (int i = startIndex; i < endIndex; i++) {
		if (_visibleLocations[i])
			_visibleLocations[i]->draw();
	}
}

void FileDialog::drawBottom() {
	if (_fileNameText) {
		Main::render_window->draw(*_fileNameText);
	}

	if (_fileNameInput) {
		_fileNameInput->draw();
	}

	_confirmButton->draw();
	_cancelButton->draw();
}

void FileDialog::cursorHover() {
	Main::Window::cursorHover();

	_leftScrollbar->cursorHover();
	_rightScrollbar->cursorHover();

	// nav buttons (top rect)
	_prevButton->cursorHover();
	_nextButton->cursorHover();
	_upButton->cursorHover();

	// nav bar (top rect)
	_navbar->cursorHover();

	// locations (left inner rect)
	if (_leftInnerRect->contains(Main::cursor->_position)) {
		for (auto& location : _visibleLocations) {
			location->cursorHover();
		}
	}

	// files (right inner rect)
	if (_rightInnerRect->contains(Main::cursor->_position)) {
		for (auto& file : _files) {
			file->cursorHover();
		}
	}

	// filename input
	_fileNameInput->cursorHover();
	_confirmButton->cursorHover();
	_cancelButton->cursorHover();
	
}

void FileDialog::handleEvent(const sf::Event& event) {
	Main::Window::handleEvent(event);

	_leftScrollbar->handleEvent(event);
	_rightScrollbar->handleEvent(event);

	_prevButton->handleEvent(event);
	_nextButton->handleEvent(event);
	_upButton->handleEvent(event);

	_navbar->handleEvent(event);

	for (auto& location : _visibleLocations) {
		location->handleEvent(event);
	}

	for (auto& file : _files) {
		file->handleEvent(event);
	}

	_fileNameInput->handleEvent(event);

	_confirmButton->handleEvent(event);
	_cancelButton->handleEvent(event);
	
}

void FileDialog::update() {
	Main::Window::update();

	_leftScrollbar->update();
	_rightScrollbar->update();

	_prevButton->update();
	_nextButton->update();
	_upButton->update();

	_navbar->update();

	for (int i = 0; i < _visibleLocations.size(); i++) {

		std::shared_ptr<LocationItem> locationItem = _visibleLocations[i];

		if (!locationItem)
			continue;

		bool openerPressed =
			locationItem->_opener &&
			locationItem->_opener->_state == CheckboxState::Pressed;

		locationItem->update();

		if (openerPressed)
			return;
	}

	for (auto& file : _files) {
		file->update();
	}

	_fileNameInput->update();

	_confirmButton->update();
	_cancelButton->update();
	
}
void FileDialog::draw() {

	Window::draw();

	sf::RectangleShape topRectShape(sf::Vector2f(_topRect.size));
	topRectShape.setPosition(sf::Vector2f(_topRect.position));
	topRectShape.setFillColor(sf::Color(23, 23, 23));
	Main::render_window->draw(topRectShape);

	sf::RectangleShape leftRectShape(sf::Vector2f(_leftRect.size));
	leftRectShape.setPosition(sf::Vector2f(_leftRect.position));
	leftRectShape.setFillColor(sf::Color(23, 23, 23));
	Main::render_window->draw(leftRectShape);

	sf::RectangleShape rightRectShape(sf::Vector2f(_rightRect.size));
	rightRectShape.setPosition(sf::Vector2f(_rightRect.position));
	rightRectShape.setFillColor(sf::Color(23, 23, 23));
	Main::render_window->draw(rightRectShape);

	sf::RectangleShape bottomRectShape(sf::Vector2f(_bottomRect.size));
	bottomRectShape.setPosition(sf::Vector2f(_bottomRect.position));
	bottomRectShape.setFillColor(sf::Color(23, 23, 23));
	Main::render_window->draw(bottomRectShape);

	sf::RectangleShape topInnerRectShape(sf::Vector2f(_topInnerRect->size));
	topInnerRectShape.setPosition(sf::Vector2f(_topInnerRect->position));
	topInnerRectShape.setFillColor(sf::Color(47, 47, 47));
	topInnerRectShape.setOutlineThickness(1.0f);
	topInnerRectShape.setOutlineColor(sf::Color(63, 63, 63));
	Main::render_window->draw(topInnerRectShape);

	sf::RectangleShape leftInnerRectShape(sf::Vector2f(_leftInnerRect->size));
	leftInnerRectShape.setPosition(sf::Vector2f(_leftInnerRect->position));
	leftInnerRectShape.setFillColor(sf::Color(47, 47, 47));
	leftInnerRectShape.setOutlineThickness(1.0f);
	leftInnerRectShape.setOutlineColor(sf::Color(63, 63, 63));
	Main::render_window->draw(leftInnerRectShape);

	sf::RectangleShape rightInnerRectShape(sf::Vector2f(_rightInnerRect->size));
	rightInnerRectShape.setPosition(sf::Vector2f(_rightInnerRect->position));
	rightInnerRectShape.setFillColor(sf::Color(47, 47, 47));
	rightInnerRectShape.setOutlineThickness(1.0f);
	rightInnerRectShape.setOutlineColor(sf::Color(63, 63, 63));
	Main::render_window->draw(rightInnerRectShape);

	sf::RectangleShape bottomInnerRectShape(sf::Vector2f(_bottomInnerRect->size));
	bottomInnerRectShape.setPosition(sf::Vector2f(_bottomInnerRect->position));
	bottomInnerRectShape.setFillColor(sf::Color(23, 23, 23));
	bottomInnerRectShape.setOutlineThickness(1.0f);
	bottomInnerRectShape.setOutlineColor(sf::Color(63, 63, 63));
	Main::render_window->draw(bottomInnerRectShape);

	_navbar->draw();

	drawTop();
	drawFiles();
	drawVisibleLocations();
	
	Main::render_window->setView(GUI_manager->_view);

	drawBottom();

	_leftScrollbar->draw();
	_rightScrollbar->draw();
}