#include "Components/Palette/Slots.hpp"
#include "Editors/MapEditor/Objects/GameObject.hpp"
#include "Editors/MapEditor/Objects/Terrain.hpp"
#include "PrefabsManager.hpp"
#include "Window.hpp"
#include "Theme.hpp"
#include "DebugLog.hpp"
#include "Editors/MapEditor/MapEditor.hpp"
#include "Editors/MapEditor/Map/Tileset.hpp"

Slot::Slot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position) : ButtonWithSprite(texture, hoverTexture, pressTexture, position) {

}

Slot::~Slot() {

}

sf::Vector2i TerrainSlot::tileRects[3][3] =
{
	{ {8, 0},  {12, 0}, {4, 0} },
	{ {10, 0}, {15, 0}, {5, 0} },
	{ {2, 0},  {3, 0},  {1, 0} }
};

TerrainSlot::TerrainSlot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position) : Slot(texture, hoverTexture, pressTexture, position) {
	_topLeftSprite = nullptr;
	_topSprite = nullptr;
	_topRightSprite = nullptr;
	_leftSprite = nullptr;
	_centerSprite = nullptr;
	_rightSprite = nullptr;
	_bottomLeftSprite = nullptr;
	_bottomSprite = nullptr;
	_bottomRightSprite = nullptr;
}

TerrainSlot::~TerrainSlot() {

}

void TerrainSlot::cursorHover() {
	ButtonWithSprite::cursorHover();
}

void TerrainSlot::handleEvent(const sf::Event& event) {
	ButtonWithSprite::handleEvent(event);
}

void TerrainSlot::update() {
	ButtonWithSprite::update();
}

void TerrainSlot::draw() {
	Slot::draw();

	if (_object == nullptr)
		return;

	std::shared_ptr<Terrain> terrain = std::dynamic_pointer_cast<Terrain>(_object);

	if (terrain == nullptr)
		return;
	

	

	sf::RenderStates states;
	states.shader = &*shader;

	shader->setUniform("resolution", sf::Vector2f(window->getSize()));
	shader->setUniform("time", currentTime.asSeconds());
	shader->setUniform("startPos", sf::Vector2f(_rect.position));

	std::shared_ptr<Texture> texture = textures_manager->getTexture(L"assets\\tex\\tileset.png");

	sf::Vector2f size(132.f, 132.f);
	sf::Vector2f tileSize(size.x / 3.f, size.y / 3.f);
	sf::Vector2f offset((160.f - size.x) / 2.f, (160.f - size.y) / 2.f);

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {

			sf::Sprite sprite(*texture->_texture);
			sprite.setScale(sf::Vector2f(tileSize.x / 16.f,tileSize.y / 16.f));
			sprite.setPosition(sf::Vector2f(_rect.position) + offset + sf::Vector2f(x * tileSize.x, y * tileSize.y));
			sprite.setTextureRect(sf::IntRect(sf::Vector2i(tileRects[y][x].x * 64, terrain->_id * 64), sf::Vector2i(16, 16)));

			window->draw(sprite, states);
		}
	}
}

GameObjectSlot::GameObjectSlot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, sf::Vector2i position) : Slot(texture, hoverTexture, pressTexture, position) {
	_object = nullptr;
	_animator = nullptr;
}

GameObjectSlot::~GameObjectSlot() {

}

void GameObjectSlot::cursorHover() {
	ButtonWithSprite::cursorHover();
}

void GameObjectSlot::handleEvent(const sf::Event& event) {
	ButtonWithSprite::handleEvent(event);
}

void GameObjectSlot::update() {
	ButtonWithSprite::update();
	if(_animator != nullptr) {
		_animator->update();
	}

}

void GameObjectSlot::draw() {

	Slot::draw();

	if (_object == nullptr)
		return;

	if (_animator != nullptr) {

		std::shared_ptr<Animations> animations = _animator->getAnimations();
		sf::IntRect frameRect = animations->getFrameRect(_animator->_animation, _animator->_frame);

		_objectTexture = animations->getTexture();

		_objectSprite = std::make_shared<sf::Sprite>(*_objectTexture->_texture);
		_objectSprite->setTextureRect(frameRect);

		float frameWidth = (float)(_objectTexture->getSize().x / animations->_framesCount);
		float frameHeight = (float)(_objectTexture->getSize().y / animations->_animationsCount);

		_objectSprite->setScale(sf::Vector2f(80.f / frameWidth, 80.f / frameHeight));
		_objectSprite->setPosition(sf::Vector2f(_rect.position) + sf::Vector2f(40, 40));
		_objectSprite->setOrigin(sf::Vector2f(frameWidth / 2.f, frameHeight / 2.f));

		window->draw(*_objectSprite);
	}
}

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

void Slots::loadObjects() {

	selectSlot(_selectedSlotId);

	if (_type == ObjectType::Terrain) {

		std::vector<std::shared_ptr<Terrain>> terrains;
		for(int i=0;i<map_editor->_tileset->groups.size(); i++) {

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
		for (int i = 0; i < map_editor->_tileset->groups.size(); i++) {

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
	_type = type;


	// +1 because scrollbar takes one slot space
	if(_type == ObjectType::Terrain) {
		createSlots(sf::Vector2i(3, 3));
		_rect.size = sf::Vector2i(600 - 2 * _main_margin, _slotsCount.y * (160 + _inner_margin) + 2 * _main_margin + _top_margin + _outer_margin);

	}
	else {
		createSlots(sf::Vector2i(6, 9));
		_rect.size = sf::Vector2i(600 - 2 * _main_margin, _slotsCount.y * (80 + _inner_margin) + 2 * _main_margin + _top_margin + _outer_margin);

	}


	sf::Vector2i scrollbarPosition = sf::Vector2i(_rect.position.x + _rect.size.x - 32 - _outer_margin, _rect.position.y + _main_margin + _top_margin + 2 * _inner_margin);
	sf::Vector2i scrollbarSize = sf::Vector2i(32, _rect.size.y - 2 * _main_margin - _top_margin - _inner_margin);
	
	int rowsTotal;
	if (_type == ObjectType::Terrain)
		rowsTotal = (int)std::ceil(map_editor->_tileset->groups.size()/_slotsCount.x);
	else
		rowsTotal = (int)std::ceil((float)prefabs_manager->getPrefabs(_type).size() / (float)_slotsCount.x);
	
	int rowsVisible = _slotsCount.y;
	int rowHeight = (_type == ObjectType::Terrain)? 160 + _inner_margin : 80 + _inner_margin;	
	int scrollbarValue = 0;
	int scrollbarMaxValue = std::max(0, (rowsTotal - rowsVisible) * rowHeight);
	int scrollbarSliderSize = (_type == ObjectType::Terrain)? _slotsCount.y * (160 + _inner_margin) : _slotsCount.y * (80 + _inner_margin);

	_scrollbar = std::make_shared<Scrollbar>(scrollbarPosition.x, scrollbarPosition.y, scrollbarSize.x, scrollbarSize.y, 0, scrollbarMaxValue, scrollbarSliderSize, scrollbarValue);

	if (_type == ObjectType::Terrain) {
		_scrollbar->setScrollArea(std::make_shared<sf::IntRect>(
			sf::Vector2i(_rect.position.x + _outer_margin, _rect.position.y + _main_margin + _top_margin + _outer_margin),
			sf::Vector2i(_rect.size.x - 32 - 2 * _outer_margin, _rect.size.y - 2 * _main_margin - _top_margin - _outer_margin)),
			(160 + _inner_margin) / 4);
	}
	else {
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

