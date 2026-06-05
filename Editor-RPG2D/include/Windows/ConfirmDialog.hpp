#pragma once
#include "WindowsManager.hpp"
#include "Controls/ButtonWithTextSpriteAndIcon.hpp"

class ConfirmDialog : public Main::Window {
public:

	std::unique_ptr<sf::Text> _text;

	std::shared_ptr<ButtonWithTextSpriteAndIcon> _confirmButton;
	std::shared_ptr<ButtonWithTextSpriteAndIcon> _cancelButton;

	std::unique_ptr<sf::Text> _bottomComment;

	ConfirmDialog(std::wstring title, std::wstring text);
	~ConfirmDialog();

	virtual void setPosition(sf::Vector2i position);
	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};