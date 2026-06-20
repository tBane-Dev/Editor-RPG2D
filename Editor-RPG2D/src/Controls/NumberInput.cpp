#include "Controls/NumberInput.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "Time.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include "Cursor.hpp"
#include "DebugLog.hpp"

bool isInteger(std::wstring text) {

	if (text.empty())
		return false;

	int start = 0;

	if (text.front() == L'-') {
		if (text.size() == 1)
			return false;

		start = 1;
	}

	for (int i = start; i < text.size(); i++) {
		if (!iswdigit(text[i]))
			return false;
	}

	return true;
}

std::wstring deleteStartZeros(std::wstring text) {

	if (text.empty())
		return text;

	bool negative = false;

	if (text[0] == L'-') {
		negative = true;
		text.erase(0, 1);
	}

	while (text.size() > 1 && text[0] == L'0')
		text.erase(0, 1);

	if (negative)
		text.insert(0, 1, L'-');

	if (text == L"-0")
		text = L"0";

	return text;
}

std::wstring clamp(std::wstring numberStr, int min, int max) {

	if (!isInteger(numberStr))
		return std::to_wstring(min);

	try {
		int value = std::stoi(numberStr);
		value = std::clamp(value, min, max);
		return std::to_wstring(value);
	}
	catch (...) {
		return (numberStr[0] == L'-')
			? std::to_wstring(min)
			: std::to_wstring(max);
	}
}

NumberInput::NumberInput(sf::Vector2i size, std::wstring defaultText,int limitCharacters, int characterSize) : Element() {
	
	_textEnteredColor = sf::Color(47, 47, 47);
	_hoverColor = sf::Color(63, 63, 63);
	_idleColor = sf::Color(23, 23, 23);
	_outlineColor = sf::Color(0, 0, 0);

	_rect = sf::IntRect(sf::Vector2i(0,0), size);
	
	_limitCharacters = limitCharacters;
	_characterSize = characterSize;
	_textStr = L"";

	_text = std::make_unique<sf::Text>(basicFont, _textStr, _characterSize);
	_text->setFillColor(sf::Color(191, 191, 191));

	_defaultText = std::make_unique<sf::Text>(basicFont, defaultText, _characterSize);
	_defaultText->setFillColor(sf::Color(127, 127, 127));

	_marginLeft = ((float)_rect.size.y-_text->getFont().getLineSpacing(_characterSize)) / 2;

	_state = NumberInputState::Idle;
	_lastCLickTime = sf::Time::Zero;
	_editState = NumberInputEditState::None;

	_cursorPosition = 0;
	_selectionStart = -1;
	_selectionEnd = -1;

	_onEditedFunction = { };
	_onClickedFunction = { };
	_onEnteredFunction = { };

}

NumberInput::~NumberInput() {
	
}

void NumberInput::setColors(sf::Color textEnteredColor, sf::Color hoverColor, sf::Color idleColor, sf::Color outlineColor) {
	_textEnteredColor = textEnteredColor;
	_hoverColor = hoverColor;
	_idleColor = idleColor;
	_outlineColor = outlineColor;
}

void NumberInput::setPosition(sf::Vector2i position) {
	_rect.position = position;

	sf::Vector2f textPosition;
	textPosition.x = (float)_rect.position.x + (float)(_border) + (float)(_marginLeft);
	textPosition.y = (float)_rect.position.y + (float)(_border);
	textPosition.y += (float)(_rect.size.y-2 * _border - basicFont.getLineSpacing(_text->getCharacterSize())) / 2.f;

	_text->setPosition(textPosition);
	_defaultText->setPosition(textPosition);
}

void NumberInput::setMinValue(int minValue) {
	_minValue = minValue;
}

void NumberInput::setMaxValue(int maxValue) {
	_maxValue = maxValue;
}

void NumberInput::setRange(int min, int max) {
	_minValue = min;
	_maxValue = max;
}
void NumberInput::setText(std::wstring text) {

	_textStr = text;
	_text->setString(_textStr);
}

void NumberInput::setValidatedText(std::wstring text) {
	_textStr = text;

	if (!_textStr.empty()) {
		_textStr = deleteStartZeros(_textStr);
		_textStr = clamp(_textStr, _minValue, _maxValue);
	}
	else {
		_textStr = L"0";
	}

	_cursorPosition = std::min(_cursorPosition, (int)_textStr.length());
	_text->setString(_textStr);
}

void NumberInput::setCursorOnEndText() {
	_cursorPosition = (int)_textStr.length();
}

void NumberInput::setLimitCharacters(int limitCharacters) {
	_limitCharacters = limitCharacters;
	_textStr = _textStr.substr(0,_limitCharacters);

	if(_cursorPosition > limitCharacters)
		_cursorPosition = _limitCharacters;

	_text->setString(_textStr.substr(0, _limitCharacters));
}

int NumberInput::getNumber() {
	if(_textStr.empty())
		return 0;

	std::wstring t = _textStr;
	t = deleteStartZeros(t);
	t = clamp(t, _minValue, _maxValue);
	return std::stoi(t);
}

sf::Vector2i NumberInput::getPosition() {
	return _rect.position;
}

sf::Vector2i NumberInput::getSize() {
	return _rect.size;
}

void NumberInput::positioningCursorByMouse() {

	int newCursorPosition = 0;

	for (int i = 0; i < (int)(_textStr.length()); i++) {
		sf::Vector2f charPos = _text->findCharacterPos(i);
		if (Main::cursor->_position.x > charPos.x) {
			newCursorPosition = i + 1;
		}
	}

	_cursorPosition = newCursorPosition;
}

void NumberInput::cursorHover() {
	if (_rect.contains(Main::cursor->_position)) {
		GUI_manager->Element_hovered = this->shared_from_this();
		return;
	}

	if (_editState == NumberInputEditState::Selecting) {
		GUI_manager->Element_hovered = this->shared_from_this();
		return;
	}

}

void NumberInput::handleEvent(const sf::Event& event) {
	
	if (const auto* mp = event.getIf<sf::Event::MouseButtonPressed>(); mp) {
		if (_rect.contains(Main::cursor->_position)) {

			if (_editState == NumberInputEditState::TextEntered) {
				if ((currentTime - _lastCLickTime).asSeconds() < 0.2f) {
					// double click
					_selectionStart = 0;
					_selectionEnd = (int)_textStr.length();
					_cursorPosition = _selectionEnd;
					_editState = NumberInputEditState::Selected;
				}
				else {
					positioningCursorByMouse();
					_selectionStart = _cursorPosition;
					_selectionEnd = _cursorPosition;

				}
			}
			else {

				if ((currentTime - _lastCLickTime).asSeconds() < 0.2f) {
					// double click
					_selectionStart = 0;
					_selectionEnd = (int)_textStr.length();
					_cursorPosition = _selectionEnd;
					_editState = NumberInputEditState::Selected;
				}
				else {
					positioningCursorByMouse();
					_selectionStart = _cursorPosition;
					_selectionEnd = _cursorPosition;
					
					_editState = NumberInputEditState::TextEntered;
					if (_onClickedFunction)
						_onClickedFunction();
				}
				
			}

			GUI_manager->Element_pressed = this->shared_from_this();
		}
		else if(_editState != NumberInputEditState::None) {
			_editState = NumberInputEditState::None;
			_selectionStart = -1;
			_selectionEnd = -1;
			_lastCLickTime = currentTime;

			setValidatedText(_textStr);
			_cursorPosition = std::min(_cursorPosition, (int)_textStr.length());

			if(GUI_manager->Element_pressed.get() == this)
				GUI_manager->Element_pressed = nullptr;
		}
		_lastCLickTime = currentTime;
		return;
	}

	if(const auto* mr = event.getIf<sf::Event::MouseButtonReleased>(); mr) {
		if (GUI_manager->Element_pressed.get() == this) {
			GUI_manager->Element_pressed = nullptr;
		}

		if(_editState == NumberInputEditState::Selecting)
			_editState = NumberInputEditState::Selected;

		return;
	}

	if (_editState == NumberInputEditState::Selecting || _editState == NumberInputEditState::Selected || _editState == NumberInputEditState::TextEntered) {
		if(const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && kp->code == sf::Keyboard::Key::X) {
				if (_selectionStart != -1 && _selectionEnd != -1 && _selectionStart != _selectionEnd) {
					int min = std::min(_selectionStart, _selectionEnd);
					int len = std::abs(_selectionEnd - _selectionStart);

					sf::Clipboard::setString(_textStr.substr(min, len));
					_textStr.erase(min, len);
					_cursorPosition = min;

					_selectionStart = -1;
					_selectionEnd = -1;

					_textStr = deleteStartZeros(_textStr);
					_cursorPosition = std::min(_cursorPosition, (int)_textStr.length());
					setText(_textStr);
					_editState = NumberInputEditState::TextEntered;
					return;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && kp->code == sf::Keyboard::Key::C) {
				if(_editState == NumberInputEditState::Selected || _editState == NumberInputEditState::Selecting) {
					sf::Clipboard::setString(_textStr.substr(std::min(_selectionStart, _selectionEnd), std::abs(_selectionEnd - _selectionStart)));
					return;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && kp->code == sf::Keyboard::Key::V) {
				if (_selectionStart != -1 && _selectionEnd != -1 && _selectionStart != _selectionEnd) {
					int min = std::min(_selectionStart, _selectionEnd);
					int max = std::max(_selectionStart, _selectionEnd);
					_textStr.erase(min, max - min);
					_cursorPosition = min;
				}
				_textStr.insert(_cursorPosition, sf::Clipboard::getString().toWideString());
				_cursorPosition += (int)sf::Clipboard::getString().getSize();

				_selectionStart = -1;
				_selectionEnd = -1;
				_editState = NumberInputEditState::TextEntered;

				_textStr = deleteStartZeros(_textStr);
				_cursorPosition = std::min(_cursorPosition, (int)_textStr.length());
				setText(_textStr);
				return;
			}
			
			// TO-DO - noAlt function because event active when Alt+A
			bool noAlt = !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RAlt);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && noAlt && kp->code == sf::Keyboard::Key::A) {
				_selectionStart = 0;
				_selectionEnd = (int)_textStr.length();
				_cursorPosition = _selectionEnd;
				_editState = NumberInputEditState::Selected;
				return;
			}


		}
	}

	if (_editState == NumberInputEditState::Selecting || _editState == NumberInputEditState::Selected || _editState == NumberInputEditState::TextEntered) {
		if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && kp->code == sf::Keyboard::Key::Left) {
				
				if (_editState == NumberInputEditState::None || _editState == NumberInputEditState::TextEntered) {
					_editState = NumberInputEditState::Selecting;
					_selectionStart = _cursorPosition;
					_selectionEnd = _cursorPosition;
				}
				if (_cursorPosition > 0) {
					_cursorPosition -= 1;
					_selectionEnd = _cursorPosition;
				}
				return;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && kp->code == sf::Keyboard::Key::Right) {
				if (_editState == NumberInputEditState::None || _editState == NumberInputEditState::TextEntered) {
					_editState = NumberInputEditState::Selecting;
					_selectionStart = _cursorPosition;
					_selectionEnd = _cursorPosition;
				}

				if(_cursorPosition < _textStr.length()) {
					_cursorPosition += 1;
					_selectionEnd = _cursorPosition;
				}
				return;
			}
		}
	}

	if (_editState == NumberInputEditState::Selecting || _editState == NumberInputEditState::Selected || _editState == NumberInputEditState::TextEntered) {
		
		if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp) {
			if (kp->code == sf::Keyboard::Key::Left) {
				
				if (_editState == NumberInputEditState::Selecting || _editState == NumberInputEditState::Selected) {
					_selectionStart = -1;
					_selectionEnd = -1;
					_editState = NumberInputEditState::TextEntered;
				}
				else if (_cursorPosition > 0) {
					_cursorPosition -= 1;
				}
				return;
				
			}else if (kp->code == sf::Keyboard::Key::Right) {

				if (_editState == NumberInputEditState::Selecting || _editState == NumberInputEditState::Selected) {
					_selectionStart = -1;
					_selectionEnd = -1;
					_editState = NumberInputEditState::TextEntered;
				}
				else if (_cursorPosition < _textStr.length()) {
					_cursorPosition += 1;
				}
				return;

				

			}
		}
		
		if (const auto* mm = event.getIf<sf::Event::MouseMoved>(); mm && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

			positioningCursorByMouse();
			_selectionEnd = _cursorPosition;

			if (_editState == NumberInputEditState::TextEntered) {
				if (!(_selectionStart == -1 && _selectionEnd == -1) && _selectionEnd != _selectionStart) {
					_editState = NumberInputEditState::Selecting;
					
				}
			}

			return;
		}
	}

	if (_editState == NumberInputEditState::TextEntered || _editState == NumberInputEditState::Selected || _editState == NumberInputEditState::Selecting) {
		if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp) {

			if (kp->code == sf::Keyboard::Key::Left) {
				if (_cursorPosition > 0) {
					_cursorPosition -= 1;
				}
				
			}
			else if (kp->code == sf::Keyboard::Key::Right) {
				if (_cursorPosition < _textStr.length()) {
					_cursorPosition += 1;
				}
			}
			else if (kp->code == sf::Keyboard::Key::Enter) {
				_textStr = deleteStartZeros(_textStr);
				_cursorPosition = std::min(_cursorPosition, (int)_textStr.length());
				setValidatedText(_textStr);
				if (_onEnteredFunction) {
					_onEnteredFunction();
				}
					
			}
			return;
		}
		else if (const auto* te = event.getIf<sf::Event::TextEntered>(); te) {

				wchar_t character = (wchar_t)te->unicode;

				if (character == 8) {
					// BACKSPACE
					if (!_textStr.empty()) {

						if (_selectionStart != -1 && _selectionEnd != -1 && _selectionStart != _selectionEnd) {
							int min = std::min(_selectionStart, _selectionEnd);
							int max = std::max(_selectionStart, _selectionEnd);
							_textStr.erase(min, max - min);
							setText(_textStr);
							_editState = NumberInputEditState::TextEntered;
							_cursorPosition = min;
							_selectionStart = -1;
							_selectionEnd = -1;
						}
						else if (_cursorPosition > 0 && _editState == NumberInputEditState::TextEntered) {
							_textStr.erase(_cursorPosition - 1, 1);
							setText(_textStr);
							_cursorPosition -= 1;

						}
						
						if (_onEditedFunction) {
							_onEditedFunction();
						}
							
					
					}
					return;
				}
				else if (character == 13 || character == 10) {
					// ENTER
					return;
				}
				else if (character >= 32 && character >= '0' && character <= '9') {
					std::wstring c;
					c += character;
					
					if (_editState == NumberInputEditState::TextEntered) {
						if (_limitCharacters == -1 || (int)_textStr.length() < _limitCharacters) {
							_textStr.insert(_cursorPosition, c);
							_cursorPosition += 1;
							setText(_textStr);
						}
					}
					else {
						// if text is selected, replace it with the new character
						int min = std::min(_selectionStart, _selectionEnd);
						int max = std::max(_selectionStart, _selectionEnd);
						_textStr.erase(min, max - min);
						_textStr.insert(min, c);
						_text->setString(_textStr.substr(0, (_limitCharacters==-1)? _textStr.length() : _limitCharacters));
						_editState = NumberInputEditState::TextEntered;
						_cursorPosition = min + 1;
						_selectionStart = -1;
						_selectionEnd = -1;
					}

					if (_onEditedFunction) {
						_onEditedFunction();
					}
						

				}
			}
			

	}
	
}

void NumberInput::update() {

	if (_editState == NumberInputEditState::TextEntered) {
	
	}
	
	if (GUI_manager->Element_hovered.get() == this) {
		_state = NumberInputState::Hover;
	}
	else {
		_state = NumberInputState::Idle;
	}
}

void NumberInput::draw() {
	// draw rect

	sf::Vector2f rectSize;
	rectSize.x = (float)(_rect.size.x - 2 * _border);
	rectSize.y = (float)(_rect.size.y - 2 * _border);
	sf::RectangleShape rect(rectSize);

	if (_editState == NumberInputEditState::TextEntered) {
		rect.setFillColor(_textEnteredColor);
	}
	else if(_state == NumberInputState::Hover) {
		rect.setFillColor(_hoverColor);
	}
	else {
		rect.setFillColor(_idleColor);
	}
	
	rect.setOutlineThickness((float)_border);
	rect.setOutlineColor(_outlineColor);

	sf::Vector2f rectPosition;
	rectPosition.x = (float)(_rect.position.x + _border);
	rectPosition.y = (float)(_rect.position.y + _border);
	rect.setPosition(rectPosition);

	Main::render_window->draw(rect);

	if (!(_selectionStart == -1 && _selectionEnd == -1) && _selectionStart != _selectionEnd) {

		int selection_margin = 1;

		int min = std::min(_selectionStart, _selectionEnd);
		int max = std::max(_selectionStart, _selectionEnd);

		sf::Vector2f selectionRectSize;
		selectionRectSize.x = _text->findCharacterPos(max).x - _text->findCharacterPos(min).x;
		selectionRectSize.y = (float)(_rect.size.y - 2 * _border - 2* selection_margin);

		sf::Vector2f selectionRectPosition;
		selectionRectPosition.x = _text->findCharacterPos(min).x;
		selectionRectPosition.y = (float)_rect.position.y + (float)_border + selection_margin;
		
		sf::RectangleShape selectionRect(selectionRectSize);

		selectionRect.setPosition(selectionRectPosition);
		selectionRect.setFillColor(sf::Color(31, 31, 127, 255));

		Main::render_window->draw(selectionRect);

	}

	// draw text
	if(_textStr.empty() && _editState != NumberInputEditState::TextEntered) {
		Main::render_window->draw(*_defaultText);
	}
	else {
		Main::render_window->draw(*_text);
	}
	
	// draw cursor
	if (_editState != NumberInputEditState::None && int(currentTime.asSeconds() * 3) % 2 == 0) {
		sf::RectangleShape cursor(sf::Vector2f(2, basicFont.getLineSpacing(_characterSize)));
		cursor.setFillColor(sf::Color::Red);
		cursor.setPosition(_text->findCharacterPos(_cursorPosition));
		Main::render_window->draw(cursor);
	}
		

}


