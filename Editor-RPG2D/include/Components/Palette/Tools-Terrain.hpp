#pragma once
#include "Controls/ButtonWithSprite.hpp"
#include "Components/Palette/Tools.hpp"
#include "Objects/Object.hpp"

enum class ToolTerrainType { None, Circle, Rect };

class ToolTerrainButton : public ButtonWithSprite {
public:
	std::shared_ptr<Texture> _toolTexture;

	ToolTerrainButton(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> toolTexture, sf::Vector2i position = sf::Vector2i(0, 0));
	~ToolTerrainButton();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};

class ToolsTerrain : public Tools {
public:
	
	std::shared_ptr<ButtonWithSprite> _prev, _next;
	std::vector<std::shared_ptr<ToolTerrainButton>> _tools;
	std::shared_ptr<ToolTerrainButton> _selectedTool = nullptr;
	ToolTerrainType _toolType = ToolTerrainType::None;

	ToolsTerrain();
	~ToolsTerrain();

	void addTool(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> toolTexture, std::function<void()> function);
	void setTool(std::shared_ptr<ToolTerrainButton> button, ToolTerrainType type);
	virtual void setPosition(sf::Vector2i position);

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};