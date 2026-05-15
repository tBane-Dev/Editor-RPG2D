#pragma once
#include "Controls/ButtonWithSprite.hpp"
#include "Editors/MapEditor/Objects/Object.hpp"

class CategoryButton : public ButtonWithSprite {
public:
	std::shared_ptr<Texture> _categoryTexture;

	CategoryButton(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> categoryTexture, sf::Vector2i position = sf::Vector2i(0, 0));
	~CategoryButton();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};

class Categories {
public:
	int _main_margin;
	int _top_margin;
	int _outer_margin;
	int _inner_margin;
	sf::IntRect _rect;

	std::unique_ptr<sf::Text> _text;
	
	std::shared_ptr<ButtonWithSprite> _prev, _next;
	std::vector<std::shared_ptr<CategoryButton>> _categories;
	std::shared_ptr<CategoryButton> _selectedCategory = nullptr;
	ObjectType _selectedType = ObjectType::None;

	Categories();
	~Categories();

	sf::Vector2i getPosition();
	sf::Vector2i getSize();
	void addCategory(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> categoryTexture, std::function<void()> function);
	void setCategory(std::shared_ptr<CategoryButton> button, ObjectType type);
	void setPosition(sf::Vector2i position);

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void update();
	void draw();
};