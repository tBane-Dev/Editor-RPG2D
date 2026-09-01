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

	std::shared_ptr<ButtonWithTextAndSprite> _wallsType, _height, _roofShape, _roofType;
	std::vector<std::shared_ptr<ButtonWithTextAndSprite>> _categories;
	int _selectedCategoryIndex = -1;
	
	std::shared_ptr<ButtonWithSprite> _prev, _next;
	std::vector<std::shared_ptr<ButtonWithTextAndSprite>> _options;
	int _visibleOptionsCount;
	
	std::vector<std::wstring> _wallTypes;
	std::vector<std::wstring> _heights;
	std::vector<std::wstring> _roofShapes;
	std::vector<std::wstring> _roofTypes;

	int _optionsCount = 0;

	int _startWallTypeIndex = 0; // scrollbar offset for options
	int _startHeightIndex = 0;
	int _startRoofShapeIndex = 0;
	int _startRoofTypeIndex = 0;

	int _selectedWallTypeIndex = -1;
	int _selectedHeightIndex = -1;
	int _selectedRoofShapeIndex = -1;
	int _selectedRoofTypeIndex = -1;
	int _inner_margin2;

	ToolsBuilding();
	~ToolsBuilding();

	int getWallType();
	int getHeight();
	int getRoofShape();
	int getRoofType();

	void createCategories();
	void selectCategory(int id);
	void createNavButtons();
	void createOptions();
	void updateOptions();
	void selectOption();
	void selectOption(int id);

	virtual void setPosition(sf::Vector2i position);

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};