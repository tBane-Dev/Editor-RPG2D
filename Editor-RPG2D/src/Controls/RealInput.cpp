#include "Controls/RealInput.hpp"
#include "Controls/NumberInput.hpp" // function deleteStartZeros()
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "Time.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include "Cursor.hpp"
#include "DebugLog.hpp"

bool haveDot(std::wstring text) {
	
	int dotCount = 0;
	for(auto c : text) {
		if (c == L'.')
			dotCount++;
	}

	return dotCount == 1;
}

bool isFloat(std::wstring text) {

	if (text.empty())
		return false;

	int start = 0;

	if (text.front() == L'-') {
		if (text.size() == 1)
			return false;

		start = 1;
	}

	bool dot = false;

	for (int i = start; i < text.size(); i++) {

		if (text[i] == L'.') {

			if (dot)
				return false;

			dot = true;
		}
		else if (!iswdigit(text[i])) {
			return false;
		}
	}

	return true;
}

std::wstring clamp(std::wstring numberStr, float min, float max) {

	if (!isFloat(numberStr))
		return std::to_wstring(min);

	try {
		float value = std::stof(numberStr);
		value = std::clamp(value, min, max);
		return std::to_wstring(value);
	}
	catch (...) {

		if (numberStr[0] == L'-') {
			std::wostringstream ss;
			ss << std::fixed << std::setprecision(1) << min;
			return ss.str();
		}
		else {
			std::wostringstream ss;
			ss << std::fixed << std::setprecision(1) << max;
			return ss.str();
		}
	}
}

RealInput::RealInput(sf::Vector2i size, std::wstring defaultText,int limitCharactersBeforeDot, int limitCharactersAfterDot, int characterSize) : Element() {
	
	_textEnteredColor = sf::Color(47, 47, 47);
	_hoverColor = sf::Color(63, 63, 63);
	_idleColor = sf::Color(23, 23, 23);
	_outlineColor = sf::Color(0, 0, 0);

	_rect = sf::IntRect(sf::Vector2i(0,0), size);
	
	_limitCharactersBeforeDot = limitCharactersBeforeDot;
	_limitCharactersAfterDot = limitCharactersAfterDot;

	_characterSize = characterSize;
	_textStr = L"";

	_text = std::make_unique<sf::Text>(basicFont, _textStr, _characterSize);
	_text->setFillColor(sf::Color(191, 191, 191));

	_defaultText = std::make_unique<sf::Text>(basicFont, defaultText, _characterSize);
	_defaultText->setFillColor(sf::Color(127, 127, 127));

	_marginLeft = ((float)_rect.size.y-_text->getFont().getLineSpacing(_characterSize)) / 2;

	_state = RealInputState::Idle;
	_lastCLickTime = sf::Time::Zero;
	_editState = RealInputEditState::None;

	_cursorPosition = 0;
	_selectionStart = -1;
	_selectionEnd = -1;

	_onEditedFunction = { };
	_onClickedFunction = { };
	_onEnteredFunction = { };

}

RealInput::~RealInput() {
	
}

void RealInput::setColors(sf::Color textEnteredColor, sf::Color hoverColor, sf::Color idleColor, sf::Color outlineColor) {
	_textEnteredColor = textEnteredColor;
	_hoverColor = hoverColor;
	_idleColor = idleColor;
	_outlineColor = outlineColor;
}

void RealInput::setPosition(sf::Vector2i position) {
	_rect.position = position;

	sf::Vector2f textPosition;
	textPosition.x = (float)_rect.position.x + (float)(_border) + (float)(_marginLeft);
	textPosition.y = (float)_rect.position.y + (float)(_border);
	textPosition.y += (float)(_rect.size.y-2 * _border - basicFont.getLineSpacing(_text->getCharacterSize())) / 2.f;

	_text->setPosition(textPosition);
	_defaultText->setPosition(textPosition);
}

void RealInput::setMinValue(float minValue) {
	_minValue = minValue;
}

void RealInput::setMaxValue(float maxValue) {
	_maxValue = maxValue;
}

void RealInput::setRange(float min, float max) {
	_minValue = min;
	_maxValue = max;
}
void RealInput::setText(std::wstring text) {

	_textStr = text;
	_text->setString(_textStr);
}

void RealInput::setValidatedText(std::wstring text) {
	_textStr = text;

	if (!_textStr.empty()) {
		_textStr = deleteStartZeros(_textStr);
		_textStr = clamp(_textStr, _minValue, _maxValue);

		std::wostringstream ss;
		ss << std::fixed << std::setprecision(_limitCharactersAfterDot) << std::stof(_textStr);
		_textStr = ss.str();
	}
	else {
		_textStr = L"0.200";
	}

	_cursorPosition = std::min(_cursorPosition, (int)_textStr.length());
	_text->setString(_textStr);
}

void RealInput::setCursorOnEndText() {
	_cursorPosition = (int)_textStr.length();
}

void RealInput::setLimitCharacters(int limitCharactersBeforeDot, int limitCharactersAfterDot) {

	_limitCharactersBeforeDot = limitCharactersBeforeDot;
	_limitCharactersAfterDot = limitCharactersAfterDot;

	int dot = _textStr.find(L'.');

	if (dot == std::wstring::npos) {

		_textStr = _textStr.substr(0, _limitCharactersBeforeDot);

		if (_cursorPosition > _textStr.size())
			_cursorPosition = _textStr.size();
	}
	else {

		std::wstring beforeDot = _textStr.substr(0, dot);
		std::wstring afterDot = _textStr.substr(dot + 1);

		beforeDot = beforeDot.substr(0, _limitCharactersBeforeDot);
		afterDot = afterDot.substr(0, _limitCharactersAfterDot);

		_textStr = beforeDot + L"." + afterDot;

		if (_cursorPosition > _textStr.size())
			_cursorPosition = _textStr.size();
	}

	_text->setString(_textStr);
}

float RealInput::getNumber() {
	if(_textStr.empty())
		return 0;

	std::wstring t = _textStr;
	t = deleteStartZeros(t);
	t = clamp(t, _minValue, _maxValue);

	return std::stof(t);
}

sf::Vector2i RealInput::getPosition() {
	return _rect.position;
}

sf::Vector2i RealInput::getSize() {
	return _rect.size;
}

void RealInput::positioningCursorByMouse() {

	int newCursorPosition = 0;

	for (int i = 0; i < (int)(_textStr.length()); i++) {
		sf::Vector2f charPos = _text->findCharacterPos(i);
		if (Cursors::cursor->_position.x > charPos.x) {
			newCursorPosition = i + 1;
		}
	}

	_cursorPosition = newCursorPosition;
}

void RealInput::cursorHover() {
	if (_rect.contains(Cursors::cursor->_position)) {
		GUI_manager->Element_hovered = this->shared_from_this();
		return;
	}

	if (_editState == RealInputEditState::Selecting) {
		GUI_manager->Element_hovered = this->shared_from_this();
		return;
	}

}

void RealInput::handleEvent(const sf::Event& event) {
	
	if (const auto* mp = event.getIf<sf::Event::MouseButtonPressed>(); mp) {
		if (_rect.contains(Cursors::cursor->_position)) {

			if (_editState == RealInputEditState::TextEntered) {
				if ((currentTime - _lastCLickTime).asSeconds() < 0.2f) {
					// double click
					_selectionStart = 0;
					_selectionEnd = (int)_textStr.length();
					_cursorPosition = _selectionEnd;
					_editState = RealInputEditState::Selected;
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
					_editState = RealInputEditState::Selected;
				}
				else {
					positioningCursorByMouse();
					_selectionStart = _cursorPosition;
					_selectionEnd = _cursorPosition;
					
					_editState = RealInputEditState::TextEntered;
					if (_onClickedFunction)
						_onClickedFunction();
				}
				
			}

			GUI_manager->Element_pressed = this->shared_from_this();
		}
		else if(_editState != RealInputEditState::None) {
			_editState = RealInputEditState::None;
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

		if(_editState == RealInputEditState::Selecting)
			_editState = RealInputEditState::Selected;

		return;
	}

	if (_editState == RealInputEditState::Selecting || _editState == RealInputEditState::Selected || _editState == RealInputEditState::TextEntered) {
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
					_editState = RealInputEditState::TextEntered;
					return;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && kp->code == sf::Keyboard::Key::C) {
				if(_editState == RealInputEditState::Selected || _editState == RealInputEditState::Selecting) {
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
				_editState = RealInputEditState::TextEntered;

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
				_editState = RealInputEditState::Selected;
				return;
			}


		}
	}

	if (_editState == RealInputEditState::Selecting || _editState == RealInputEditState::Selected || _editState == RealInputEditState::TextEntered) {
		if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && kp->code == sf::Keyboard::Key::Left) {
				
				if (_editState == RealInputEditState::None || _editState == RealInputEditState::TextEntered) {
					_editState = RealInputEditState::Selecting;
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
				if (_editState == RealInputEditState::None || _editState == RealInputEditState::TextEntered) {
					_editState = RealInputEditState::Selecting;
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

	if (_editState == RealInputEditState::Selecting || _editState == RealInputEditState::Selected || _editState == RealInputEditState::TextEntered) {
		
		if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp) {
			if (kp->code == sf::Keyboard::Key::Left) {
				
				if (_editState == RealInputEditState::Selecting || _editState == RealInputEditState::Selected) {
					_selectionStart = -1;
					_selectionEnd = -1;
					_editState = RealInputEditState::TextEntered;
				}
				else if (_cursorPosition > 0) {
					_cursorPosition -= 1;
				}
				return;
				
			}else if (kp->code == sf::Keyboard::Key::Right) {

				if (_editState == RealInputEditState::Selecting || _editState == RealInputEditState::Selected) {
					_selectionStart = -1;
					_selectionEnd = -1;
					_editState = RealInputEditState::TextEntered;
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

			if (_editState == RealInputEditState::TextEntered) {
				if (!(_selectionStart == -1 && _selectionEnd == -1) && _selectionEnd != _selectionStart) {
					_editState = RealInputEditState::Selecting;
					
				}
			}

			return;
		}
	}

	if (_editState == RealInputEditState::TextEntered || _editState == RealInputEditState::Selected || _editState == RealInputEditState::Selecting) {
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
							_editState = RealInputEditState::TextEntered;
							_cursorPosition = min;
							_selectionStart = -1;
							_selectionEnd = -1;
						}
						else if (_cursorPosition > 0 && _editState == RealInputEditState::TextEntered) {
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
				else if (character >= 32 && ((character >= '0' && character <= '9') || character == '.' )) {
					std::wstring c;
					c += character;
					
					if (_editState == RealInputEditState::TextEntered) {

						int lenBeforeDot = std::find(_textStr.begin(), _textStr.end(), L'.') - _textStr.begin();
						int lenAfterDot = (int)_textStr.length() - lenBeforeDot - 1;
						if ((isInteger(_textStr) && !haveDot(_textStr) && character == '.') ||
							(character != '.' && _cursorPosition <= lenBeforeDot && lenBeforeDot < _limitCharactersBeforeDot) ||
							(character != '.' && _cursorPosition > lenBeforeDot && lenAfterDot < _limitCharactersAfterDot)
						){
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
						_text->setString(_textStr);
						_editState = RealInputEditState::TextEntered;
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

void RealInput::update() {

	if (_editState == RealInputEditState::TextEntered) {
	
	}
	
	if (GUI_manager->Element_hovered.get() == this) {
		_state = RealInputState::Hover;
	}
	else {
		_state = RealInputState::Idle;
	}
}

void RealInput::draw() {
	// draw rect

	sf::Vector2f rectSize;
	rectSize.x = (float)(_rect.size.x - 2 * _border);
	rectSize.y = (float)(_rect.size.y - 2 * _border);
	sf::RectangleShape rect(rectSize);

	if (_editState == RealInputEditState::TextEntered) {
		rect.setFillColor(_textEnteredColor);
	}
	else if(_state == RealInputState::Hover) {
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
	if(_textStr.empty() && _editState != RealInputEditState::TextEntered) {
		Main::render_window->draw(*_defaultText);
	}
	else {
		Main::render_window->draw(*_text);
	}
	
	// draw cursor
	if (_editState != RealInputEditState::None && int(currentTime.asSeconds() * 3) % 2 == 0) {
		sf::RectangleShape cursor(sf::Vector2f(2, basicFont.getLineSpacing(_characterSize)));
		cursor.setFillColor(sf::Color::Red);
		cursor.setPosition(_text->findCharacterPos(_cursorPosition));
		Main::render_window->draw(cursor);
	}
		

}


