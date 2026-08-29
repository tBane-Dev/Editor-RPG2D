#include "Components/Palette/Slots.hpp"
#include "Objects/Terrain.hpp"
#include "Objects/Floor.hpp"
#include "Wallset.hpp"
#include "Components/Palette/Palette.hpp"
#include "Components/Palette/TerrainSlot.hpp"
#include "Components/Palette/GameObjectSlot.hpp"
#include "Components/Palette/FloorSlot.hpp" 
#include "Components/Palette/WallSlot.hpp"
#include "Components/Palette/BuildingSlot.hpp"
#include "Components/Palette/Slot.hpp"
#include "Components/Palette/Tools.hpp"
#include "Components/Palette/Tools-Terrain.hpp"	
#include "Components/Palette/Tools-Building.hpp"	
#include "PrefabsManager.hpp"
#include "RenderWindow.hpp"
#include "Theme.hpp"
#include "DebugLog.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Tileset.hpp"
#include "TexturesManager.hpp"


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

	_emptySlotAnimation = std::make_shared<Animations>(
		L"none",
		textures_manager->getTexture(L"assets\\tex\\palette\\none.png"),
		sf::Vector2i(80, 80),
		1, 1
	);

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

	// Scrollbar
	if (_scrollbar) {
		sf::Vector2i scrollbarPosition(_rect.position.x + _rect.size.x - 32 - _outer_margin, _rect.position.y + _main_margin + _top_margin);
		_scrollbar->setPosition(scrollbarPosition);
	}

	// Slots
	int slotSize = 80;

	if (_type == ObjectType::Terrain)
		slotSize = 160;
	else if (_type == ObjectType::Building)
		slotSize = 240;
	else if (_type == ObjectType::Wall || _type == ObjectType::Floor || _type == ObjectType::Door || _type == ObjectType::Window || _type == ObjectType::WallMounted)
		slotSize = 120;

	int scrollOffset = 0;

	if (_scrollbar)
		scrollOffset = _scrollbar->getValue() % (slotSize + _inner_margin);

	for (int i = 0; i < _slots.size(); i++) {

		int slotX = i % _slotsCount.x;
		int slotY = i / _slotsCount.x;

		sf::Vector2i slotPosition(_rect.position.x + _outer_margin + slotX * (slotSize + _inner_margin), _rect.position.y + _main_margin + _top_margin + _outer_margin + slotY * (slotSize + _inner_margin) -scrollOffset);
		_slots[i]->setPosition(slotPosition);
	}
}

void Slots::createSlots(sf::Vector2i slotsCount) {
	_slots.clear();

	_slotsCount = slotsCount;

	std::shared_ptr<Texture> slotTexture;
	std::shared_ptr<Texture> slotHoverTexture;
	std::shared_ptr<Texture> slotPressTexture;
	std::shared_ptr<Texture> slotInactiveTexture;

	if(_type == ObjectType::Terrain) {
		_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 3 * 160) / 4;
		slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160.png");
		slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160_hover.png");
		slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160_press.png");
		slotInactiveTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160_inactive.png");
	}
	else if (_type == ObjectType::Building) {
		_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 2 * 240) / 3;
		slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_240.png");
		slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_240_hover.png");
		slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_240_press.png");
		slotInactiveTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_240_inactive.png");
	}
	else if(_type == ObjectType::Wall || _type == ObjectType::Floor || _type == ObjectType::Door || _type == ObjectType::Window || _type == ObjectType::WallMounted) {
		_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 4 * 120) / 5;
		slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_120.png");
		slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_120_hover.png");
		slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_120_press.png");
		slotInactiveTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_120_inactive.png");
	}
	else {
		_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 6 * 80) / 7;
		slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80.png");
		slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80_hover.png");
		slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80_press.png");
		slotInactiveTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80_inactive.png");
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
					slotInactiveTexture,
					position
				));
			}
			else if (_type == ObjectType::Building) {
				position = sf::Vector2i(_rect.position.x + _outer_margin + x * (240 + _inner_margin), _rect.position.y + _main_margin + _outer_margin + _top_margin + y * (240 + _inner_margin));
				
				int index = y * _slotsCount.x + x;
				std::shared_ptr<BuildingPrefab> b = nullptr;
				if (index < Components::Palette::buildings.size())
					b = Components::Palette::buildings[index];

				std::shared_ptr<BuildingSlot> slot = std::make_shared<BuildingSlot>(
					slotTexture,
					slotHoverTexture,
					slotPressTexture,
					slotInactiveTexture,
					position,
					b
				);

				
				_slots.emplace_back(slot);	
			}
			else if (_type == ObjectType::Wall || _type == ObjectType::Floor || _type == ObjectType::Door || _type == ObjectType::Window || _type == ObjectType::WallMounted) {
				position = sf::Vector2i(_rect.position.x + _outer_margin + x * (120 + _inner_margin), _rect.position.y + _main_margin + _outer_margin + _top_margin + y * (120 + _inner_margin));
				
				if (_type == ObjectType::Floor) {
					position = sf::Vector2i(_rect.position.x + _outer_margin + x * (120 + _inner_margin), _rect.position.y + _main_margin + _outer_margin + _top_margin + y * (120 + _inner_margin));
					std::shared_ptr<FloorSlot> slot = std::make_shared<FloorSlot>(
						slotTexture,
						slotHoverTexture,
						slotPressTexture,
						slotInactiveTexture,
						position
					);
					int index = y * _slotsCount.x + x;
					if (index >= FloorSlot::_floorset->_texture->getSize().x / 64)
						index = 0;
					slot->setIndex(index);
					_slots.emplace_back(slot);
				}
				else if(_type == ObjectType::Wall) {

					std::shared_ptr<WallSlot> slot = std::make_shared<WallSlot>(
						slotTexture,
						slotHoverTexture,
						slotPressTexture,
						slotInactiveTexture,
						position
					);
					slot->_object = wallset->getPrefab(y * _slotsCount.x + x);
					_slots.emplace_back(slot);
				}
				else {
					_slots.emplace_back(std::make_shared<Slot>(
						slotTexture,
						slotHoverTexture,
						slotPressTexture,
						slotInactiveTexture,
						position
					));
				}
			}
			else {
				position = sf::Vector2i(_rect.position.x + _outer_margin + x * (80 + _inner_margin), _rect.position.y + _main_margin + _outer_margin + _top_margin + y * (80 + _inner_margin));
				_slots.emplace_back(std::make_shared<GameObjectSlot>(
					slotTexture,
					slotHoverTexture,
					slotPressTexture,
					slotInactiveTexture,
					position
				));
			}
			
			_slots.back()->setActive(false);
		}
	}
}

void Slots::generateScrollbar() {
	sf::Vector2i scrollbarPosition = sf::Vector2i(_rect.position.x + _rect.size.x - 32 - _outer_margin, _rect.position.y + _main_margin + _top_margin);
	sf::Vector2i scrollbarSize = sf::Vector2i(32, _rect.size.y - 2 * _main_margin - _top_margin);

	int rowsTotal;
	int rowsVisible;
	int rowHeight;
	int scrollbarValue;
	int scrollbarMaxValue;
	int scrollbarSliderSize;

	if (_type == ObjectType::Terrain) {
		rowsTotal = (int)std::ceil(tileset->groups.size() / _slotsCount.x);
		rowsVisible = _slotsCount.y;
		rowHeight = 160 + _inner_margin;
		scrollbarValue = 0;
		scrollbarMaxValue = std::max(0, (rowsTotal - rowsVisible) * rowHeight);
		scrollbarSliderSize = _slotsCount.y * (160 + _inner_margin);
	}
	else if (_type == ObjectType::Building) {
		rowsTotal = (int)std::ceil(Components::Palette::buildings.size() / _slotsCount.x);
		rowsVisible = _slotsCount.y;
		rowHeight = 240 + _inner_margin;
		scrollbarValue = 0;
		scrollbarMaxValue = std::max(0, (rowsTotal - rowsVisible) * rowHeight);
		scrollbarSliderSize = _slotsCount.y * (240 + _inner_margin);
	}
	else if(_type == ObjectType::Wall || _type == ObjectType::Floor || _type == ObjectType::Door || _type == ObjectType::Window || _type == ObjectType::WallMounted) {
		rowsTotal = (int)std::ceil(tileset->groups.size() / _slotsCount.x);
		rowsVisible = _slotsCount.y;
		rowHeight = 120 + _inner_margin;
		scrollbarValue = 0;
		scrollbarMaxValue = std::max(0, (rowsTotal - rowsVisible) * rowHeight);
		scrollbarSliderSize = _slotsCount.y * (120 + _inner_margin);
	}
	else {
		rowsTotal = (int)std::ceil(tileset->groups.size() / _slotsCount.x);
		rowsVisible = _slotsCount.y;
		rowHeight = 80 + _inner_margin;
		scrollbarValue = 0;
		scrollbarMaxValue = std::max(0, (rowsTotal - rowsVisible) * rowHeight);
		scrollbarSliderSize = _slotsCount.y * (80 + _inner_margin);
	}
	

	if (_type == ObjectType::Terrain) {
		_scrollbar = std::make_shared<Scrollbar>(scrollbarPosition.x, scrollbarPosition.y, scrollbarSize.x, scrollbarSize.y, 0, scrollbarMaxValue, scrollbarSliderSize, scrollbarValue);
		_scrollbar->setScrollArea(std::make_shared<sf::IntRect>(
			sf::Vector2i(_rect.position.x + _outer_margin, _rect.position.y + _main_margin + _top_margin + _outer_margin),
			sf::Vector2i(_rect.size.x - 32 - 2 * _outer_margin, _rect.size.y - 2 * _main_margin - _top_margin - _outer_margin)),
			(160 + _inner_margin) / 4);
	}
	else if (_type == ObjectType::Building) {
		_scrollbar = std::make_shared<Scrollbar>(scrollbarPosition.x, scrollbarPosition.y, scrollbarSize.x, scrollbarSize.y, 0, scrollbarMaxValue, scrollbarSliderSize, scrollbarValue);
		_scrollbar->setScrollArea(std::make_shared<sf::IntRect>(
			sf::Vector2i(_rect.position.x + _outer_margin, _rect.position.y + _main_margin + _top_margin + _outer_margin),
			sf::Vector2i(_rect.size.x - 32 - 2 * _outer_margin, _rect.size.y - 2 * _main_margin - _top_margin - _outer_margin)),
			(240 + _inner_margin) / 4);
	}
	else if (_type == ObjectType::Wall || _type == ObjectType::Floor || _type == ObjectType::Door || _type == ObjectType::Window || _type == ObjectType::WallMounted) {
		_scrollbar = std::make_shared<Scrollbar>(scrollbarPosition.x, scrollbarPosition.y, scrollbarSize.x, scrollbarSize.y, 0, scrollbarMaxValue, scrollbarSliderSize, scrollbarValue);
		_scrollbar->setScrollArea(std::make_shared<sf::IntRect>(
			sf::Vector2i(_rect.position.x + _outer_margin, _rect.position.y + _main_margin + _top_margin + _outer_margin),
			sf::Vector2i(_rect.size.x - 32 - 2 * _outer_margin, _rect.size.y - 2 * _main_margin - _top_margin - _outer_margin)),
			(120 + _inner_margin) / 6);
	}
	else {
		_scrollbar = std::make_shared<Scrollbar>(scrollbarPosition.x, scrollbarPosition.y, scrollbarSize.x, scrollbarSize.y, 0, scrollbarMaxValue, scrollbarSliderSize, scrollbarValue);
		_scrollbar->setScrollArea(std::make_shared<sf::IntRect>(
			sf::Vector2i(_rect.position.x + _outer_margin, _rect.position.y + _main_margin + _top_margin + _outer_margin),
			sf::Vector2i(_rect.size.x - 32 - 2 * _outer_margin, _rect.size.y - 2 * _main_margin - _top_margin - _outer_margin)),
			(80 + _inner_margin) / 10);
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
			else if (_type == ObjectType::Building) {
				position = sf::Vector2i(
					_rect.position.x + _outer_margin + (i % _slotsCount.x) * (240 + _inner_margin),
					_rect.position.y + _main_margin + _top_margin + _outer_margin + (i / _slotsCount.x) * (240 + _inner_margin) - (_scrollbar->getValue() % (240 + _inner_margin))
				);
			}
			else if(_type == ObjectType::Wall || _type == ObjectType::Floor || _type == ObjectType::Door || _type == ObjectType::Window || _type == ObjectType::WallMounted) {
				position = sf::Vector2i(
					_rect.position.x + _outer_margin + (i % _slotsCount.x) * (120 + _inner_margin),
					_rect.position.y + _main_margin + _top_margin + _outer_margin + (i / _slotsCount.x) * (120 + _inner_margin) - (_scrollbar->getValue() % (120 + _inner_margin))
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

		for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y + 1); i++) {
			if (i < Components::Palette::terrains.size()) {
				_slots[i]->_object = Components::Palette::terrains[i];
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(true);
			}
			else {
				_slots[i]->_object = std::weak_ptr<Object>();
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(false);
			}
		}

		return;
	}

	if(_type == ObjectType::Floor) {
		for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y + 1); i++) {
			if (i < FloorSlot::_floorset->_texture->getSize().x / 64) {
				_slots[i]->_object = Components::Palette::floors[i];
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(true);
			}
			else {
				_slots[i]->_object = std::weak_ptr<Object>();
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(false);
			}
		}
		return;
	}

	if (_type == ObjectType::Building) {
		for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y + 1); i++) {
			if (i < Components::Palette::buildings.size()) {
				_slots[i]->_object = Components::Palette::buildings[i];
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(true);
			}
			else {
				_slots[i]->_object = std::weak_ptr<Object>();
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(false);
			}
		}
		return;
	}

	if (_type == ObjectType::Wall) {
		for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y + 1); i++) {
			if (i < wallset->_texture->_texture->getSize().y / 32) {
				_slots[i]->_object = Components::Palette::walls[i];
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(true);
			}
			else {
				_slots[i]->_object = std::weak_ptr<Object>();
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(false);
			}
		}
		return;
	}

	std::vector<std::shared_ptr<GameObject>> prefabs = prefabs_manager->getPrefabs(_type);

	for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y+1); i++) {
		if (i < prefabs.size()) {
			if(prefabs[i]->getAnimations().expired())
				DebugLog(L"Prefab " + prefabs[i]->_name + L" has no animations");
			_slots[i]->_object = prefabs[i];
			_slots[i]->_animator = std::make_shared<Animator>((prefabs[i]->getAnimations().expired())? _emptySlotAnimation : prefabs[i]->getAnimations());
			_slots[i]->_animator->play();
			_slots[i]->setActive(true);
		}
		else {
			_slots[i]->_object = std::weak_ptr<Object>();
			_slots[i]->_animator = nullptr;
			_slots[i]->setActive(false);
		}
	}
}

void Slots::updateObjects() {

	int startIndex;
	
	if(_type == ObjectType::Terrain)
		startIndex = _scrollbar->getValue() / (160 + _inner_margin) * _slotsCount.x;
	else if (_type == ObjectType::Building)
		startIndex = _scrollbar->getValue() / (240 + _inner_margin) * _slotsCount.x;
	else if (_type == ObjectType::Wall || _type == ObjectType::Floor || _type == ObjectType::Door || _type == ObjectType::Window || _type == ObjectType::WallMounted)
		startIndex = _scrollbar->getValue() / (120 + _inner_margin) * _slotsCount.x;
	else
		startIndex = _scrollbar->getValue() / (80 + _inner_margin) * _slotsCount.x;

	selectSlot(_selectedSlotId);

	if (_type == ObjectType::Terrain) {

		for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y + 1); i++) {
			if (i + startIndex < Components::Palette::terrains.size()) {
				_slots[i]->_object = Components::Palette::terrains[i + startIndex];
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(true);
			}
			else {
				_slots[i]->_object = std::weak_ptr<Object>();
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(false);
			}
		}

		return;
	}


	if (_type == ObjectType::Floor) {
		for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y + 1); i++) {
			if (i + startIndex < FloorSlot::_floorset->_texture->getSize().x / 64) {
				_slots[i]->_object = Components::Palette::floors[i + startIndex];
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(true);
			}
			else {
				_slots[i]->_object = std::weak_ptr<Object>();
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(false);
			}
		}
		return;
	}

	if (_type == ObjectType::Wall) {
		for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y + 1); i++) {
			if (i + startIndex < wallset->_texture->_texture->getSize().y / 32) {
				_slots[i]->_object = Components::Palette::walls[i + startIndex];
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(true);
			}
			else {
				_slots[i]->_object = std::weak_ptr<Object>();
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(false);
			}
		}
		return;
	}


	if (_type == ObjectType::Building) {
		for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y + 1); i++) {
			if (i + startIndex < Components::Palette::buildings.size()) {
				if (_slots[i]->_object.lock() != Components::Palette::buildings[i + startIndex]) {
					_slots[i]->_object = Components::Palette::buildings[i + startIndex];
					std::dynamic_pointer_cast<BuildingSlot>(_slots[i])->generate();
					_slots[i]->_animator = nullptr;
					_slots[i]->setActive(true);
				}
				
			}
			else {
				_slots[i]->_object = std::weak_ptr<Object>();
				_slots[i]->_animator = nullptr;
				_slots[i]->setActive(false);
			}
		}
		return;
	}

	std::vector<std::shared_ptr<GameObject>> prefabs = prefabs_manager->getPrefabs(_type);
	
	for (int i = 0; i < (_slotsCount.x) * (_slotsCount.y+1); i++) {
		if (i + startIndex < prefabs.size()) {
			_slots[i]->_object = prefabs[i + startIndex];
			_slots[i]->_animator = std::make_shared<Animator>((prefabs[i]->getAnimations().expired()) ? _emptySlotAnimation : prefabs[i]->getAnimations());
			_slots[i]->_animator->play();
			_slots[i]->setActive(true);
		}
		else {
			_slots[i]->_object = std::weak_ptr<Object>();
			_slots[i]->_animator = nullptr;
			_slots[i]->setActive(false);
		}
	}
}

void Slots::setCategory(ObjectType type) {

	if (_type != type) {
		_type = type;
		selectSlot(-1);
	}

	// +1 because scrollbar takes one slot space
	if (_type == ObjectType::Terrain) {
		createSlots(sf::Vector2i(3, 3));
		_rect.size = sf::Vector2i(600 - 2 * _main_margin, _slotsCount.y * (160 + _inner_margin) + 2 * _main_margin + _top_margin + _outer_margin);
	}
	else if (_type == ObjectType::Floor) {
		createSlots(sf::Vector2i(4, 5));
		_rect.size = sf::Vector2i(600 - 2 * _main_margin, _slotsCount.y * (120 + _inner_margin) + 2 * _main_margin + _top_margin + _outer_margin);
	}
	else if (_type == ObjectType::Building) {
		createSlots(sf::Vector2i(2, 2));
		_rect.size = sf::Vector2i(600 - 2 * _main_margin, _slotsCount.y * (240 + _inner_margin) + 2 * _main_margin + _top_margin + _outer_margin);
	}
	else if (_type == ObjectType::Wall || _type == ObjectType::Door || _type == ObjectType::Window || _type == ObjectType::WallMounted) {
		createSlots(sf::Vector2i(4, 6));
		_rect.size = sf::Vector2i(600 - 2 * _main_margin, _slotsCount.y * (120 + _inner_margin) + 2 * _main_margin + _top_margin + _outer_margin);
	}
	else {
		createSlots(sf::Vector2i(6, 9));
		_rect.size = sf::Vector2i(600 - 2 * _main_margin, _slotsCount.y * (80 + _inner_margin) + 2 * _main_margin + _top_margin + _outer_margin);
	}


	generateScrollbar();
	loadObjects();

	setPosition(getPosition() - sf::Vector2i(_main_margin, _main_margin));
}

void Slots::setFunction(std::function<void(std::shared_ptr<Slot> slot, int selectedSlotId)> function) {


	for (int i = 0; i < _slots.size(); i++) {

		std::shared_ptr<Slot> slot = _slots[i];

		slot->_onclick_func = [this, slot, i, function]() {

			if (!slot->_object.expired()) {
				int slotHeight;
				if(_type == ObjectType::Terrain)
					slotHeight = 160 + _inner_margin;
				else if (_type == ObjectType::Building)
					slotHeight = 240 + _inner_margin;
				else if(_type == ObjectType::Wall || _type == ObjectType::Floor || _type == ObjectType::Door || _type == ObjectType::Window || _type == ObjectType::WallMounted)
					slotHeight = 120 + _inner_margin;
				else
					slotHeight = 80 + _inner_margin;
				int startIndex = _scrollbar->getValue() / (slotHeight) * _slotsCount.x;
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
		std::shared_ptr<Texture> slotInactiveTexture;

		if (_type == ObjectType::Terrain) {
			_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 3 * 160) / 4;
			slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160.png");
			slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160_hover.png");
			slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160_press.png");
			slotInactiveTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_160_inactive.png");
		}
		else if (_type == ObjectType::Building) {
			_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 2 * 240) / 3;
			slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_240.png");
			slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_240_hover.png");
			slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_240_press.png");
			slotInactiveTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_240_inactive.png");
		}
		else if (_type == ObjectType::Wall || _type == ObjectType::Floor || _type == ObjectType::Door || _type == ObjectType::Window || _type == ObjectType::WallMounted) {
			_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 4 * 120) / 5;
			slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_120.png");
			slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_120_hover.png");
			slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_120_press.png");
			slotInactiveTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_120_inactive.png");
		}
		else {
			_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 6 * 80) / 7;
			slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80.png");
			slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80_hover.png");
			slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80_press.png");
			slotInactiveTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\slot_80_inactive.png");
		}

		_selectedSlot->_texture = slotTexture;
		_selectedSlot->_hoverTexture = slotHoverTexture;
		_selectedSlot->_pressTexture = slotPressTexture;
	}

	_selectedSlotId = selectedSlotId;

	if (_selectedSlotId < 0)
		return;

	int slotHeight;
	if(_type == ObjectType::Terrain)
		slotHeight = 160 + _inner_margin;
	else if(_type == ObjectType::Building)
		slotHeight = 240 + _inner_margin;
	else if(_type == ObjectType::Wall || _type == ObjectType::Floor || _type == ObjectType::Door || _type == ObjectType::Window || _type == ObjectType::WallMounted)
		slotHeight = 120 + _inner_margin;
	else
		slotHeight = 80 + _inner_margin;
	int startIndex = _scrollbar->getValue() / slotHeight * _slotsCount.x;
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
			_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 3 * 160) / 4;
			slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_160.png");
			slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_160_hover.png");
			slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_160_press.png");
		}
		else if (_type == ObjectType::Building) {
			_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 2 * 240) / 3;
			slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_240.png");
			slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_240_hover.png");
			slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_240_press.png");
		}
		else if(_type == ObjectType::Wall || _type == ObjectType::Floor || _type == ObjectType::Door || _type == ObjectType::Window || _type == ObjectType::WallMounted) {
			_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 4 * 120) / 5;
			slotTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_120.png");
			slotHoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_120_hover.png");
			slotPressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\slots\\selected_120_press.png");
		}
		else {
			_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - 32 - 6 * 80) / 7;
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

	if (sf::IntRect(getSlotsRect()).contains(Cursors::cursor->_position)) {
		for (auto& slot : _slots) {
			slot->cursorHover();
		}
	}
	
	_scrollbar->cursorHover();
}

void Slots::handleEvent(const sf::Event& event) {
	if (sf::IntRect(getSlotsRect()).contains(Cursors::cursor->_position)) {
		for (auto& slot : _slots) {
			slot->handleEvent(event);
		}
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
	Main::render_window->draw(background);

	Main::render_window->draw(*_text);

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
	Main::render_window->setView(view);

	for (auto& slot : _slots) {
		slot->draw();
	}
}

