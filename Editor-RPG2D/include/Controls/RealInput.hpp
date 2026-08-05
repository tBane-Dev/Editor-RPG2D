#pragma once
#include "GUIManager.hpp"
#include "SFML//Graphics.hpp"
#include <functional>

enum class RealInputState { Idle, Hover };
enum class RealInputEditState { None, TextEntered, Selecting, Selected};

bool haveDot(std::wstring text);
bool isFloat(std::wstring text);
std::wstring clamp(std::wstring numberStr, float min, float max);

class RealInput : public Element {
public:

	sf::IntRect _rect;

	int _border = 2;
	int _marginLeft = 4;
	
	sf::Color _textEnteredColor;
	sf::Color _hoverColor;
	sf::Color _idleColor;
	sf::Color _outlineColor;

	int _characterSize;
	int _limitCharactersBeforeDot;
	int _limitCharactersAfterDot;
	float _minValue = std::numeric_limits<float>::lowest();
	float _maxValue = std::numeric_limits<float>::max();

	std::unique_ptr<sf::Text> _defaultText;
	std::wstring _textStr;
	std::unique_ptr<sf::Text> _text;

	RealInputState _state;
	sf::Time _lastCLickTime;
	RealInputEditState _editState;
	int _cursorPosition;
	int _selectionStart, _selectionEnd;

	std::function<void()> _onClickedFunction;
	std::function<void()> _onEditedFunction;
	std::function<void()> _onEnteredFunction;
	

	RealInput(sf::Vector2i size, std::wstring defaultText, int limitCharactersBeforeDot, int limitCharactersAdterDot, int characterSize);
	~RealInput();
	
	void setColors(sf::Color textEnteredColor, sf::Color hoverColor, sf::Color idleColor, sf::Color outlineColor);
	void setPosition(sf::Vector2i position);
	void setMinValue(float minValue);
	void setMaxValue(float maxValue);
	void setRange(float min, float max);
	void setText(std::wstring text);
	void setValidatedText(std::wstring text);
	void setCursorOnEndText();
	void setLimitCharacters(int limitCharactersBeforeDot, int limitCharactersAfterDot);
	float getNumber();
	sf::Vector2i getPosition();
	sf::Vector2i getSize();
	void positioningCursorByMouse();


	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();

};