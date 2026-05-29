#include "Components/Palette/Slots.hpp"
#include "Objects/Terrain.hpp"
#include "Components/Palette/TerrainSlot.hpp"
#include "Components/Palette/GameObjectSlot.hpp"
#include "PrefabsManager.hpp"
#include "Window.hpp"
#include "Theme.hpp"
#include "DebugLog.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/MapEditor/Map/Tileset.hpp"

Slots::Slots() {

	_main_margin = 16;
	_top_margin = 22;
	_outer_margin = 16;
	
	_text = std::make_unique<sf::Text>(basicFont, L"Palette", 20);
	_text->setFillColor(basic_text_color);

	_slotsCount = sf::Vector2i(6, 8);

	_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 6 * 80) / 5;	// 32 is scrollbar width, 80 is slot size
	_rect.size = sf::Vector2i(600 - 2 * _main_margin, _slotsCount.y*(80+_inner_margin) + 2 * _main_margin + _top_margin + _outer_margin);

	_scrollbar = nullptr;

	_selectedSlotId = -1;
	_selectedSlot = nullptr;


	_type = ObjectType::None;
	setCategory(_type);

	
}

Slots::~Slots() {

}

sf::Vector2i Slots::getPosition() {
	return _rect.position;
}

sf::Vector2i Slots::getSize() {
	return _rect.size;
}

void Slots::setPosition(sf::Vector2i position) {

	_rect.position = position + sf::Vector2i(_main_margin, _main_margin);
	
	int x = _rect.position.x + _outer_margin;
	int y = _rect.position.y + _outer_margin;
	
	_text->setPosition(sf::Vector2f(x, y));

}

void Slots::createSlots(sf::Vector2i slotsCount) {
	_slots.clear();

	_slotsCount = slotsCount;

	std::shared_ptr<Texture> slotTexture;
	std::shared_ptr<Texture> slotHoverTexture;
	std::shared_ptr<Texture> slotPressTexture;

	if(_type == ObjectType::Terrain) {
		_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 3 * 160) / 2;
		slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160.png");
		slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160_hover.png");
		slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160_press.png");
	}
	else {
		_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 6 * 80) / 5;
		slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80.png");
		slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80_hover.png");
		slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80_press.png");
	}

	for (int y = 0; y < (_slotsCount.y+1); y++) {
		for (int x = 0; x < (_slotsCount.x); x++) {
			sf::Vector2i position;
			
			if (_type == ObjectType::Terrain) {
				position = sf::Vector2i(_rect.position.x + _outer_margin + x * (160 + _inner_margin), _rect.position.y + _main_margin + _outer_margin + _top_margin + y * (160 + _inner_margin));
				_slots.emplace_back(std::make_shared<TerrainSlot>(
					slotTexture,
					slotHoverTexture,
					slotPressTexture,
					position
				));
			} else {
				position = sf::Vector2i(_rect.position.x + _outer_margin + x * (80 + _inner_margin), _rect.position.y + _main_margin + _outer_margin + _top_margin + y * (80 + _inner_margin));
				_slots.emplace_back(std::make_shared<GameObjectSlot>(
					slotTexture,
					slotHoverTexture,
					slotPressTexture,
					position
				));
			}
			
			
		}
	}
}

void Slots::generateScrollbar() {
	sf::Vector2i scrollbarPosition = sf::Vector2i(_rect.position.x + _rect.size.x - 32 - _outer_margin, _rect.position.y + _main_margin + _top_margin + 2 * _inner_margin);
	sf::Vector2i scrollbarSize = sf::Vector2i(32, _rect.size.y - 2 * _main_margin - _top_margin - _inner_margin);

	int rowsTotal;
	if (_type == ObjectType::Terrain)
		rowsTotal = (int)std::ceil(MapEditor::editor->_tileset->groups.size() / _slotsCount.x);
	else
		rowsTotal = (int)std::ceil((float)prefabs_manager->getPrefabs(_type).size() / (float)_slotsCount.x);

	int rowsVisible = _slotsCount.y;
	int rowHeight = (_type == ObjectType::Terrain) ? 160 + _inner_margin : 80 + _inner_margin;
	int scrollbarValue = 0;
	int scrollbarMaxValue = std::max(0, (rowsTotal - rowsVisible) * rowHeight);
	int scrollbarSliderSize = (_type == ObjectType::Terrain) ? _slotsCount.y * (160 + _inner_margin) : _slotsCount.y * (80 + _inner_margin);

	if (_type == ObjectType::Terrain) {
		_scrollbar = std::make_shared<Scrollbar>(scrollbarPosition.x, scrollbarPosition.y, scrollbarSize.x, scrollbarSize.y, 0, scrollbarMaxValue, scrollbarSliderSize, scrollbarValue);
		_scrollbar->setScrollArea(std::make_shared<sf::IntRect>(
			sf::Vector2i(_rect.position.x + _outer_margin, _rect.position.y + _main_margin + _top_margin + _outer_margin),
			sf::Vector2i(_rect.size.x - 32 - 2 * _outer_margin, _rect.size.y - 2 * _main_margin - _top_margin - _outer_margin)),
			(160 + _inner_margin) / 4);
	}
	else {
		_scrollbar = std::make_shared<Scrollbar>(scrollbarPosition.x, scrollbarPosition.y, scrollbarSize.x, scrollbarSize.y, 0, scrollbarMaxValue, scrollbarSliderSize, scrollbarValue);
		_scrollbar->setScrollArea(std::make_shared<sf::IntRect>(
			sf::Vector2i(_rect.position.x + _outer_margin, _rect.position.y + _main_margin + _top_margin + _outer_margin),
			sf::Vector2i(_rect.size.x - 32 - 2 * _outer_margin, _rect.size.y - 2 * _main_margin - _top_margin - _outer_margin)),
			(80 + _inner_margin) / 4);
	}


	_scrollbar->_func = [this]() {
		int i = 0;
		for (auto& slot : _slots) {
			updateObjects();

			sf::Vector2i position;
			if (_type == ObjectType::Terrain) {
				position = sf::Vector2i(
					_rect.position.x + _outer_margin + (i % _slotsCount.x) * (160 + _inner_margin),
					_rect.position.y + _main_margin + _top_margin + _outer_margin + (i / _slotsCount.x) * (160 + _inner_margin) - (_scrollbar->getValue() % (160 + _inner_margin))
				);
			}
			else {
				position = sf::Vector2i(
					_rect.position.x + _outer_margin + (i % _slotsCount.x) * (80 + _inner_margin),
					_rect.position.y + _main_margin + _top_margin + _outer_margin + (i / _slotsCount.x) * (80 + _inner_margin) - (_scrollbar->getValue() % (80 + _inner_margin))
				);
			}

			slot->setPosition(position);
			i += 1;
		}
		};
}

void Slots::loadObjects() {

	selectSlot(_selectedSlotId);

	if (_type == ObjectType::Terrain) {

		std::vector<std::shared_ptr<Terrain>> terrains;
		for(int i=0;i<MapEditor::editor->_tileset->groups.size(); i++) {

			terrains.emplace_back(std::make_shared<Terrain>(i, sf::IntRect(sf::Vector2i(15 * 64, i * 64), sf::Vector2i(16, 16))));
		}

		for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y + 1); i++) {
			if (i < terrains.size()) {
				_slots[i]->_object = terrains[i];
				_slots[i]->_animator = nullptr;
			}
			else {
				_slots[i]->_object = nullptr;
				_slots[i]->_animator = nullptr;
			}
		}

		return;
	}

	std::vector<std::shared_ptr<GameObject>> prefabs = prefabs_manager->getPrefabs(_type);

	for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y+1); i++) {
		if (i < prefabs.size()) {
			_slots[i]->_object = prefabs[i];
			_slots[i]->_animator = std::make_shared<Animator>(prefabs[i]->getAnimations(), 0.2f);
			_slots[i]->_animator->play();
		}
		else {
			_slots[i]->_object = nullptr;
			_slots[i]->_animator = nullptr;
		}
	}
}

void Slots::updateObjects() {

	int startIndex;
	
	if(_type == ObjectType::Terrain)
		startIndex = _scrollbar->getValue() / (160 + _inner_margin) * _slotsCount.x;
	else
		startIndex = _scrollbar->getValue() / (80 + _inner_margin) * _slotsCount.x;

	selectSlot(_selectedSlotId);

	if (_type == ObjectType::Terrain) {

		std::vector<std::shared_ptr<Terrain>> terrains;
		for (int i = 0; i < MapEditor::editor->_tileset->groups.size(); i++) {

			terrains.emplace_back(std::make_shared<Terrain>(i, sf::IntRect(sf::Vector2i(15 * 64, i * 64), sf::Vector2i(16, 16))));

		}

		for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y + 1); i++) {
			if (i+startIndex < terrains.size()) {
				_slots[i]->_object = terrains[i+startIndex];
				_slots[i]->_animator = nullptr;
			}
			else {
				_slots[i]->_object = nullptr;
				_slots[i]->_animator = nullptr;
			}
		}

		return;
	}

	std::vector<std::shared_ptr<GameObject>> prefabs = prefabs_manager->getPrefabs(_type);

	
	for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y+1); i++) {
		if (i + startIndex < prefabs.size()) {
			_slots[i]->_object = prefabs[i + startIndex];
			_slots[i]->_animator = std::make_shared<Animator>(prefabs[i + startIndex]->getAnimations(), 0.2f);
			_slots[i]->_animator->play();
		}
		else {
			_slots[i]->_object = nullptr;
			_slots[i]->_animator = nullptr;
		}
	}
}

void Slots::setCategory(ObjectType type) {

	if (_type != type) {
		_type = type;
		selectSlot(-1);
		MapEditor::editor->_cursor_on_map->_object = nullptr;
	}

	

	// +1 because scrollbar takes one slot space
	if(_type == ObjectType::Terrain) {
		createSlots(sf::Vector2i(3, 3));
		_rect.size = sf::Vector2i(600 - 2 * _main_margin, _slotsCount.y * (160 + _inner_margin) + 2 * _main_margin + _top_margin + _outer_margin);

	}
	else {
		createSlots(sf::Vector2i(6, 9));
		_rect.size = sf::Vector2i(600 - 2 * _main_margin, _slotsCount.y * (80 + _inner_margin) + 2 * _main_margin + _top_margin + _outer_margin);

	}


	generateScrollbar();
	loadObjects();
}

void Slots::setFunction(std::function<void(std::shared_ptr<Slot> slot, int selectedSlotId)> function) {


	for (int i = 0; i < _slots.size(); i++) {

		std::shared_ptr<Slot> slot = _slots[i];

		slot->_onclick_func = [this, slot, i, function]() {

			if (slot->_object != nullptr) {
				int startIndex = _scrollbar->getValue() / ((_type == ObjectType::Terrain) ? (160 + _inner_margin) : (80 + _inner_margin)) * _slotsCount.x;
				function(slot, i + startIndex);
			}
			};
	}
}
void Slots::selectSlot(int selectedSlotId) {

	if (_selectedSlot != nullptr) {

		std::shared_ptr<Texture> slotTexture;
		std::shared_ptr<Texture> slotHoverTexture;
		std::shared_ptr<Texture> slotPressTexture;

		if (_type == ObjectType::Terrain) {
			_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 3 * 160) / 2;
			slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160.png");
			slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160_hover.png");
			slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160_press.png");
		}
		else {
			_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 6 * 80) / 5;
			slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80.png");
			slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80_hover.png");
			slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80_press.png");
		}

		_selectedSlot->_texture = slotTexture;
		_selectedSlot->_hoverTexture = slotHoverTexture;
		_selectedSlot->_pressTexture = slotPressTexture;
	}

	_selectedSlotId = selectedSlotId;

	if (_selectedSlotId < 0)
		return;

	int startIndex = _scrollbar->getValue() / ((_type == ObjectType::Terrain) ? (160 + _inner_margin) : (80 + _inner_margin)) * _slotsCount.x;
	int localSlotId = selectedSlotId - startIndex;

	if (localSlotId < 0 || localSlotId >= _slots.size()) {
		_selectedSlot = nullptr;
		return;
	}

	_selectedSlot = _slots[localSlotId];

	if (_selectedSlot != nullptr) {

		std::shared_ptr<Texture> slotTexture;
		std::shared_ptr<Texture> slotHoverTexture;
		std::shared_ptr<Texture> slotPressTexture;

		if (_type == ObjectType::Terrain) {
			_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 3 * 160) / 2;
			slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_160.png");
			slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_160_hover.png");
			slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_160_press.png");
		}
		else {
			_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 6 * 80) / 5;
			slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_80.png");
			slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_80_hover.png");
			slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_80_press.png");
		}

		_selectedSlot->_texture = slotTexture;
		_selectedSlot->_hoverTexture = slotHoverTexture;
		_selectedSlot->_pressTexture = slotPressTexture;
	}
}

void Slots::selectLastSlot() {

	std::vector<std::shared_ptr<GameObject>> prefabs = prefabs_manager->getPrefabs(_type);
	_selectedSlotId = prefabs.size() - 1;
	selectSlot(_selectedSlotId);
}

sf::FloatRect Slots::getSlotsRect() {


	return sf::FloatRect(
		sf::Vector2f((float)_rect.position.x, (float)_rect.position.y + _main_margin + _top_margin + _outer_margin),
		sf::Vector2f((float)_rect.size.x - 32.f, (float)(_rect.size.y - 2 * _main_margin - _top_margin - _outer_margin))
	);
}


void Slots::cursorHover() {

	for (auto& slot : _slots) {
		slot->cursorHover();
	}

	_scrollbar->cursorHover();
}

void Slots::handleEvent(const sf::Event& event) {

	for (auto& slot : _slots) {
		slot->handleEvent(event);
	}

	_scrollbar->handleEvent(event);
}

void Slots::update() {

	for (auto& slot : _slots) {
		slot->update();
	}

	_scrollbar->update();
}

void Slots::draw() {

	int border = 2;
	sf::RectangleShape background(sf::Vector2f(_rect.size.x - 2 * border, _rect.size.y - 2 * border));
	background.setPosition(sf::Vector2f(_rect.position.x + border, _rect.position.y + border));
	background.setFillColor(sf::Color(31, 31, 31));
	background.setOutlineThickness(2);
	background.setOutlineColor(sf::Color(63, 63, 63));
	window->draw(background);

	window->draw(*_text);

	_scrollbar->draw();

	sf::FloatRect slotsRect = getSlotsRect();
	sf::View view(slotsRect);

	sf::FloatRect vp(
		sf::Vector2f(
			slotsRect.position.x / GUI_manager->_view.getSize().x,
			slotsRect.position.y / GUI_manager->_view.getSize().y
		),

		sf::Vector2f(
			slotsRect.size.x / GUI_manager->_view.getSize().x,
			slotsRect.size.y / GUI_manager->_view.getSize().y
		)
	);

	view.setViewport(vp);
	window->setView(view);

	for (auto& slot : _slots) {
		slot->draw();
	}
}

