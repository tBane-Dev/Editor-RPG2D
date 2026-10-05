#pragma once
#include "Controls/ButtonWithSprite.hpp"
#include "Components/Palette/Tools.hpp"
#include "Objects/Object.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"

enum class ToolBuildingWallType {Wooden, Stone, Mulch, Mud, Brick};

class ToolBuildingWallTypeOption: public ButtonWithTextAndSprite {
public:

	ToolBuildingWallType _type;

	ToolBuildingWallTypeOption(std::wstring text, ToolBuildingWallType type, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position = sf::Vector2i(0,0));
	~ToolBuildingWallTypeOption();
};


class ToolsBuilding : public Tools {
public:

	std::shared_ptr<ButtonWithSprite> _prevCategory, _nextCategory;
	std::shared_ptr<ButtonWithTextAndSprite> _wallsType, _height, _roofShape, _roofType, _wallsRoof, _floor, _skeleton, _foundation;
	std::vector<std::shared_ptr<ButtonWithTextAndSprite>> _categories;
	std::vector<std::shared_ptr<ButtonWithTextAndSprite>> _visibleCategories;
	int _visibleCategoriesCount;
	int _selectedCategoryIndex = -1;
	int _startCategoryIndex = 0; // scrollbar offset for options

	std::vector<std::wstring> _wallTypes;
	std::vector<std::wstring> _heights;
	std::vector<std::wstring> _roofShapes;
	std::vector<std::wstring> _roofTypes;
	std::vector<std::wstring> _wallsRoofs;
	std::vector<std::wstring> _floors;
	std::vector<std::wstring> _skeletons;

	std::shared_ptr<ButtonWithSprite> _prevOption, _nextOption;
	std::vector<std::shared_ptr<ButtonWithTextAndSprite>> _visibleOptions;
	int _visibleOptionsCount;

	int _optionsCount = 0;

	// scrollbar offset for options
	int _startWallTypeIndex = 0; 
	int _startHeightIndex = 0;
	int _startRoofShapeIndex = 0;
	int _startRoofTypeIndex = 0;
	int _startWallsRoofIndex = 0;
	int _startFloorIndex = 0;
	int _startSkeletonIndex = 0;

	// selected index for options
	int _selectedWallTypeIndex = -1;
	int _selectedHeightIndex = -1;
	int _selectedRoofShapeIndex = -1;
	int _selectedRoofTypeIndex = -1;
	int _selectedWallsRoofIndex = -1;
	int _selectedFloorIndex = -1;
	int _selectedSkeletonIndex = -1;

	int _inner_margin2;

	ToolsBuilding();
	~ToolsBuilding();

	int getWallType();
	int getHeight();
	int getRoofShape();
	int getRoofType();
	int getFloor();
	int getWallsRoof();
	int getSkeleton();

	void createNavButtons();

	void createCategories();
	void updateCategories();
	void selectCategory(int id);

	void createOptions();
	void updateOptions();
	bool selectOption(int id, bool force);

	virtual void setPosition(sf::Vector2i position);

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};