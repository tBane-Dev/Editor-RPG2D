#include "Components/Palette/Tools-Building.hpp"
#include "RenderWindow.hpp"
#include "Theme.hpp"
#include "DebugLog.hpp"

ToolBuildingWallTypeOption::ToolBuildingWallTypeOption(std::wstring text, ToolBuildingWallType type, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position) : ButtonWithTextAndSprite(text, texture, hoverTexture, pressTexture, nullptr, position) {
	_type = type;
} 

ToolBuildingWallTypeOption::~ToolBuildingWallTypeOption() {

}

ToolsBuilding::ToolsBuilding() : Tools() {

	_text->setString(L"Build Options");

	createCategories();
	selectCategory(0);

	_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 4 * 128) / 3;
	
	//DebugLog(std::to_wstring(_rect.size.x) + L" x " + std::to_wstring(_rect.size.y));

	createNavButtons();
	createOptions();

	_selectedWallTypeIndex = 0;
	_selectedHeightIndex = 2;
	_selectedRoofShapeIndex = 0;
	_selectedRoofTypeIndex = 0;

	selectOption();

	_rect.size = sf::Vector2i(600 - 2 * _main_margin, 2 * 64 + 2 * _main_margin + _top_margin + 2* _inner_margin);
}

ToolsBuilding::~ToolsBuilding() {

}

void ToolsBuilding::createCategories() {

	_categories.clear();

	_wallsType = std::make_shared<ButtonWithTextAndSprite>(
		L"walls type",
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_press.png")
	);
	_categories.push_back(_wallsType);

	_height = std::make_shared<ButtonWithTextAndSprite>(
		L"height",
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_press.png")
	);
	_categories.push_back(_height);

	_roofShape = std::make_shared<ButtonWithTextAndSprite>(
		L"roof shape",
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_press.png")
	);
	_categories.push_back(_roofShape);

	_roofType = std::make_shared<ButtonWithTextAndSprite>(
		L"roof type",
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_press.png")
	);
	_categories.push_back(_roofType);

	for (int i = 0; i < _categories.size(); i += 1) {
		_categories[i]->_onclick_func = [this, i]() { 
			selectCategory(i); 
			createOptions();
			selectOption();
			setPosition(getPosition() - sf::Vector2i(_outer_margin, _outer_margin)); // Update positions of options
			};
	}
}

void ToolsBuilding::selectCategory(int id) {

	if (id < 0 || id >= _categories.size())
		return;

	if (_selectedCategoryIndex > -1 && _selectedCategoryIndex < _categories.size()) {
		std::shared_ptr<ButtonWithTextAndSprite> oldSelected = _categories[_selectedCategoryIndex];

		if (oldSelected) {
			oldSelected->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category.png");
			oldSelected->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_hover.png");
			oldSelected->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_press.png");
		}
	}

	_selectedCategoryIndex = id;

	for (auto& category : _categories) {
		category->setSelect(category == _categories[_selectedCategoryIndex]);
	}

	std::shared_ptr<ButtonWithTextAndSprite> newSelected = _categories[_selectedCategoryIndex];

	if (newSelected) {
		newSelected->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_selected.png");
		newSelected->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_selected_hover.png");
		newSelected->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_selected_press.png");
	}
}

void ToolsBuilding::createNavButtons() {
	_prev = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\prev.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\prev_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\prev_press.png")
	);

	_next = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\next.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\next_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\next_press.png")
	);

	_prev->_onclick_func = [this]() {

		int* startIndex = nullptr;

		if (_categories[_selectedCategoryIndex] == _wallsType)
			startIndex = &_startWallTypeIndex;
		else if (_categories[_selectedCategoryIndex] == _height)
			startIndex = &_startHeightIndex;
		else if (_categories[_selectedCategoryIndex] == _roofShape)
			startIndex = &_startRoofShapeIndex;
		else if (_categories[_selectedCategoryIndex] == _roofType)
			startIndex = &_startRoofTypeIndex;

		if (startIndex && *startIndex > 0) {
			*startIndex -= 1;
			createOptions();
			setPosition(getPosition() - sf::Vector2i(_outer_margin, _outer_margin));
		}
		};

	_next->_onclick_func = [this]() {

		int* startIndex = nullptr;

		if (_categories[_selectedCategoryIndex] == _wallsType)
			startIndex = &_startWallTypeIndex;
		else if (_categories[_selectedCategoryIndex] == _height)
			startIndex = &_startHeightIndex;
		else if (_categories[_selectedCategoryIndex] == _roofShape)
			startIndex = &_startRoofShapeIndex;
		else if (_categories[_selectedCategoryIndex] == _roofType)
			startIndex = &_startRoofTypeIndex;

		if (startIndex && _options.size() > _visibleOptionsCount + *startIndex) {
			*startIndex += 1;
			createOptions();
			setPosition(getPosition() - sf::Vector2i(_outer_margin, _outer_margin));
		}
		};
}

void ToolsBuilding::createOptions() {
	_options.clear();
	_visibleOptionsCount = 0;

	if (_categories[_selectedCategoryIndex] == _wallsType) {

		std::vector<std::wstring> names = { L"Wooden", L"Stone", L"Mulch", L"Mud", L"Brick" };
		_visibleOptionsCount = 3;

		for (int i = 0; i + _startWallTypeIndex < names.size(); i++) {

			int optionIndex = i + _startWallTypeIndex;

			auto option = std::make_shared<ToolBuildingWallTypeOption>(
				names[optionIndex],
				ToolBuildingWallType(optionIndex),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_walls_type.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_walls_type_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_walls_type_press.png")
			);

			option->_onclick_func = [this, optionIndex]() {
				selectOption(optionIndex);
				};
			
			_options.push_back(option);
		}

		_inner_margin2 = (600 - 2 * _main_margin - 2 * _outer_margin - _prev->getSize().x - _next->getSize().x - _visibleOptionsCount * 112) / (_visibleOptionsCount + 1);

	}

	if(_categories[_selectedCategoryIndex] == _height) {
		std::vector<std::wstring> names = { L"2", L"3", L"4", L"5", L"6" };
		_visibleOptionsCount = 5;

		for (int i = 0; i + _startHeightIndex < names.size(); i++) {
			int optionIndex = i + _startHeightIndex;
			auto option = std::make_shared<ButtonWithTextAndSprite>(
				names[optionIndex],
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png")
			);
			option->_onclick_func = [this, optionIndex]() {
				selectOption(optionIndex);
				};
			_options.push_back(option);
		}

		_inner_margin2 = (600 - 2 * _main_margin - 2 * _outer_margin - _prev->getSize().x - _next->getSize().x - _visibleOptionsCount * 64) / (_visibleOptionsCount + 1);

	}

	if (_categories[_selectedCategoryIndex] == _roofShape) {
		std::vector<std::wstring> names = { L"flat", L"gable", L"flanks", L"gable2"};
		_visibleOptionsCount = 3;

		for (int i = 0; i + _startRoofShapeIndex < names.size(); i++) {
			int optionIndex = i + _startRoofShapeIndex;
			auto option = std::make_shared<ButtonWithTextAndSprite>(
				names[optionIndex],
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape_press.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape.png")
			);
			option->_onclick_func = [this, optionIndex]() {
				selectOption(optionIndex);
				};
			_options.push_back(option);
		}

		_inner_margin2 = (600 - 2 * _main_margin - 2 * _outer_margin - _prev->getSize().x - _next->getSize().x - _visibleOptionsCount * 112) / (_visibleOptionsCount + 1);

	}

	if (_categories[_selectedCategoryIndex] == _roofType) {
		std::vector<std::wstring> names = { L"red", L"stone", L"green", L"blue", L"sand"};
		_visibleOptionsCount = 5;

		for (int i = 0; i + _startRoofTypeIndex < names.size(); i++) {
			int optionIndex = i + _startRoofTypeIndex;
			auto option = std::make_shared<ButtonWithTextAndSprite>(
				names[optionIndex],
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type_press.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type.png")
			);
			option->_onclick_func = [this, optionIndex]() {
				selectOption(optionIndex);
				};
			_options.push_back(option);
		}

		_inner_margin2 = (600 - 2 * _main_margin - 2 * _outer_margin - _prev->getSize().x - _next->getSize().x - _visibleOptionsCount * 72) / (_visibleOptionsCount + 1);

	}

}

void ToolsBuilding::selectOption() {

	int* selectedIndex = nullptr;
	int* startIndex = nullptr;

	if (_categories[_selectedCategoryIndex] == _wallsType) {
		selectedIndex = &_selectedWallTypeIndex;
		startIndex = &_startWallTypeIndex;
	}
	else if (_categories[_selectedCategoryIndex] == _height) {
		selectedIndex = &_selectedHeightIndex;
		startIndex = &_startHeightIndex;
	}
	else if (_categories[_selectedCategoryIndex] == _roofShape) {
		selectedIndex = &_selectedRoofShapeIndex;
		startIndex = &_startRoofShapeIndex;
	}
	else if (_categories[_selectedCategoryIndex] == _roofType) {
		selectedIndex = &_selectedRoofTypeIndex;
		startIndex = &_startRoofTypeIndex;
	}

	if (!selectedIndex || !startIndex)
		return;

	int optionsCount = *startIndex + static_cast<int>(_options.size());
	int maxStartIndex = std::max(0, optionsCount - _visibleOptionsCount);

	if (*selectedIndex < *startIndex) {
		*startIndex = *selectedIndex;
	}
	else if (*selectedIndex >= *startIndex + _visibleOptionsCount) {
		*startIndex = *selectedIndex - _visibleOptionsCount + 1;
	}

	*startIndex = std::clamp(*startIndex, 0, maxStartIndex);

	createOptions();
	selectOption(*selectedIndex);
}

void ToolsBuilding::selectOption(int id) {

	int* startIndex = nullptr;
	int* selectedOptionIndex = nullptr;

	if (_categories[_selectedCategoryIndex] == _wallsType) {
		startIndex = &_startWallTypeIndex;
		selectedOptionIndex = &_selectedWallTypeIndex;
	}
	else if (_categories[_selectedCategoryIndex] == _height) {
		startIndex = &_startHeightIndex;
		selectedOptionIndex = &_selectedHeightIndex;
	}
	else if (_categories[_selectedCategoryIndex] == _roofShape) {
		startIndex = &_startRoofShapeIndex;
		selectedOptionIndex = &_selectedRoofShapeIndex;
	}
	else if (_categories[_selectedCategoryIndex] == _roofType) {
		startIndex = &_startRoofTypeIndex;
		selectedOptionIndex = &_selectedRoofTypeIndex;
	}

	if (!startIndex || !selectedOptionIndex)
		return;

	int localIndex = id - *startIndex;

	if (localIndex < 0 || localIndex >= _options.size()) {
		for (auto& option : _options) {
			option->setSelect(false);
		}
		return;
	}

	int oldLocalIndex = *selectedOptionIndex - *startIndex;

	if (oldLocalIndex >= 0 && oldLocalIndex < _options.size()) {
		auto oldSelected = _options[oldLocalIndex];

		if (_categories[_selectedCategoryIndex] == _wallsType) {
			oldSelected->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_walls_type.png");
			oldSelected->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_walls_type_hover.png");
			oldSelected->_pressTexture =textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_walls_type_press.png");
		} 
		else if(_categories[_selectedCategoryIndex] == _height) {
			oldSelected->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png");
			oldSelected->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png");
			oldSelected->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png");
		}
		else if (_categories[_selectedCategoryIndex] == _roofShape) {
			oldSelected->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape.png");
			oldSelected->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape_hover.png");
			oldSelected->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape_press.png");
		}
		else if (_categories[_selectedCategoryIndex] == _roofType) {
			oldSelected->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type.png");
			oldSelected->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type_hover.png");
			oldSelected->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type_press.png");
		}
		else {
			oldSelected->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png");
			oldSelected->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png");
			oldSelected->_pressTexture =textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png");
		}
	}

	*selectedOptionIndex = id;

	for (int i = 0; i < _options.size(); i += 1) {
		_options[i]->setSelect(i == localIndex);
	}

	auto newSelected = _options[localIndex];

	if (_categories[_selectedCategoryIndex] == _wallsType) {
		newSelected->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_walls_type_selected.png");
		newSelected->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_walls_type_selected_hover.png");
		newSelected->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_walls_type_selected_press.png");
	}
	else if(_categories[_selectedCategoryIndex] == _height) {
		newSelected->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\selected.png");
		newSelected->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\selected_hover.png");
		newSelected->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\selected_press.png");
	}
	else if (_categories[_selectedCategoryIndex] == _roofShape) {
		newSelected->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape_selected.png");
		newSelected->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape_selected_hover.png");
		newSelected->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape_selected_press.png");
	}
	else if (_categories[_selectedCategoryIndex] == _roofType) {
		newSelected->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type_selected.png");
		newSelected->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type_selected_hover.png");
		newSelected->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type_selected_press.png");
	}
	else {
		newSelected->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\selected.png");
		newSelected->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\selected_hover.png");
		newSelected->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\selected_press.png");
	}
}

void ToolsBuilding::setPosition(sf::Vector2i position) {

	_rect.position = position + sf::Vector2i(_main_margin, _main_margin);

	int x = _rect.position.x + _outer_margin;
	int y = _rect.position.y + _outer_margin;

	_text->setPosition(sf::Vector2f(x, y));

	y += _top_margin + _inner_margin;

	for (auto& category : _categories) {
		category->setPosition(sf::Vector2i(x, y));
		x += category->_rect.size.x + _inner_margin;
	}

	y += 64 + _inner_margin;
	x = _rect.position.x + _outer_margin;

	_prev->setPosition(sf::Vector2i(x, y));

	x += _prev->getSize().x + _inner_margin2;

	for (int i = 0; i < _options.size() && i < _visibleOptionsCount; i += 1) {
		_options[i]->setPosition(sf::Vector2i(x, y));
		x += _options[i]->_rect.size.x + _inner_margin2;
	}

	x = _rect.position.x + _rect.size.x - 64 - 2 * _outer_margin ;
	_next->setPosition(sf::Vector2i(x, y));
}

void ToolsBuilding::cursorHover() {

	_prev->cursorHover();
	_next->cursorHover();

	for (auto& category : _categories) {
		category->cursorHover();
	}

	for (int i = 0; i < _options.size() && i < _visibleOptionsCount; i += 1) {
		_options[i]->cursorHover();
	}
}

void ToolsBuilding::handleEvent(const sf::Event& event) {

	_prev->handleEvent(event);
	_next->handleEvent(event);

	for (auto& category : _categories) {
		category->handleEvent(event);
	}

	for (int i = 0; i < _options.size() && i < _visibleOptionsCount; i += 1) {
		_options[i]->handleEvent(event);
	}
}

void ToolsBuilding::update() {

	_prev->update();
	_next->update();

	for (auto& category : _categories) {
		category->update();
	}

	for (int i = 0; i < _options.size() && i < _visibleOptionsCount; i += 1) {
		_options[i]->update();
	}
}

void ToolsBuilding::draw() {

	int border = 2;
	sf::RectangleShape background(sf::Vector2f(_rect.size.x - 2 * border, _rect.size.y - 2 * border));
	background.setPosition(sf::Vector2f(_rect.position.x + border, _rect.position.y + border));
	background.setFillColor(sf::Color(31, 31, 31));
	background.setOutlineThickness(2);
	background.setOutlineColor(sf::Color(63, 63, 63));
	Main::render_window->draw(background);

	Main::render_window->draw(*_text);

	_prev->draw();
	_next->draw();

	for (auto& category : _categories) {
		category->draw();
	}

	for (int i = 0; i < _options.size() && i < _visibleOptionsCount; i += 1) {
		_options[i]->draw();
	}
}