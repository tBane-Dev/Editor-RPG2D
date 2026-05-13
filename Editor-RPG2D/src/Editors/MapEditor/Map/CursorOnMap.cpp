#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "Cursor.hpp"
#include "Editors/MapEditor/Map/CameraOnMap.hpp"
#include "Editors/MapEditor/Map/Map.hpp"
#include "Editors/Editor.hpp"
#include "Editors/MapEditor/MapEditor.hpp"
#include "Editors/MapEditor/Objects/GameObject.hpp"
#include "Editors/MapEditor/Objects/Monster.hpp"
#include "Editors/MapEditor/Map/GameObjectsOnMap.hpp"
#include <typeinfo>
#include "Animator.hpp"


CursorOnMap::CursorOnMap() {

    window->setView(map_editor->_camera->_view);
    _position = sf::Vector2i(window->mapPixelToCoords(cursor->_position));

	_object = nullptr;
}

CursorOnMap::~CursorOnMap() {

}

void CursorOnMap::update() {


    window->setView(map_editor->_camera->_view);
    _position = sf::Vector2i(window->mapPixelToCoords(cursor->_position));
}

void CursorOnMap::handleEvent(const sf::Event& event) {
	if (_object != nullptr) {
		if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Right) {
			_object = nullptr;
		}

		if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {

			std::shared_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(_object);
			std::shared_ptr<Animations> animations = prefab->getAnimations();
			sf::IntRect frameRect = animations->getFrameRect(0, _frame);

			float frameWidth = (float)(animations->getTexture()->getSize().x / animations->_framesCount);
			float frameHeight = (float)(animations->getTexture()->getSize().y / animations->_animationsCount);

			// position of object on the map, aligning to the grid
			sf::Vector2i position;
			position.x = (_position.x - (int)frameWidth / 2) / Tile::tileSize * Tile::tileSize;
			position.y = (_position.y - (int)frameHeight / 2) / Tile::tileSize * Tile::tileSize;

			if (dynamic_cast<MonsterPrefab*>(prefab.get())) {
				position.x += prefab->getOrigin().x;
				position.y += prefab->getOrigin().y;
			}

			// create object on map by type 
			std::shared_ptr<GameObjectOnMap> objectOnMap;
			
			if(dynamic_pointer_cast<MonsterPrefab>(prefab)) objectOnMap = std::make_shared<Monster>(prefab);
			else objectOnMap = std::make_shared<GameObjectOnMap>(prefab);


			// positioning and adding object to map
			objectOnMap->setPosition(position);
			map_editor->_game_objects->addGameObject(objectOnMap);
		}

	}
}

void CursorOnMap::draw()
{
	
    if(_object == nullptr)
		return;

	if(!(GUI_manager->Element_hovered == map_editor->_map || GUI_manager->Element_hovered == nullptr))
		return;

	window->setView(map_editor->_camera->_view);

	if(dynamic_pointer_cast<GameObject>(_object) != nullptr) {

		std::shared_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(_object);
		std::shared_ptr<Animations> animations = prefab->getAnimations();
		sf::IntRect frameRect = animations->getFrameRect(0, _frame);

		float frameWidth = (float)(animations->getTexture()->getSize().x / animations->_framesCount);
		float frameHeight = (float)(animations->getTexture()->getSize().y / animations->_animationsCount);

		sf::Vector2i position;
		position.x = (_position.x - (int)frameWidth/2) / Tile::tileSize * Tile::tileSize;
		position.y = (_position.y - (int)frameHeight/2) / Tile::tileSize * Tile::tileSize;

		sf::RectangleShape outlineRect(sf::Vector2f(frameRect.size));
		outlineRect.setPosition(sf::Vector2f(position));
		outlineRect.setFillColor(sf::Color::Transparent);
		outlineRect.setOutlineThickness(2);
		outlineRect.setOutlineColor(sf::Color::Green);
		window->draw(outlineRect);

		sf::Sprite sprite(*animations->getTexture()->_texture);
		sprite.setTextureRect(frameRect);

		sprite.setPosition(sf::Vector2f(position));
		window->draw(sprite);
		return;
	}


}

std::shared_ptr<CursorOnMap> cursorOnMap;