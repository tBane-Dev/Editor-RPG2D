#pragma once
#include "Controls/ButtonWithSprite.hpp"
#include "Components/Palette/Tools.hpp"
#include "Objects/Object.hpp"

enum class ToolBuildingType { None, Circle, Rect };

class ToolBuildingButton : public ButtonWithSprite {
public:
	std::shared_ptr<Texture> _toolTexture;

	ToolBuildingButton(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> toolTexture, sf::Vector2i position = sf::Vector2i(0, 0));
	~ToolBuildingButton();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};

class ToolsBuilding : public Tools {
public:

	std::shared_ptr<ButtonWithSprite> _prev, _next;
	std::vector<std::shared_ptr<ToolBuildingButton>> _tools;
	std::shared_ptr<ToolBuildingButton> _selectedTool = nullptr;
	ToolBuildingType _toolType = ToolBuildingType::None;

	ToolsBuilding();
	~ToolsBuilding();

	void addTool(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> toolTexture, std::function<void()> function);
	void setTool(std::shared_ptr<ToolBuildingButton> button, ToolBuildingType type);
	virtual void setPosition(sf::Vector2i position);

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};