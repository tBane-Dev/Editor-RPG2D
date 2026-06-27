#pragma once
#include "WindowsManager.hpp"
#include "Controls/Button.hpp"
#include "AnimationsManager.hpp"
#include "Theme.hpp"
#include "Controls/Scrollbar.hpp"
#include "Controls/ButtonWithTextAndSprite.hpp"
#include "Controls/TextInput.hpp"
#include "Controls/ButtonWithText.hpp"

class AnimationsListItem : public Button { 
public:
	
	std::unique_ptr<sf::Text> _name;
	std::unique_ptr<sf::Text> _animCount;
	std::unique_ptr<sf::Text> _framesCount;

	std::weak_ptr<Animations> _animations;
	
	AnimationsListItem(int offset, int otherButtonsWidth);
	~AnimationsListItem();

	void setAnimation(std::weak_ptr<Animations> animation);
	virtual void setPosition(sf::Vector2i position, int offset, int otherButtonsWidth);

	virtual void cursorHover();
	virtual void draw();
};

class AnimationsListDialog : public Main::Window {
public:

	int _padding;

	sf::IntRect _detailsRect;
	sf::IntRect _itemsRect;
	sf::IntRect _bottomRect;

	std::shared_ptr<sf::IntRect> _detailsInnerRect;
	std::shared_ptr<sf::IntRect> _itemsInnerRect;
	std::shared_ptr<sf::IntRect> _bottomInnerRect;

	
	std::shared_ptr<ButtonWithText> _name;
	int _offset;
	std::shared_ptr<ButtonWithText> _animCount;
	std::shared_ptr<ButtonWithText> _framesCount;

	std::vector<std::shared_ptr<AnimationsListItem>> _animationsItems;
	int _selectedItemIndex = -1;
	std::shared_ptr<Scrollbar> _scrollbar;
	
	std::unique_ptr<sf::Text> _fileNameText;
	std::shared_ptr<TextInput> _fileNameInput;
	std::shared_ptr<ButtonWithTextAndSprite> _confirmButton;
	std::shared_ptr<ButtonWithTextAndSprite> _cancelButton;


	AnimationsListDialog();
	~AnimationsListDialog();

	void createAnimationsDetails();
	void createAnimationsItems();
	void createBottom(std::function<void()> function);
	void setAnimationsItems();
	virtual void setPosition(sf::Vector2i position);
	sf::FloatRect getItemsRect();

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};