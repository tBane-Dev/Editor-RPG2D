#include "Controls/TextInput.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "Time.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include "Cursor.hpp"
#include "DebugLog.hpp"

TextInput::TextInput(sf::Vector2i size, std::wstring defaultText,int limitCharacters, int characterSize) : Element() {
	
	_rect = sf::IntRect(sf::Vector2i(0,0), size);
	
	_limitCharacters = limitCharacters;
	_characterSize = characterSize;
	_textStr = L"";

	_text = std::make_unique<sf::Text>(basicFont, _textStr, _characterSize);
	_text->setFillColor(sf::Color(191, 191, 191));

	_defaultText = std::make_unique<sf::Text>(basicFont, defaultText, _characterSize);
	_defaultText->setFillColor(sf::Color(127, 127, 127));

	_marginLeft = ((float)_rect.size.y-_text->getFont().getLineSpacing(_characterSize)) / 2;

	_state = TextInputState::Idle;
	_lastCLickTime = sf::Time::Zero;
	_editState = TextInputEditState::None;

	_cursorPosition = 0;
	_selectionStart = -1;
	_selectionEnd = -1;

	_onEditedFunction = { };
	_onClickedFunction = { };
	_onEnteredFunction = { };

}

TextInput::~TextInput() {
	
}

void TextInput::setPosition(sf::Vector2i position) {
	_rect.position = position;

	sf::Vector2f textPosition;
	textPosition.x = (float)_rect.position.x + (float)(_border) + (float)(_marginLeft);
	textPosition.y = (float)_rect.position.y + (float)(_border);
	textPosition.y += (float)(_rect.size.y-2 * _border - basicFont.getLineSpacing(_text->getCharacterSize())) / 2.f;

	_text->setPosition(textPosition);
	_defaultText->setPosition(textPosition);
}

void TextInput::setText(std::wstring text) {
	_textStr = text.substr(0, _limitCharacters);
	_text->setString(_textStr);
}

void TextInput::setCursorOnEndText() {
	_cursorPosition = (int)_textStr.length();
}

void TextInput::setLimitCharacters(int limitCharacters) {
	_limitCharacters = limitCharacters;
	_textStr = _textStr.substr(0,_limitCharacters);

	if(_cursorPosition > limitCharacters)
		_cursorPosition = _limitCharacters;

	_text->setString(_textStr.substr(0, _limitCharacters));
}

std::wstring TextInput::getText() {
	return _textStr;
}

sf::Vector2i TextInput::getPosition() {
	return _rect.position;
}

sf::Vector2i TextInput::getSize() {
	return _rect.size;
}

void TextInput::positioningCursorByMouse() {

	int newCursorPosition = 0;

	for (int i = 0; i < (int)(_textStr.length()); i++) {
		sf::Vector2f charPos = _text->findCharacterPos(i);
		if (cursor->_position.x > charPos.x) {
			newCursorPosition = i + 1;
		}
	}

	_cursorPosition = newCursorPosition;
}

void TextInput::cursorHover() {
	if (_rect.contains(cursor->_position)) {
		GUI_manager->Element_hovered = this->shared_from_this();
		return;
	}

	if (_editState == TextInputEditState::Selecting) {
		GUI_manager->Element_hovered = this->shared_from_this();
		return;
	}

}

void TextInput::handleEvent(const sf::Event& event) {
	
	if (const auto* mp = event.getIf<sf::Event::MouseButtonPressed>(); mp) {
		if (_rect.contains(cursor->_position)) {

			if (_editState == TextInputEditState::TextEntered) {
				if ((currentTime - _lastCLickTime).asSeconds() < 0.2f) {
					// double click
					_selectionStart = 0;
					_selectionEnd = (int)_textStr.length();
					_cursorPosition = _selectionEnd;
					_editState = TextInputEditState::Selected;
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
					_editState = TextInputEditState::Selected;
				}
				else {
					positioningCursorByMouse();
					_selectionStart = _cursorPosition;
					_selectionEnd = _cursorPosition;
					
					_editState = TextInputEditState::TextEntered;
					if (_onClickedFunction)
						_onClickedFunction();
				}
				
			}

			GUI_manager->Element_pressed = this->shared_from_this();
		}
		else {
			_editState = TextInputEditState::None;
			_selectionStart = -1;
			_selectionEnd = -1;
			_lastCLickTime = currentTime;

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

		if(_editState == TextInputEditState::Selecting)
			_editState = TextInputEditState::Selected;

		return;
	}

	if (_editState == TextInputEditState::Selecting || _editState == TextInputEditState::Selected || _editState == TextInputEditState::TextEntered) {
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

					setText(_textStr);
					_editState = TextInputEditState::TextEntered;
					return;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && kp->code == sf::Keyboard::Key::C) {
				if(_editState == TextInputEditState::Selected || _editState == TextInputEditState::Selecting) {
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
				_editState = TextInputEditState::TextEntered;

				setText(_textStr);
				return;
			}
			
			// TO-DO - noAlt function because event active when Alt+A
			bool noAlt = !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RAlt);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && noAlt && kp->code == sf::Keyboard::Key::A) {
				_selectionStart = 0;
				_selectionEnd = (int)_textStr.length();
				_cursorPosition = _selectionEnd;
				_editState = TextInputEditState::Selected;
				return;
			}


		}
	}

	if (_editState == TextInputEditState::Selecting || _editState == TextInputEditState::Selected || _editState == TextInputEditState::TextEntered) {
		if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && kp->code == sf::Keyboard::Key::Left) {
				
				if (_editState == TextInputEditState::None || _editState == TextInputEditState::TextEntered) {
					_editState = TextInputEditState::Selecting;
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
				if (_editState == TextInputEditState::None || _editState == TextInputEditState::TextEntered) {
					_editState = TextInputEditState::Selecting;
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

	if (_editState == TextInputEditState::Selecting || _editState == TextInputEditState::Selected || _editState == TextInputEditState::TextEntered) {
		
		if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp) {
			if (kp->code == sf::Keyboard::Key::Left) {
				
				if (_editState == TextInputEditState::Selecting || _editState == TextInputEditState::Selected) {
					_selectionStart = -1;
					_selectionEnd = -1;
					_editState = TextInputEditState::TextEntered;
				}
				else if (_cursorPosition > 0) {
					_cursorPosition -= 1;
				}
				return;
				
			}else if (kp->code == sf::Keyboard::Key::Right) {

				if (_editState == TextInputEditState::Selecting || _editState == TextInputEditState::Selected) {
					_selectionStart = -1;
					_selectionEnd = -1;
					_editState = TextInputEditState::TextEntered;
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

			if (_editState == TextInputEditState::TextEntered) {
				if (!(_selectionStart == -1 && _selectionEnd == -1) && _selectionEnd != _selectionStart) {
					_editState = TextInputEditState::Selecting;
					
				}
			}

			return;
		}
	}

	if (_editState == TextInputEditState::TextEntered || _editState == TextInputEditState::Selected || _editState == TextInputEditState::Selecting) {
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
				if (_onEnteredFunction)
					_onEnteredFunction();
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
							_editState = TextInputEditState::TextEntered;
							_cursorPosition = min;
							_selectionStart = -1;
							_selectionEnd = -1;
						}
						else if (_cursorPosition > 0 && _editState == TextInputEditState::TextEntered) {
							_textStr.erase(_cursorPosition - 1, 1);
							setText(_textStr);
							_cursorPosition -= 1;

						}
						
						if (_onEditedFunction)
							_onEditedFunction();
					
					}
					return;
				}
				else if (character == 13 || character == 10) {
					// ENTER
					return;
				}
				else if (character >= 32) {
					std::wstring c;
					c += character;
					
					if (_editState == TextInputEditState::TextEntered) {
						if ((int)_textStr.length() < _limitCharacters) {
							_textStr.insert(_cursorPosition, c);
							_cursorPosition += 1;
							setText(_textStr);
						}
					}
					else {
						int min = std::min(_selectionStart, _selectionEnd);
						int max = std::max(_selectionStart, _selectionEnd);
						_textStr.erase(min, max - min);
						_textStr.insert(min, c);
						_text->setString(_textStr.substr(0, _limitCharacters));
						_editState = TextInputEditState::TextEntered;
						_cursorPosition = min + 1;
						_selectionStart = -1;
						_selectionEnd = -1;
					}

					if (_onEditedFunction)
						_onEditedFunction();

				}
			}
			

	}
	
}

void TextInput::update() {

	if (_editState == TextInputEditState::TextEntered) {
	
	}
	
	if (GUI_manager->Element_hovered.get() == this) {
		_state = TextInputState::Hover;
	}
	else {
		_state = TextInputState::Idle;
	}
}

void TextInput::draw() {
	// draw rect

	sf::Vector2f rectSize;
	rectSize.x = (float)(_rect.size.x - 2 * _border);
	rectSize.y = (float)(_rect.size.y - 2 * _border);
	sf::RectangleShape rect(rectSize);

	if (_editState == TextInputEditState::TextEntered) {
		rect.setFillColor(sf::Color(47,47,47));
	}
	else if(_state == TextInputState::Hover) {
		rect.setFillColor(sf::Color(63,63,63));
	}
	else {
		rect.setFillColor(sf::Color(23,23,23));
	}
	
	rect.setOutlineThickness((float)_border);
	rect.setOutlineColor(sf::Color(0,0,0));

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
	if(_textStr.empty() && _editState != TextInputEditState::TextEntered) {
		Main::render_window->draw(*_defaultText);
	}
	else {
		Main::render_window->draw(*_text);
	}
	

	
	// draw cursor
	if (_editState != TextInputEditState::None && int(currentTime.asSeconds() * 3) % 2 == 0) {
		sf::RectangleShape cursor(sf::Vector2f(2, basicFont.getLineSpacing(_characterSize)));
		cursor.setFillColor(sf::Color::Red);
		cursor.setPosition(_text->findCharacterPos(_cursorPosition));
		Main::render_window->draw(cursor);
	}
		

}


