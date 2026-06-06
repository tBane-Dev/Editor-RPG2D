#include "Windows/FileDialog/FileDialog.hpp"
#include "Theme.hpp"
#include "Cursor.hpp"
#include "RenderWindow.hpp"


int FileDialog::_padding = 8;

FileDialog::FileDialog(std::wstring title) : Main::Window(title, sf::Vector2i(640,416), sf::Vector2i(100,100)) {
	
	_leftRectWidth = 224;
	_bottomRectHeight = 96;
	_scrollbarWidth = 24;

	_topRect = sf::IntRect(_contentRect.position, sf::Vector2i(_contentRect.size.x, 40));
	_leftRect = sf::IntRect(_contentRect.position + sf::Vector2i(0, _topRect.size.y), sf::Vector2i(_leftRectWidth, _contentRect.size.y - _topRect.size.y - _bottomRectHeight));
	_rightRect = sf::IntRect(_leftRect.position + sf::Vector2i(_leftRect.size.x - _padding, 0), sf::Vector2i(_contentRect.size.x - _leftRectWidth + _padding, _leftRect.size.y));
	_bottomRect = sf::IntRect(_contentRect.position + sf::Vector2i(0, _contentRect.size.y - _bottomRectHeight), sf::Vector2i(_contentRect.size.x, _bottomRectHeight));

	_topInnerRect = std::make_shared<sf::IntRect>(_topRect.position + sf::Vector2i(_padding, 8), _topRect.size - sf::Vector2i(_padding * 2, 8));
	_leftInnerRect = std::make_shared<sf::IntRect>(_leftRect.position + sf::Vector2i(_padding, _padding), _leftRect.size - sf::Vector2i(_padding * 2 + _scrollbarWidth, _padding * 2));
	_rightInnerRect = std::make_shared<sf::IntRect>(_rightRect.position + sf::Vector2i(_padding, _padding), _rightRect.size - sf::Vector2i(_padding * 2 + _scrollbarWidth, _padding * 2));

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
	setTheLocationsSize();
	setTheLocations();


	_leftScrollbar = std::make_shared<Scrollbar>(
		_leftInnerRect->position.x + _leftInnerRect->size.x + 4,
		_leftInnerRect->position.y,
		_scrollbarWidth,
		_leftInnerRect->size.y,
		0,
		std::max(0, (int)_locationsPaths.size() * basic_text_rect_height - _leftInnerRect->size.y),
		std::ceil((float)_leftInnerRect->size.y / (float)basic_text_rect_height) * basic_text_rect_height,
		0
	);
	_leftScrollbar->_func = [this]() { setTheLocations(); };
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

	int marginTop = (_topInnerRect->size.y - 32)/2;
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

	if (!isDrive(_currentPath))
		_filesPaths.insert(_filesPaths.begin(), std::filesystem::path(_currentPath).parent_path());

	//std::wcout << L"Total files: " << _paths.size() << std::endl;
		
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
			std::wstring filename = (i + startIndex == 0 && !isDrive(_currentPath))? L".." : _filesPaths[i + startIndex].filename().wstring();
			_files[i]->setFile(filename, _filesPaths[i+startIndex]);
			_files[i]->_onclick_func = [this, i]() {
				if (_files[i]->_type == FileItemType::Drive || _files[i]->_type == FileItemType::Directory) {
					_currentPath = _files[i]->_path;
					loadDirectory();
					_rightScrollbar->setValue(0);
					_rightScrollbar->setMax(std::max(0, (int)_filesPaths.size() * basic_text_rect_height - _rightInnerRect->size.y));
					setTheFilesSize();
					setTheFiles();
				}
				else {
					// TO-DO 
					std::wcout << L"Selected file: " << _files[i]->_path.wstring() << std::endl;
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

void FileDialog::createLocations() {
	
	std::vector<std::wstring> paths;

	// favorite locations
	const wchar_t* userProfile = _wgetenv(L"USERPROFILE");
	std::wstring up(userProfile);
	paths.push_back(up + L"\\AppData\\Roaming\\Microsoft\\Windows\\Recent");
	paths.push_back(up + L"\\Documents");
	paths.push_back(up + L"\\Music");
	paths.push_back(up + L"\\Pictures");
	paths.push_back(up + L"\\Downloads");
	paths.push_back(up + L"\\Desktop");
	paths.push_back(up + L"\\Videos");

	// load the harddrivers
	DWORD drives = GetLogicalDrives();
	for (int i = 0; i < 32; i++)
		if ((drives >> i) & 1) {
			//printf("%c:\\\n", 'A' + i);
			paths.push_back(std::wstring(1, L'A' + i) + L":\\");
		}

	_locationsPaths.clear();
	for (auto& path : paths) {
		_locationsPaths.push_back(std::filesystem::path(path));
	}

	_locations.clear();

	for (auto& path : paths) {
		std::shared_ptr<LocationItem> location = std::make_shared<LocationItem>();

		std::wstring name = L"";
		if (isDrive(path)) {
			name = L"Disc (" + path;
			name.pop_back(); // remove the trailing backslash
			name += L")";
		}
		else
			name = std::filesystem::path(path).filename();

		location->setFile(name, path);
		_locations.push_back(location);
	}
}


void FileDialog::setTheLocationsSize() {
	for(auto& location : _locations) {
		location->setSize(sf::Vector2i(_leftInnerRect->size.x, basic_text_rect_height));
	}
}

void FileDialog::setTheLocations() {

	int scrollbarValue = (_leftScrollbar) ? _leftScrollbar->getValue() : 0;
	int startIndex = scrollbarValue / basic_text_rect_height;
	int offset = scrollbarValue % basic_text_rect_height;

	for (int i = 0; i < _locations.size(); i++) {
			
		int index = i + startIndex;

		if (index < _locationsPaths.size()) {
			if (isDrive(_locationsPaths[index])) {
				std::wstring name = L"Disc (" + _locationsPaths[index].wstring();
				name.pop_back(); // remove the trailing backslash
				name += L")";
				_locations[i]->setFile(name, _locationsPaths[index]);
			}
			else
				_locations[i]->setFile(_locationsPaths[index].filename().wstring(), _locationsPaths[index]);
			
			_locations[i]->setSize(sf::Vector2i(_leftInnerRect->size.x, basic_text_rect_height));
			_locations[i]->_onclick_func = [this, i]() {
				_currentPath = _locations[i]->_path;
				loadDirectory();
				_rightScrollbar->setValue(0);
				_rightScrollbar->setMax(std::max(0, (int)_filesPaths.size() * basic_text_rect_height - _rightInnerRect->size.y));
				setTheFilesSize();
				setTheFiles();
				};
		}

		_locations[i]->setPosition(
			_leftInnerRect->position + sf::Vector2i(0, i * basic_text_rect_height - offset)
		);
	}
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

	{
		int marginTop = (_topInnerRect->size.y - 32) / 2;
		int marginLeft = 4;
		_prevButton->setPosition(_topInnerRect->position + sf::Vector2i(marginLeft, marginTop));
		_nextButton->setPosition(_topInnerRect->position + sf::Vector2i(32, 0) + sf::Vector2i(marginLeft, marginTop));
		_upButton->setPosition(_topInnerRect->position + sf::Vector2i(64, 0) + sf::Vector2i(marginLeft, marginTop));
		int navbarX = _upButton->getPosition().x + _upButton->getSize().x + marginLeft;
		_navbar = std::make_shared<NavBar>(sf::Vector2i(navbarX, _topInnerRect->position.y + marginTop), sf::Vector2i(_topInnerRect->position.x + _topInnerRect->size.x - navbarX, 32));

	}

	{
		int leftScrollbarValue = (_leftScrollbar) ? _leftScrollbar->getValue() : 0;
		int startPosition = leftScrollbarValue % basic_text_rect_height;

		for (int i = 0; i < _locations.size(); i++) {
			_locations[i]->setPosition(_leftInnerRect->position + sf::Vector2i(0, i * basic_text_rect_height - startPosition));
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

}

sf::FloatRect FileDialog::getFilesRect() {
	return sf::FloatRect(
		sf::Vector2f((float)_rightInnerRect->position.x, (float)_rightInnerRect->position.y),
		sf::Vector2f((float)_rightInnerRect->size.x, (float)(_rightInnerRect->size.y))
	);
}

sf::FloatRect FileDialog::getLocationsRect() {
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

void FileDialog::drawLocations() {
	sf::FloatRect locationsRect = getLocationsRect();
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

	for (auto& location : _locations) {
		location->draw();
	}
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
		for (auto& location : _locations) {
			location->cursorHover();
		}
	}

	// files (right inner rect)
	if (_rightInnerRect->contains(Main::cursor->_position)) {
		for (auto& file : _files) {
			file->cursorHover();
		}
	}
	
}

void FileDialog::handleEvent(const sf::Event& event) {
	Main::Window::handleEvent(event);

	_leftScrollbar->handleEvent(event);
	_rightScrollbar->handleEvent(event);

	_prevButton->handleEvent(event);
	_nextButton->handleEvent(event);
	_upButton->handleEvent(event);

	_navbar->handleEvent(event);

	for (auto& location : _locations) {
		location->handleEvent(event);
	}

	for (auto& file : _files) {
		file->handleEvent(event);
	}

	
}

void FileDialog::update() {
	Main::Window::update();

	_leftScrollbar->update();
	_rightScrollbar->update();

	_prevButton->update();
	_nextButton->update();
	_upButton->update();

	_navbar->update();

	for (auto& location : _locations) {
		location->update();
	}

	for (auto& file : _files) {
		file->update();
	}

	
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
	bottomRectShape.setFillColor(sf::Color(47, 47, 47));
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

	_navbar->draw();

	drawTop();
	drawFiles();
	drawLocations();

	Main::render_window->setView(GUI_manager->_view);

	_leftScrollbar->draw();
	_rightScrollbar->draw();
}