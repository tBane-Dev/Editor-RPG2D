#include "Components/Palette/Tools-Building.hpp"
#include "RenderWindow.hpp"
#include "Theme.hpp"
#include "DebugLog.hpp"
#include "Components/Palette/Palette.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/MapEditor/Palette.hpp"


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
	updateOptions();

	_selectedWallTypeIndex = 0;
	_selectedHeightIndex = 1;
	_selectedRoofShapeIndex = 0;
	_selectedRoofTypeIndex = 0;

	selectOption();

	_rect.size = sf::Vector2i(600 - 2 * _main_margin, 2 * 64 + 2 * _main_margin + _top_margin + 2* _inner_margin);
}

ToolsBuilding::~ToolsBuilding() {

}

int ToolsBuilding::getWallType() {
	return _selectedWallTypeIndex;
}

int ToolsBuilding::getHeight() {
	return _selectedHeightIndex + 2;
}

int ToolsBuilding::getRoofShape() {
	return _selectedRoofShapeIndex;
}

int ToolsBuilding::getRoofType() {
	return _selectedRoofTypeIndex;
}

void ToolsBuilding::createNavButtons() {
	_prevCategory = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\prev.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\prev_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\prev_press.png")
	);

	_nextCategory = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\next.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\next_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\next_press.png")
	);

	_prevCategory->_onclick_func = [this]() {
		if (_startCategoryIndex > 0) {
			_startCategoryIndex -= 1;
			updateCategories();
			selectCategory(_selectedCategoryIndex);
			setPosition(getPosition() - sf::Vector2i(_outer_margin, _outer_margin));
		}
		};

	_nextCategory->_onclick_func = [this]() {
		if (_startCategoryIndex + _visibleCategoriesCount < _categories.size()) {
			_startCategoryIndex += 1;
			updateCategories();
			selectCategory(_selectedCategoryIndex);
			setPosition(getPosition() - sf::Vector2i(_outer_margin, _outer_margin));
		}
		};

	_prevOption = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\prev.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\prev_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\prev_press.png")
	);

	_nextOption = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\next.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\next_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\next_press.png")
	);

	_prevOption->_onclick_func = [this]() {

		int* startIndex = nullptr;
		int* selectedIndex = nullptr;

		if (_categories[_selectedCategoryIndex] == _wallsType) {
			startIndex = &_startWallTypeIndex;
			selectedIndex = &_selectedWallTypeIndex;
		}
		else if (_categories[_selectedCategoryIndex] == _height) {
			startIndex = &_startHeightIndex;
			selectedIndex = &_selectedHeightIndex;
		}
		else if (_categories[_selectedCategoryIndex] == _roofShape) {
			startIndex = &_startRoofShapeIndex;
			selectedIndex = &_selectedRoofShapeIndex;
		}
		else if (_categories[_selectedCategoryIndex] == _roofType) {
			startIndex = &_startRoofTypeIndex;
			selectedIndex = &_selectedRoofTypeIndex;
		}

		if (startIndex && selectedIndex && *startIndex > 0) {
			*startIndex -= 1;
			updateOptions();
			selectOption(*selectedIndex);
			setPosition(getPosition() - sf::Vector2i(_outer_margin, _outer_margin));
		}
		};

	_nextOption->_onclick_func = [this]() {

		int* startIndex = nullptr;
		int* selectedIndex = nullptr;

		if (_categories[_selectedCategoryIndex] == _wallsType) {
			startIndex = &_startWallTypeIndex;
			selectedIndex = &_selectedWallTypeIndex;
		}
		else if (_categories[_selectedCategoryIndex] == _height) {
			startIndex = &_startHeightIndex;
			selectedIndex = &_selectedHeightIndex;
		}
		else if (_categories[_selectedCategoryIndex] == _roofShape) {
			startIndex = &_startRoofShapeIndex;
			selectedIndex = &_selectedRoofShapeIndex;
		}
		else if (_categories[_selectedCategoryIndex] == _roofType) {
			startIndex = &_startRoofTypeIndex;
			selectedIndex = &_selectedRoofTypeIndex;
		}

		if (startIndex && selectedIndex && _visibleOptionsCount + *startIndex < _optionsCount) {
			*startIndex += 1;
			updateOptions();
			selectOption(*selectedIndex);
			setPosition(getPosition() - sf::Vector2i(_outer_margin, _outer_margin));
		}
		};
}


void ToolsBuilding::createCategories() {

	_visibleCategoriesCount = 3;

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

	_floor = std::make_shared<ButtonWithTextAndSprite>(
		L"floor",
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_press.png")
	);
	_categories.push_back(_floor);

	_foundation = std::make_shared<ButtonWithTextAndSprite>(
		L"foundation",
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_press.png")
	);
	_categories.push_back(_foundation);

	_skeleton = std::make_shared<ButtonWithTextAndSprite>(
		L"skeleton",
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_category_press.png")
	);
	_categories.push_back(_skeleton);

	for (int i = 0; i < _categories.size(); i += 1) {
		_categories[i]->_onclick_func = [this, i]() { 
			updateCategories();
			selectCategory(i); 
			updateOptions();
			selectOption();
			setPosition(getPosition() - sf::Vector2i(_outer_margin, _outer_margin)); // Update positions of options
			};
	}

	updateCategories();
}

void ToolsBuilding::updateCategories() {
	
	_visibleCategories.clear();
	for(int i=_startCategoryIndex; i < _startCategoryIndex + _visibleCategoriesCount && i < _categories.size(); i++) {
		_visibleCategories.push_back(_categories[i]);
	}

	std::wcout << L"\n\n\n";
}

void ToolsBuilding::selectCategory(int id) {

	if (id < 0 || id >= _categories.size())
		return;

	if (_selectedCategoryIndex >= 0 && _selectedCategoryIndex < _categories.size()) {
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


void ToolsBuilding::createOptions() {
	_wallTypes = { L"Wooden", L"Stone", L"Mulch", L"Mud", L"Brick" };
	_heights = { L"2", L"3", L"4", L"5", L"6", L"7", L"8" };
	_roofShapes = { L"flat", L"gable", L"flanks", L"gable2" };
	_roofTypes = { L"red", L"stone", L"green", L"blue", L"sand" };
}

void ToolsBuilding::updateOptions() {
	_visibleOptions.clear();
	_visibleOptionsCount = 0;

	if (_categories[_selectedCategoryIndex] == _wallsType) {

		_visibleOptionsCount = 3;
		_optionsCount = _wallTypes.size();

		for (int i = 0; i + _startWallTypeIndex < _wallTypes.size(); i++) {

			int optionIndex = i + _startWallTypeIndex;

			auto option = std::make_shared<ToolBuildingWallTypeOption>(
				_wallTypes[optionIndex],
				ToolBuildingWallType(optionIndex),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_walls_type.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_walls_type_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_walls_type_press.png")
			);

			option->_onclick_func = [this, optionIndex]() {
				selectOption(optionIndex);
				Components::Palette::createBuildingsPrefabs(1, getWallType(), getHeight(), getRoofShape(), getRoofType());
				MapEditor::editor->_palette->_slots->updateObjects();
				};
			
			_visibleOptions.push_back(option);
		}

		_inner_margin2 = (600 - 2 * _main_margin - 2 * _outer_margin - _prevOption->getSize().x - _nextOption->getSize().x - _visibleOptionsCount * 112) / (_visibleOptionsCount + 1);

	}

	if(_categories[_selectedCategoryIndex] == _height) {
		_visibleOptionsCount = 5;
		_optionsCount = _heights.size();

		for (int i = 0; i + _startHeightIndex < _heights.size(); i++) {
			int optionIndex = i + _startHeightIndex;
			auto option = std::make_shared<ButtonWithTextAndSprite>(
				_heights[optionIndex],
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png")
			);
			option->_onclick_func = [this, optionIndex]() {
				selectOption(optionIndex);
				Components::Palette::createBuildingsPrefabs(1, getWallType(), getHeight(), getRoofShape(), getRoofType());
				MapEditor::editor->_palette->_slots->updateObjects();
				};
			_visibleOptions.push_back(option);
		}

		_inner_margin2 = (600 - 2 * _main_margin - 2 * _outer_margin - _prevOption->getSize().x - _nextOption->getSize().x - _visibleOptionsCount * 64) / (_visibleOptionsCount + 1);

	}

	if (_categories[_selectedCategoryIndex] == _roofShape) {
		
		_visibleOptionsCount = 3;
		_optionsCount = _roofShapes.size();

		for (int i = 0; i + _startRoofShapeIndex < _roofShapes.size(); i++) {
			int optionIndex = i + _startRoofShapeIndex;
			auto option = std::make_shared<ButtonWithTextAndSprite>(
				_roofShapes[optionIndex],
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape_press.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_shape.png")
			);
			option->_onclick_func = [this, optionIndex]() {
				selectOption(optionIndex);
				Components::Palette::createBuildingsPrefabs(1, getWallType(), getHeight(), getRoofShape(), getRoofType());
				MapEditor::editor->_palette->_slots->updateObjects();
				};
			_visibleOptions.push_back(option);
		}

		_inner_margin2 = (600 - 2 * _main_margin - 2 * _outer_margin - _prevOption->getSize().x - _nextOption->getSize().x - _visibleOptionsCount * 112) / (_visibleOptionsCount + 1);

	}

	if (_categories[_selectedCategoryIndex] == _roofType) {
		
		_visibleOptionsCount = 5;
		_optionsCount = _roofTypes.size();

		for (int i = 0; i + _startRoofTypeIndex < _roofTypes.size(); i++) {
			int optionIndex = i + _startRoofTypeIndex;
			auto option = std::make_shared<ButtonWithTextAndSprite>(
				_roofTypes[optionIndex],
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type_hover.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type_press.png"),
				textures_manager->getTexture(L"assets\\tex\\palette\\tools\\building_roof_type.png")
			);
			option->_onclick_func = [this, optionIndex]() {
				selectOption(optionIndex);
				Components::Palette::createBuildingsPrefabs(1, getWallType(), getHeight(), getRoofShape(), getRoofType());
				MapEditor::editor->_palette->_slots->updateObjects();
				};
			_visibleOptions.push_back(option);
		}

		_inner_margin2 = (600 - 2 * _main_margin - 2 * _outer_margin - _prevOption->getSize().x - _nextOption->getSize().x - _visibleOptionsCount * 72) / (_visibleOptionsCount + 1);

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

	int optionsCount = *startIndex + static_cast<int>(_visibleOptions.size());
	int maxStartIndex = std::max(0, optionsCount - _visibleOptionsCount);

	if (*selectedIndex < *startIndex) {
		*startIndex = *selectedIndex;
	}
	else if (*selectedIndex >= *startIndex + _visibleOptionsCount) {
		*startIndex = *selectedIndex - _visibleOptionsCount + 1;
	}

	*startIndex = std::clamp(*startIndex, 0, maxStartIndex);

	updateOptions();
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

	if (localIndex < 0 || localIndex >= _visibleOptions.size()) {
		for (auto& option : _visibleOptions) {
			option->setSelect(false);
		}
		return;
	}

	int oldLocalIndex = *selectedOptionIndex - *startIndex;

	if (oldLocalIndex >= 0 && oldLocalIndex < _visibleOptions.size()) {
		auto oldSelected = _visibleOptions[oldLocalIndex];

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

	for (int i = 0; i < _visibleOptions.size(); i += 1) {
		_visibleOptions[i]->setSelect(i == localIndex);
	}

	auto newSelected = _visibleOptions[localIndex];

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

	_prevCategory->setPosition(sf::Vector2i(x, y));
	x += _prevCategory->getSize().x + _inner_margin;

	for (auto& category : _visibleCategories) {
		category->setPosition(sf::Vector2i(x, y));
		x += category->_rect.size.x + _inner_margin;
	}

	x = _rect.position.x + _rect.size.x - 60 - _outer_margin;
	_nextCategory->setPosition(sf::Vector2i(x, y));

	y += 64 + _inner_margin;

	x = _rect.position.x + _outer_margin;

	_prevOption->setPosition(sf::Vector2i(x, y));

	x += _prevOption->getSize().x + _inner_margin2;

	for (int i = 0; i < _visibleOptions.size() && i < _visibleOptionsCount; i += 1) {
		_visibleOptions[i]->setPosition(sf::Vector2i(x, y));
		x += _visibleOptions[i]->_rect.size.x + _inner_margin2;
	}

	x = _rect.position.x + _rect.size.x - 60 - _outer_margin ;
	_nextOption->setPosition(sf::Vector2i(x, y));
}

void ToolsBuilding::cursorHover() {

	_prevCategory->cursorHover();
	_nextCategory->cursorHover();

	_prevOption->cursorHover();
	_nextOption->cursorHover();

	for (int i = 0; i < _visibleCategories.size() && i < _visibleCategoriesCount; i += 1) {
		_visibleCategories[i]->cursorHover();
	}

	for (int i = 0; i < _visibleOptions.size() && i < _visibleOptionsCount; i += 1) {
		_visibleOptions[i]->cursorHover();
	}
}

void ToolsBuilding::handleEvent(const sf::Event& event) {

	_prevCategory->handleEvent(event);
	_nextCategory->handleEvent(event);
	
	_prevOption->handleEvent(event);
	_nextOption->handleEvent(event);

	for (int i = 0; i < _visibleCategories.size() && i < _visibleCategoriesCount; i += 1) {
		_visibleCategories[i]->handleEvent(event);
	}

	for (int i = 0; i < _visibleOptions.size() && i < _visibleOptionsCount; i += 1) {
		_visibleOptions[i]->handleEvent(event);
	}
}

void ToolsBuilding::update() {

	_prevCategory->update();
	_nextCategory->update();

	_prevOption->update();
	_nextOption->update();

	for (int i = 0; i < _visibleCategories.size() && i < _visibleCategoriesCount; i += 1) {
		_visibleCategories[i]->update();
	}

	for (int i = 0; i < _visibleOptions.size() && i < _visibleOptionsCount; i += 1) {
		_visibleOptions[i]->update();
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

	_prevCategory->draw();
	_nextCategory->draw();

	_prevOption->draw();
	_nextOption->draw();

	for (int i = 0; i < _visibleCategories.size() && i < _visibleCategoriesCount; i += 1) {
		_visibleCategories[i]->draw();
	}

	for (int i = 0; i < _visibleOptions.size() && i < _visibleOptionsCount; i += 1) {
		_visibleOptions[i]->draw();
	}
}