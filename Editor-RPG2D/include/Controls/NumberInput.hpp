#pragma once
#include "GUIManager.hpp"
#include "SFML//Graphics.hpp"
#include <functional>

enum class NumberInputState { Idle, Hover };
enum class NumberInputEditState { None, TextEntered, Selecting, Selected};

bool isInteger(std::wstring text);
std::wstring deleteStartZeros(std::wstring text);
std::wstring clamp(std::wstring numberStr, int min, int max);
std::wstring validateNumberStr(std::wstring numberStr, int min, int max);

class NumberInput : public Element {
public:

	sf::IntRect _rect;

	int _border = 2;
	int _marginLeft = 4;
	
	sf::Color _textEnteredColor;
	sf::Color _hoverColor;
	sf::Color _idleColor;
	sf::Color _outlineColor;

	int _characterSize;
	int _limitCharacters;
	int _minValue = INT_MIN;
	int _maxValue = INT_MAX;

	std::unique_ptr<sf::Text> _defaultText;
	std::wstring _textStr;
	std::unique_ptr<sf::Text> _text;

	NumberInputState _state;
	sf::Time _lastCLickTime;
	NumberInputEditState _editState;
	int _cursorPosition;
	int _selectionStart, _selectionEnd;

	std::function<void()> _onClickedFunction;
	std::function<void()> _onEditedFunction;
	std::function<void()> _onEnteredFunction;
	

	NumberInput(sf::Vector2i size, std::wstring defaultText, int limitCharacters, int characterSize);
	~NumberInput();
	
	void setColors(sf::Color textEnteredColor, sf::Color hoverColor, sf::Color idleColor, sf::Color outlineColor);
	void setPosition(sf::Vector2i position);
	void setMinValue(int minValue);
	void setMaxValue(int maxValue);
	void setRange(int min, int max);
	void setText(std::wstring text);
	void setValidatedText(std::wstring text);
	void setCursorOnEndText();
	void setLimitCharacters(int limitCharacters);
	int getNumber();
	sf::Vector2i getPosition();
	sf::Vector2i getSize();
	void positioningCursorByMouse();


	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();

};