#include "Windows/ConfirmDialog.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"

ConfirmDialog::ConfirmDialog(std::wstring title, std::wstring text) : Window(title) {
	
	_text = std::make_unique<sf::Text>(basicFont, text, small_text_size);
	_text->setFillColor(small_text_color);
	_text->setPosition(sf::Vector2f(getContentPosition()) + sf::Vector2f(2*padding, padding));
	
	float textWidth = _text->getGlobalBounds().size.x;
	float textHeight = _text->getGlobalBounds().size.y;

	int distance_between_buttons = 32;
	int buttonsWidth = 128 + distance_between_buttons + 128;

	int contentWidth = std::max(
		(int)textWidth + 4 * padding,
		buttonsWidth + 2 * padding
	);

	int contentHeight =
		padding + 16 +
		(int)textHeight +
		32 +
		48 +
		16 +
		16 +
		padding;

	setSize(sf::Vector2i(
		contentWidth + 2 * border,
		contentHeight + 2 * border + basic_text_rect_height
	));

	_confirmButton = std::make_shared<ButtonWithTextSpriteAndIcon>(
		L"Confirm", 
		textures_manager->getTexture(L"assets\\tex\\windows\\button.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\button_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\button_press.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\confirm.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\confirm_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\confirm_press.png")
	);

	_confirmButton->_onclick_func = [this]() {
		std::shared_ptr<ConfirmDialog> newDialog = std::make_shared<ConfirmDialog>(L"Test Confirm Window", L"Lorem ipsum dolor sit amet, consectetur\nadipiscing elit, sed do eiusmod tempor\nincididunt ut labore et dolore magna\naliqua.");
		newDialog->setPosition(_rect.position + sf::Vector2i(rand()%80 - 40, rand()%80 - 40));
		Main::windows_manager->push_back(newDialog);
		
		};

	_cancelButton = std::make_shared<ButtonWithTextSpriteAndIcon>(
		L"Cancel",
		textures_manager->getTexture(L"assets\\tex\\windows\\button.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\button_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\button_press.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\cancel.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\cancel_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\cancel_press.png")
	);

	_cancelButton->_onclick_func = []() { Main::windows_manager->pop_back(); };

	_bottomComment = std::make_unique<sf::Text>(basicFont, L"Enter - Confirm, Esc - Cancel", 12);
	_bottomComment->setFillColor(small_text_color);
	_bottomComment->setPosition(sf::Vector2f(getContentPosition()) + sf::Vector2f(padding, getContentSize().y - 16 - padding));

	setPosition(sf::Vector2i(200, 200));
}

ConfirmDialog::~ConfirmDialog() {

}

void ConfirmDialog::setPosition(sf::Vector2i position) {
	
	Window::setPosition(position);

	_text->setPosition(sf::Vector2f(getContentPosition()) + sf::Vector2f(2*padding, padding+16));

	int distance_between_buttons = 32;
	sf::Vector2i startPos;
	startPos.x = getContentPosition().x + getContentSize().x/2 - 128 - distance_between_buttons/2;
	startPos.y = getContentPosition().y + getContentSize().y - 48 - 40;

	if(_confirmButton)
		_confirmButton->setPosition(startPos);
	
	if(_cancelButton)
		_cancelButton->setPosition(startPos + sf::Vector2i(128 + distance_between_buttons, 0));

	if(_bottomComment)
		_bottomComment->setPosition(sf::Vector2f(getContentPosition()) + sf::Vector2f((getContentSize().x-_bottomComment->getGlobalBounds().size.x)/2, getContentSize().y - 16 - padding));
}

void ConfirmDialog::cursorHover() {
	Window::cursorHover();

	_confirmButton->cursorHover();
	_cancelButton->cursorHover();
}

void ConfirmDialog::handleEvent(const sf::Event& event) {
	Window::handleEvent(event);

	if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp && kp->code == sf::Keyboard::Key::Escape) {
		_cancelButton->click();
		return;
	}

	if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp && kp->code == sf::Keyboard::Key::Enter) {
		_confirmButton->click();
		return;
	}

	_confirmButton->handleEvent(event);
	_cancelButton->handleEvent(event);
}

void ConfirmDialog::update() {
	Window::update();

	_confirmButton->update();
	_cancelButton->update();
}

void ConfirmDialog::draw() {
	Window::draw();

	Main::render_window->draw(*_text);

	_confirmButton->draw();
	_cancelButton->draw();

	Main::render_window->draw(*_bottomComment);
}