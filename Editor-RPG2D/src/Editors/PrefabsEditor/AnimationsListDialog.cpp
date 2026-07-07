#include "Editors/PrefabsEditor/AnimationsListDialog.hpp"
#include "RenderWindow.hpp"
#include "Cursor.hpp"

AnimationsListItem::AnimationsListItem(int offset, int otherButtonsWidth) : Button() {
	_name = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	_name->setFillColor(basic_text_color);

	_animCount = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	_animCount->setFillColor(basic_text_color);

	_framesCount = std::make_unique<sf::Text>(basicFont, L"--", basic_text_size);
	_framesCount->setFillColor(basic_text_color);

	_name->setPosition(sf::Vector2f(_rect.position.x + 8, _rect.position.y));
	_animCount->setPosition(sf::Vector2f(_rect.position.x + offset + otherButtonsWidth - _animCount->getGlobalBounds().size.x - 16, _rect.position.y));
	_framesCount->setPosition(sf::Vector2f(_rect.position.x + offset + 2 * otherButtonsWidth - _framesCount->getGlobalBounds().size.x - 16, _rect.position.y));

	_rectIdleColor = sf::Color::Transparent;
	_rectHoverColor = sf::Color(71, 71, 71);
	_rectPressColor = sf::Color(63, 63, 63);

	_rectSelectIdleColor = sf::Color(31, 31, 31);
	_rectSelectHoverColor = sf::Color(15, 15, 15);
	_rectSelectPressColor = sf::Color(23, 23, 23);

}

AnimationsListItem::~AnimationsListItem() {
	
}

void AnimationsListItem::setAnimation(std::weak_ptr<Animations> animation) {
	_animations = animation;
	_name->setString((!animation.expired()) ? animation.lock()->_path : L"");
	_animCount->setString((!animation.expired()) ? std::to_wstring(animation.lock()->_animationsCount) : L"--");
	_framesCount->setString((!animation.expired()) ? std::to_wstring(animation.lock()->_framesCount) : L"--");	
}

void AnimationsListItem::setPosition(sf::Vector2i position, int offset, int otherButtonsWidth) {
	Button::setPosition(position);

	_name->setPosition(sf::Vector2f(_rect.position.x + 8, _rect.position.y));
	_animCount->setPosition(sf::Vector2f(_rect.position.x + offset + otherButtonsWidth - _animCount->getGlobalBounds().size.x - 16, _rect.position.y));
	_framesCount->setPosition(sf::Vector2f(_rect.position.x + offset + 2*otherButtonsWidth - _framesCount->getGlobalBounds().size.x - 16, _rect.position.y));
}

void AnimationsListItem::cursorHover() {
	
	if(!_animations.expired())
		Button::cursorHover();
}

void AnimationsListItem::draw() {
	Button::draw();
	Main::render_window->draw(*_name);
	Main::render_window->draw(*_animCount);
	Main::render_window->draw(*_framesCount);
}



AnimationsListDialog::AnimationsListDialog() : Main::Window(L"Animations List", sf::Vector2i(640, 440), sf::Vector2i(256, 256)) {

	_padding = 8;

	int margin = 2;
	int scrollbarWidth = 24;
	int bottomHeight = 128 + 8;

	_detailsRect = sf::IntRect(
		_contentRect.position + sf::Vector2i(margin, margin), 
		sf::Vector2i(_contentRect.size.x - 2 * margin - scrollbarWidth, basic_text_rect_height + 2* _padding - 2*margin)
	);

	_itemsRect = sf::IntRect(
		_contentRect.position + sf::Vector2i(margin, margin + basic_text_rect_height + margin), 
		_contentRect.size - sf::Vector2i(2*margin, 2*margin + bottomHeight + basic_text_rect_height)
	);

	_bottomRect = sf::IntRect(
		_contentRect.position + sf::Vector2i(margin, _contentRect.size.y - bottomHeight - margin), 
		sf::Vector2i(_contentRect.size.x - 2 * margin, bottomHeight)
	);
	
	_detailsInnerRect = std::make_shared<sf::IntRect>(
		_detailsRect.position + sf::Vector2i(_padding, _padding), 
		_detailsRect.size - sf::Vector2i(_padding * 2, _padding * 2)
	);

	_itemsInnerRect = std::make_shared<sf::IntRect>(
		_itemsRect.position + sf::Vector2i(_padding, _padding), 
		_itemsRect.size - sf::Vector2i(_padding * 2 + scrollbarWidth, _padding * 2)
	);

	_bottomInnerRect = std::make_shared<sf::IntRect>(
		_bottomRect.position + sf::Vector2i(_padding, _padding), 
		_bottomRect.size - sf::Vector2i(_padding * 2, _padding * 2)
	);

	_offset = 512 - 128;

	createAnimationsDetails();
	createAnimationsItems();
	createBottom(nullptr);

	_selectedItemIndex = -1;

	setAnimationsItems();

	// create scrollbar
	_scrollbar = std::make_shared<Scrollbar>(
		_itemsRect.position.x + _itemsRect.size.x - scrollbarWidth,
		_detailsRect.position.y,
		scrollbarWidth,
		(_itemsRect.position.y + _itemsRect.size.y) - _detailsRect.position.y,
		0,
		std::max(0, (int)animations_manager->getAnimationsCount() * basic_text_rect_height - _itemsInnerRect->size.y),
		std::ceil((float)_itemsInnerRect->size.y / (float)basic_text_rect_height) * basic_text_rect_height,
		0
	);

	_scrollbar->_func = [this]() { 
		setAnimationsItems();
		};

}

AnimationsListDialog::~AnimationsListDialog() {

}

void AnimationsListDialog::createAnimationsDetails() {

	_name = std::make_shared<ButtonWithText>(
		L"Name",
		sf::Vector2i(_offset, _detailsInnerRect->size.y),
		sf::Vector2i(_detailsInnerRect->position.x, _detailsInnerRect->position.y)
	);

	int buttonWidth = (_detailsInnerRect->size.x - _offset) / 2;
	_animCount = std::make_shared<ButtonWithText>(
		L"Anims",
		sf::Vector2i(buttonWidth, _detailsInnerRect->size.y),
		sf::Vector2i(_detailsInnerRect->position.x + _offset, _detailsInnerRect->position.y)
	);

	_framesCount = std::make_shared<ButtonWithText>(
		L"Frames",
		sf::Vector2i(buttonWidth, _detailsInnerRect->size.y),
		sf::Vector2i(_detailsInnerRect->position.x + _offset + buttonWidth, _detailsInnerRect->position.y)
	);

	_name->setAlignment(ButtonWithTextAlignment::Left);
	//_animCount->setAlignment(ButtonWithTextAlignment::Left);
	//_framesCount->setAlignment(ButtonWithTextAlignment::Left);

	sf::Color idleColor = sf::Color(71, 71, 71);
	sf::Color hoverColor = sf::Color(95, 95, 95);
	sf::Color pressColor = sf::Color(63, 63, 63);
	_name->setRectColors(idleColor, hoverColor, pressColor, idleColor, hoverColor, pressColor, sf::Color::Transparent);
	_animCount->setRectColors(idleColor, hoverColor, pressColor, idleColor, hoverColor, pressColor, sf::Color::Transparent);
	_framesCount->setRectColors(idleColor, hoverColor, pressColor, idleColor, hoverColor, pressColor, sf::Color::Transparent);


}

void AnimationsListDialog::createAnimationsItems() {

	int itemsCount = std::ceil((float)_itemsInnerRect->size.y / (float)basic_text_rect_height) + 1;

	for (int i = 0; i < itemsCount; i += 1) {
		std::shared_ptr<AnimationsListItem> item = std::make_shared<AnimationsListItem>(_offset, (_itemsInnerRect->size.x - _offset) / 2);
		item->setSize(sf::Vector2i(_itemsInnerRect->size.x, basic_text_rect_height));
		item->setPosition(_itemsInnerRect->position + sf::Vector2i(0, i * basic_text_rect_height), _offset, (_itemsInnerRect->size.x - _offset) / 2);
		_animationsItems.push_back(item);
	}
}

void AnimationsListDialog::createBottom(std::function<void()> function) {
	
	_fileNameText = std::make_unique<sf::Text>(basicFont, L"Animation", basic_text_size);
	_fileNameText->setFillColor(basic_text_color);
	_fileNameText->setPosition(sf::Vector2f(_bottomInnerRect->position + sf::Vector2i(32, 16 + 4)));

	sf::Vector2i textInputSize;
	textInputSize.x = _bottomInnerRect->size.x - 32 - _fileNameText->getGlobalBounds().size.x - 32;
	textInputSize.y = 32;

	_fileNameInput = std::make_shared<TextInput>(textInputSize, L"", -1, basic_text_size);
	_fileNameInput->setColors(sf::Color(71, 71, 71), sf::Color(95, 95, 95), sf::Color(71, 71, 71), sf::Color(63, 63, 63));
	_fileNameInput->setPosition(_bottomInnerRect->position + sf::Vector2i(32 + _fileNameText->getGlobalBounds().size.x + 16, 16));

	sf::Vector2i pos = _bottomInnerRect->position + sf::Vector2i(_bottomInnerRect->size.x - 2 * 128 - Main::Window::padding, _bottomInnerRect->size.y - 48 - Main::Window::padding);

	_confirmButton = std::make_shared<ButtonWithTextAndSprite>(
		L"select",
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\button.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\button_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\button_press.png"),
		nullptr,
		pos
	);

	_cancelButton = std::make_shared<ButtonWithTextAndSprite>(
		L"cancel",
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\button.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\button_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\windows\\file_dialog\\button_press.png"),
		nullptr,
		pos + sf::Vector2i(128, 0)
	);

	_confirmButton->_onclick_func = [this, function]() {
		if (function)
			function();
	};


	_cancelButton->_onclick_func = [this]() {
		_closeButton->_onclick_func();
	};
}

void AnimationsListDialog::setAnimationsItems() {
	int scrollbarValue = (_scrollbar)? _scrollbar->getValue() : 0;
	int startIndex = scrollbarValue / basic_text_rect_height;
	for (int i = 0; i < _animationsItems.size(); i += 1) {

		if (i + startIndex == _selectedItemIndex) {
			_animationsItems[i]->setSelect(true);
		}
		else {
			_animationsItems[i]->setSelect(false);
		}

		if (i + startIndex < animations_manager->getAnimationsCount()) {
			_animationsItems[i]->setAnimation(animations_manager->getAnimations(i + startIndex));
			_animationsItems[i]->_onclick_func = [this, i]() {
				_selectedItemIndex = i + (_scrollbar ? _scrollbar->getValue() / basic_text_rect_height : 0);
				setAnimationsItems();
				_fileNameInput->setText(_animationsItems[i]->_animations.lock()->_path);
			};
		}
		else {
			_animationsItems[i]->setAnimation(std::weak_ptr<Animations>());
			_animationsItems[i]->_onclick_func = nullptr;
		}

		_animationsItems[i]->setPosition(_itemsInnerRect->position + sf::Vector2i(0, i * basic_text_rect_height - scrollbarValue % basic_text_rect_height), _offset, (_itemsInnerRect->size.x - _offset) / 2);
	}
}

void AnimationsListDialog::setPosition(sf::Vector2i position) {
	Main::Window::setPosition(position);

	int margin = 2;
	int scrollbarWidth = 24;

	_detailsRect.position = _contentRect.position + sf::Vector2i(margin, margin);
	_itemsRect.position = _contentRect.position + sf::Vector2i(margin, margin + basic_text_rect_height + margin),
	_bottomRect.position = _contentRect.position + sf::Vector2i(margin, _contentRect.size.y - _bottomRect.size.y - margin);

	_detailsInnerRect->position = _detailsRect.position + sf::Vector2i(_padding, _padding);
	_itemsInnerRect->position = _itemsRect.position + sf::Vector2i(_padding, _padding);
	_bottomInnerRect->position = _bottomRect.position + sf::Vector2i(_padding, _padding);

	_name->setPosition(sf::Vector2i(_detailsInnerRect->position.x, _detailsInnerRect->position.y));
	_animCount->setPosition(sf::Vector2i(_detailsInnerRect->position.x + _offset, _detailsInnerRect->position.y));
	_framesCount->setPosition(sf::Vector2i(_detailsInnerRect->position.x + _offset + (_detailsInnerRect->size.x - _offset) / 2, _detailsInnerRect->position.y));

	for (int i = 0; i < _animationsItems.size(); i += 1) {
		_animationsItems[i]->setPosition(_itemsInnerRect->position + sf::Vector2i(0, i * basic_text_rect_height - _scrollbar->getValue()%basic_text_rect_height), _offset, (_itemsInnerRect->size.x - _offset) / 2);
	}

	_scrollbar->setPosition(sf::Vector2i(_itemsRect.position.x + _itemsRect.size.x - 24, _detailsRect.position.y));

	_fileNameText->setPosition(sf::Vector2f(_bottomInnerRect->position + sf::Vector2i(32, 16 + 4)));

	sf::Vector2i textInputSize;
	textInputSize.x = _bottomInnerRect->size.x - 32 - _fileNameText->getGlobalBounds().size.x - 32;
	textInputSize.y = 32;

	_fileNameInput->setPosition(_bottomInnerRect->position + sf::Vector2i(32 + _fileNameText->getGlobalBounds().size.x + 16, 16));

	sf::Vector2i pos = _bottomInnerRect->position + sf::Vector2i(_bottomInnerRect->size.x - 2 * 128 - Main::Window::padding, _bottomInnerRect->size.y - 48 - Main::Window::padding);
	_confirmButton->setPosition(pos);
	_cancelButton->setPosition(pos + sf::Vector2i(128, 0));

}

sf::FloatRect AnimationsListDialog::getItemsRect() {
	return sf::FloatRect(
		sf::Vector2f((float)_itemsInnerRect->position.x, (float)_itemsInnerRect->position.y),
		sf::Vector2f((float)_itemsInnerRect->size.x, (float)(_itemsInnerRect->size.y))
	);
}

void AnimationsListDialog::cursorHover() {
	Main::Window::cursorHover();

	_name->cursorHover();
	_animCount->cursorHover();
	_framesCount->cursorHover();

	_scrollbar->cursorHover();

	if (_itemsInnerRect->contains(Main::cursor->_position)) {
		for (auto& item : _animationsItems) {
			item->cursorHover();
		}
	}

	_fileNameInput->cursorHover();

	_confirmButton->cursorHover();
	_cancelButton->cursorHover();
}

void AnimationsListDialog::handleEvent(const sf::Event& event) {
	Main::Window::handleEvent(event);

	_name->handleEvent(event);
	_animCount->handleEvent(event);
	_framesCount->handleEvent(event);

	_scrollbar->handleEvent(event);

	for (auto& item : _animationsItems) {
		item->handleEvent(event);
	}

	_fileNameInput->handleEvent(event);
	_confirmButton->handleEvent(event);
	_cancelButton->handleEvent(event);
}

void AnimationsListDialog::update() {
	Main::Window::update();

	_name->update();
	_animCount->update();
	_framesCount->update();

	_scrollbar->update();

	for (auto& item : _animationsItems) {
		item->update();
	}

	_fileNameInput->update();

	_confirmButton->update();
	_cancelButton->update();
}

void AnimationsListDialog::draw() {
	Main::Window::draw();

	sf::RectangleShape detailsRectShape(sf::Vector2f(_detailsRect.size));
	detailsRectShape.setPosition(sf::Vector2f(_detailsRect.position));
	detailsRectShape.setFillColor(sf::Color(23, 23, 23));
	Main::render_window->draw(detailsRectShape);

	sf::RectangleShape itemsRectShape(sf::Vector2f(_itemsRect.size));
	itemsRectShape.setPosition(sf::Vector2f(_itemsRect.position));
	itemsRectShape.setFillColor(sf::Color(23, 23, 23));
	Main::render_window->draw(itemsRectShape);

	sf::RectangleShape bottomRectShape(sf::Vector2f(_bottomRect.size));
	bottomRectShape.setPosition(sf::Vector2f(_bottomRect.position));
	bottomRectShape.setFillColor(sf::Color(23, 23, 23));
	Main::render_window->draw(bottomRectShape);

	sf::RectangleShape detailsInnerRect(sf::Vector2f(_detailsInnerRect->size));
	detailsInnerRect.setPosition(sf::Vector2f(_detailsInnerRect->position));
	detailsInnerRect.setFillColor(sf::Color(47, 47, 47));
	detailsInnerRect.setOutlineThickness(1.0f);
	detailsInnerRect.setOutlineColor(sf::Color(63, 63, 63));
	Main::render_window->draw(detailsInnerRect);

	_name->draw();
	_animCount->draw();
	_framesCount->draw();

	sf::RectangleShape itemsInnerRect(sf::Vector2f(_itemsInnerRect->size));
	itemsInnerRect.setPosition(sf::Vector2f(_itemsInnerRect->position));
	itemsInnerRect.setFillColor(sf::Color(47, 47, 47));
	itemsInnerRect.setOutlineThickness(1.0f);
	itemsInnerRect.setOutlineColor(sf::Color(63, 63, 63));
	Main::render_window->draw(itemsInnerRect);

	sf::FloatRect frect = getItemsRect();
	sf::View view(frect);

	sf::FloatRect vp(
		sf::Vector2f(
			frect.position.x / GUI_manager->_view.getSize().x,
			frect.position.y / GUI_manager->_view.getSize().y
		),

		sf::Vector2f(
			frect.size.x / GUI_manager->_view.getSize().x,
			frect.size.y / GUI_manager->_view.getSize().y
		)
	);

	view.setViewport(vp);
	Main::render_window->setView(view);

	for(auto& item : _animationsItems) {
		item->draw();
	}

	GUI_manager->setView();
	_scrollbar->draw();

	sf::RectangleShape bottomInnerRect(sf::Vector2f(_bottomInnerRect->size));
	bottomInnerRect.setPosition(sf::Vector2f(_bottomInnerRect->position));
	bottomInnerRect.setFillColor(sf::Color(23, 23, 23));
	bottomInnerRect.setOutlineThickness(1.0f);
	bottomInnerRect.setOutlineColor(sf::Color(63, 63, 63));
	Main::render_window->draw(bottomInnerRect);

	if (_fileNameText) {
		Main::render_window->draw(*_fileNameText);
	}

	if (_fileNameInput) {
		_fileNameInput->draw();
	}

	_confirmButton->draw();
	_cancelButton->draw();
}