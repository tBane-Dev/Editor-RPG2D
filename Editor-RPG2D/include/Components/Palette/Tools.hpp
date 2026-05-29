#pragma once
#include "Controls/ButtonWithSprite.hpp"
#include "Objects/Object.hpp"

enum class ToolType { None, Circle, Rect };

class ToolButton : public ButtonWithSprite {
public:
	std::shared_ptr<Texture> _toolTexture;

	ToolButton(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> toolTexture, sf::Vector2i position = sf::Vector2i(0, 0));
	~ToolButton();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};

class Tools {
public:
	int _main_margin;
	int _top_margin;
	int _outer_margin;
	int _inner_margin;
	sf::IntRect _rect;

	std::unique_ptr<sf::Text> _text;

	
	std::shared_ptr<ButtonWithSprite> _prev, _next;
	std::vector<std::shared_ptr<ToolButton>> _tools;
	std::shared_ptr<ToolButton> _selectedTool = nullptr;
	ToolType _toolType = ToolType::None;

	Tools();
	~Tools();

	sf::Vector2i getPosition();
	sf::Vector2i getSize();
	void addTool(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> toolTexture, std::function<void()> function);
	void setTool(std::shared_ptr<ToolButton> button, ToolType type);
	void setPosition(sf::Vector2i position);

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};