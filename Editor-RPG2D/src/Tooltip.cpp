#include "Tooltip.hpp"
#include "Time.hpp"
#include "Theme.hpp"
#include "Controls/Button.hpp"
#include "Cursor.hpp"
#include "DebugLog.hpp"

namespace Main {

	std::wstring wrap_text(int line_width, sf::Font& font, int characterSize, std::wstring text) {

		std::wstring wrapped_text = L"";

		std::wstring line = L"";
		std::wstring word = L"";

		for (auto& character : text) {

			std::shared_ptr<sf::Text> text_word = std::make_shared<sf::Text>(font, word + character, characterSize);

			if (text_word->getGlobalBounds().size.x > line_width) {

				if (line != L"") {
					wrapped_text += line + L"\n";
					line = L"";
				}

				// word longer than line
				std::wstring l = L"";
				word = word + character;
				for (wchar_t& c : word) {
					std::shared_ptr<sf::Text> text_line_plus_character = std::make_shared<sf::Text>(font, l + c, characterSize);
					if (text_line_plus_character->getGlobalBounds().size.x > line_width) {
						wrapped_text += l + L"\n";
						l = c;
					}
					else
						l = l + c;
				}

				std::shared_ptr<sf::Text> text_line = std::make_shared<sf::Text>(font, l, characterSize);
				wrapped_text += l + L"\n";

				word = L"";
				continue;
			}

			std::shared_ptr<sf::Text> text_line_plus_word_plus_character = std::make_shared<sf::Text>(font, line + word + character, characterSize);
			if (text_line_plus_word_plus_character->getGlobalBounds().size.x > line_width)
			{
				wrapped_text += line + L"\n";
				line = L"";
				word = word + character;
				continue;
			}

			if (character == L'\n') {

				std::shared_ptr<sf::Text> text_line_plus_word = std::make_shared<sf::Text>(font, line + word, characterSize);
				if (text_line_plus_word->getGlobalBounds().size.x > line_width) {
					wrapped_text += line + L"\n";
					wrapped_text += word + L"\n";

					line = L"";
					word = L"";
				}
				else {
					wrapped_text += line + word + L"\n";
					line = L"";
					word = L"";
				}

				continue;
			}

			if (character == L' ' || character == L'\t') {

				std::shared_ptr<sf::Text> text_line_plus_word = std::make_shared<sf::Text>(font, line + word, characterSize);

				if (text_line_plus_word->getGlobalBounds().size.x > line_width) {
					wrapped_text += line + L"\n";
					line = L"";
				}
				else {
					line = line + word + character;
				}

				word = L"";
			}
			else {
				word = word + character;
			}
		}

		if (line != L"" || word != L"") {
			wrapped_text += line + word;
		}

		return wrapped_text;

	}



	Tooltip::Tooltip() {
		_timer = sf::Time::Zero;

		_title = L"";
		_description = L"";

		_title_text = std::make_shared<sf::Text>(basicFont, _title, small_text_size);
		_title_text->setStyle(sf::Text::Bold);
		_title_text->setFillColor(small_text_color);

		_description_text = std::make_shared<sf::Text>(basicFont, _description, small_text_size);
		_description_text->setFillColor(small_text_color);
	}

	Tooltip::~Tooltip() {

	}

	void Tooltip::setElement(std::weak_ptr<Element> element, int width) {

		if (element.expired()) return;

		if (element.lock() == _element.lock()) return;
		
		_element = element;
		_timer = currentTime;

		_title = L"";
		_description = L"";

		if (std::dynamic_pointer_cast<Button>(_element.lock())) {
			std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(_element.lock());
			_title = button->_title;
			_description = wrap_text(width, basicFont, small_text_size, button->_description);
		}
		
		_title_text->setString(_title);
		_description_text->setString(_description);
	}

	float Tooltip::getHeightOfTitleAndDescription() {

		float height = 0.f;

		if (_title != L"") height += basicFont.getLineSpacing(small_text_size);

		for (wchar_t c : _title) {
			if (c == L'\n') height += basicFont.getLineSpacing(small_text_size);
		}

		if (_description != L"") height += basicFont.getLineSpacing(small_text_size);

		for (wchar_t c : _description) {
			if (c == L'\n') height += basicFont.getLineSpacing(small_text_size);
		}

		return height;

	}

	void Tooltip::update() {
		if (_element.expired()) return;


		if (std::dynamic_pointer_cast<Button>(_element.lock())) {
			if(!std::dynamic_pointer_cast<Button>(_element.lock())->_rect.contains(Main::cursor->_position)) {
				_element = std::weak_ptr<Element>();
			}
		}

	}

	void Tooltip::draw() {

		if (_element.expired()) return;

		if ((currentTime - _timer).asSeconds() > 0.5f && (_title != L"" || _description != L"")) {

			int padding = 4;

			sf::Vector2i rectSize;
			rectSize.x = (_title_text->getGlobalBounds().size.x > _description_text->getGlobalBounds().size.x) ? int(_title_text->getGlobalBounds().size.x) : int(_description_text->getGlobalBounds().size.x);
			rectSize.x += 2 * padding;

			if (_title != L"" && _description != L"")
				rectSize.y = int(getHeightOfTitleAndDescription() + 3 * padding);
			else
				rectSize.y = int(getHeightOfTitleAndDescription() + 2 * padding);


			rectSize.x = rectSize.x + padding;
			rectSize.y = rectSize.y + padding;

			sf::Vector2i pos;
			if (std::dynamic_pointer_cast<Button>(_element.lock())) {
				std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(_element.lock());
				pos.x = button->getPosition().x + button->getSize().x / 2;
				pos.y = button->getPosition().y + button->getSize().y;
			}

			int screenPadding = 16;
			pos.x = std::clamp(pos.x, screenPadding, int(Main::render_window->getSize().x - rectSize.x - screenPadding));
			pos.y = std::clamp(pos.y, screenPadding, int(Main::render_window->getSize().y - rectSize.y - screenPadding));

			sf::RectangleShape outerRectShape(sf::Vector2f(rectSize.x, rectSize.y));
			outerRectShape.setPosition(sf::Vector2f(pos.x, pos.y));
			outerRectShape.setFillColor(sf::Color(47, 47, 47));
			outerRectShape.setOutlineThickness(2.0f);
			outerRectShape.setOutlineColor(sf::Color(63, 63, 63));
			Main::render_window->draw(outerRectShape);


			if (_title != L"") {
				sf::Vector2i titlePos;
				titlePos.x = pos.x + padding;
				titlePos.y = pos.y + padding;
				_title_text->setPosition(sf::Vector2f(titlePos));
				Main::render_window->draw(*_title_text);
			}

			if (_description != L"") {
				sf::Vector2i descriptionPos;
				descriptionPos.x = pos.x + padding;
				descriptionPos.y = pos.y + padding;
				if (_title != L"") descriptionPos.y += (int)(basicFont.getLineSpacing(small_text_size)) + padding;
				_description_text->setPosition(sf::Vector2f(descriptionPos));
				Main::render_window->draw(*_description_text);
			}
		}
	}

	std::shared_ptr<Tooltip> tooltip;
}