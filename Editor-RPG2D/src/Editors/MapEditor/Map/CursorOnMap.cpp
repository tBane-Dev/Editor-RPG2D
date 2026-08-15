#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "Cursor.hpp"
#include "Editors/MapEditor/Map/CameraOnMap.hpp"
#include "Editors/MapEditor/Map/Map.hpp"
#include "EditorsManager.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Objects/GameObject.hpp"
#include "Objects/Terrain.hpp"
#include "Objects/Monster.hpp"
#include "Objects/Nature.hpp"
#include "Objects/Building/Building.hpp"
#include "PrefabsManager.hpp"
#include "Editors/MapEditor/Map/GameObjectsOnMap.hpp"
#include <typeinfo>
#include "Animator.hpp"
#include "DebugLog.hpp"
#include <set>
#include "WindowsManager.hpp"

CursorOnMap::CursorOnMap() : CursorWithObject() {
    _selectionRect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(0, 0));
}

CursorOnMap::~CursorOnMap() {

}

void CursorOnMap::update() {
    Cursor::update();
    Main::render_window->setView(MapEditor::editor->_camera->_view);
    _globalPosition = sf::Vector2i(Main::render_window->mapPixelToCoords(_position));
}

void CursorOnMap::handleEvent(const sf::Event& event) {

    if (_object.expired()) {
        if (GUI_manager->Element_pressed == MapEditor::editor->_map) {
            if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {


                _isDragging = false;
				_isSelecting = false;
                if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                    for (auto& object : _selectedObjects) {
                        std::shared_ptr<Mesh> mesh = object->_object.lock()->_prefab.lock()->getMesh();

                        sf::Vector2i monsterOffset = sf::Vector2i(0, 0);
                        if (!object->_object.lock()->_prefab.expired()) {
                            if (object->_object.lock()->_prefab.lock()->_type == ObjectType::Monster) {
                                std::shared_ptr<Monster> monster = std::dynamic_pointer_cast<Monster>(object->_object.lock());
                                if (monster->_prefab.lock()->getCollider()->_type == ColliderType::Circular) {
                                    monsterOffset = monster->_prefab.lock()->getOrigin();
                                }
                            }
                        }

                        if (mesh && mesh->isPointInside(MapEditor::editor->_cursor_on_map->_globalPosition, object->_object.lock()->_position - monsterOffset)) {

                            _isDragging = true;

                            for (auto& o : _selectedObjects) {
                                if (!o->_object.expired()) {
                                    o->_offset = MapEditor::editor->_cursor_on_map->_globalPosition - o->_object.lock()->_position;
                                }
                            }

                            return;
                        }
                    }
                }

                if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))) {
                    for (auto& object : _selectedObjects) {
                        if (!object->_object.expired()) {
                            object->_object.lock()->_isSelected = false;
                        }
                    }
                    _selectedObjects.clear();

                }

                std::shared_ptr<GameObjectOnMap> selectedGameObject = nullptr;
                for (auto& object : MapEditor::editor->_game_objects->_visibleGameObjectsOnMap) {
                    std::shared_ptr<Mesh> mesh = object->_prefab.lock()->getMesh();

                    sf::Vector2i monsterOffset = sf::Vector2i(0, 0);
                    if (!object->_prefab.expired()) {
                        if (object->_prefab.lock()->_type == ObjectType::Monster) {
                            std::shared_ptr<Monster> monster = std::dynamic_pointer_cast<Monster>(object);
                            if (monster->_prefab.lock()->getCollider()->_type == ColliderType::Circular) {
                                monsterOffset = monster->_prefab.lock()->getOrigin();
                            }
                        }
                    }

                    if (mesh && mesh->isPointInside(MapEditor::editor->_cursor_on_map->_globalPosition, object->_position - monsterOffset)) {
                        selectedGameObject = object;
                    }
                }

                if (selectedGameObject) {

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                        if (std::find_if(_selectedObjects.begin(), _selectedObjects.end(),
                            [&](const auto& selectedObject) {
                                return selectedObject->_object.lock() == selectedGameObject;
                            }
                        ) == _selectedObjects.end()) {
                            selectedGameObject->_isSelected = true;
                            _selectedObjects.push_back(std::make_shared<SelectedGameObjectOnMap>(selectedGameObject, MapEditor::editor->_cursor_on_map->_globalPosition - selectedGameObject->_position));
                        }
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {

                        auto it = std::find_if(
                            _selectedObjects.begin(),
                            _selectedObjects.end(),
                            [&](const auto& selectedObject) {
                                return selectedObject->_object.lock().get() == selectedGameObject.get();
                            }
                        );

                        if (it != _selectedObjects.end()) {
                            selectedGameObject->_isSelected = false;
                            _selectedObjects.erase(it);
                        }
                        else {
                            selectedGameObject->_isSelected = true;
                            _selectedObjects.push_back(std::make_shared<SelectedGameObjectOnMap>(selectedGameObject, MapEditor::editor->_cursor_on_map->_globalPosition - selectedGameObject->_position));
                        }
                    }
                    else {
                        // normal seletion
                        for (auto& object : _selectedObjects)
                            if (!object->_object.expired())
                                object->_object.lock()->_isSelected = false;
                        _selectedObjects.clear();

                        selectedGameObject->_isSelected = true;
                        _selectedObjects.push_back(std::make_shared<SelectedGameObjectOnMap>(selectedGameObject, MapEditor::editor->_cursor_on_map->_globalPosition - selectedGameObject->_position));
                    }


                    //DebugLog(std::to_wstring(_selectedObjects.size()));

                }
                else {

                    _isSelecting = true;

                    _selectionRect.position =
                        MapEditor::editor->_cursor_on_map->_globalPosition;

                    _selectionRect.size = sf::Vector2i(0, 0);
                }

               
            }
            else if (const auto* mm = event.getIf<sf::Event::MouseMoved>(); (mm || MapEditor::editor->_camera->_isMoving) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

                if (_isDragging) {
                    for (auto& object : _selectedObjects) {

                        if (object->_object.expired())
                            continue;

                        std::shared_ptr<GameObjectOnMap> gameObject = object->_object.lock();

                        sf::Vector2i oldPos =
                            (gameObject->_prefab.lock()->_type == ObjectType::Monster)
                            ? std::dynamic_pointer_cast<Monster>(gameObject)->_basePosition
                            : gameObject->_position;


                        if (gameObject->_type == ObjectType::Building) {

                            std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(gameObject);
                            building->removeWallsFromGameObjects();
                        }


                        std::shared_ptr<Chunk> chunk = MapEditor::editor->_map->getChunkByGlobalPosition(oldPos);

                        if (chunk)
                            chunk->removeGameObjectOnMap(gameObject);

                        sf::Vector2i newPos = MapEditor::editor->_cursor_on_map->_globalPosition - object->_offset;
                         
                        newPos.x = (newPos.x / Tile::tileSize) * Tile::tileSize;
                        newPos.y = (newPos.y / Tile::tileSize) * Tile::tileSize;

                        gameObject->setPosition(newPos);


                        chunk = MapEditor::editor->_map->getChunkByGlobalPosition(newPos);

                        if (chunk)
                            chunk->addGameObjectOnMap(gameObject);

                        if (gameObject->_type == ObjectType::Building) {

                            std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(gameObject);
                            building->addWallsToGameObjects();
                        }
                    }

                    return;
                }

                if (!_isSelecting)
                    return;

                if (GUI_manager->Element_pressed == MapEditor::editor->_map)
                    _selectionRect.size = sf::Vector2i(_globalPosition.x - _selectionRect.position.x, _globalPosition.y - _selectionRect.position.y);

                if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))) {
                    for (auto& object : _selectedObjects) {
                        if (!object->_object.expired()) {
                            object->_object.lock()->_isSelected = false;
                        }
                    }
                    _selectedObjects.clear();
                }



                if (_selectionRect.size.x != 0 || _selectionRect.size.y != 0) {
                    std::vector<std::shared_ptr<GameObjectOnMap>> selectedGameObjects;
                    for (auto& object : MapEditor::editor->_game_objects->_visibleGameObjectsOnMap) {
                        std::shared_ptr<Mesh> mesh = object->_prefab.lock()->getMesh();

                        sf::Vector2i monsterOffset = sf::Vector2i(0, 0);
                        if (!object->_prefab.expired()) {
                            if (object->_prefab.lock()->_type == ObjectType::Monster) {
                                std::shared_ptr<Monster> monster = std::dynamic_pointer_cast<Monster>(object);
                                if (monster->_prefab.lock()->getCollider()->_type == ColliderType::Circular) {
                                    monsterOffset = monster->_prefab.lock()->getOrigin();
                                }
                            }
                        }

                        if (mesh && mesh->isInsideRect(_selectionRect, object->_position - monsterOffset)) {
                            selectedGameObjects.push_back(object);
                        }
                    }

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                        for (auto& selected : _selectedObjects) {
                            if (!selected->_object.expired()) {
                                selected->_object.lock()->_isSelected = false;
                            }
                        }

                        _selectedObjects.clear();

                        for (auto& prev : _prevSelectedObjects) {
                            if (!prev->_object.expired()) {
                                prev->_object.lock()->_isSelected = true;
                                _selectedObjects.push_back(prev);
                            }
                        }

                        for (auto& object : selectedGameObjects) {

                            auto it = std::find_if(
                                _selectedObjects.begin(),
                                _selectedObjects.end(),
                                [&](const std::shared_ptr<SelectedGameObjectOnMap>& selected) {
                                    return !selected->_object.expired() &&
                                        selected->_object.lock().get() == object.get();
                                }
                            );

                            if (it == _selectedObjects.end()) {
                                object->_isSelected = true;
                                _selectedObjects.push_back(std::make_shared<SelectedGameObjectOnMap>(object, MapEditor::editor->_cursor_on_map->_globalPosition - object->_position));
                            }
                        }
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {

                        for (auto& selected : _selectedObjects) {
                            if (!selected->_object.expired()) {
                                selected->_object.lock()->_isSelected = false;
                            }
                        }

                        _selectedObjects.clear();


                        for (auto& prev : _prevSelectedObjects) {
                            if (prev->_object.expired())
                                continue;

                            prev->_object.lock()->_isSelected = true;
                            _selectedObjects.push_back(prev);
                        }

                        for (auto& object : selectedGameObjects) {
                            auto prevIt = std::find_if(
                                _prevSelectedObjects.begin(),
                                _prevSelectedObjects.end(),
                                [&](const std::shared_ptr<SelectedGameObjectOnMap>& selected) {
                                    return !selected->_object.expired() &&
                                        selected->_object.lock().get() == object.get();
                                }
                            );

                            if (prevIt != _prevSelectedObjects.end()) {

                                object->_isSelected = false;
                                auto it = std::find_if(
                                    _selectedObjects.begin(),
                                    _selectedObjects.end(),
                                    [&](const std::shared_ptr<SelectedGameObjectOnMap>& selected) {
                                        return !selected->_object.expired() &&
                                            selected->_object.lock().get() == object.get();
                                    }
                                );

                                if (it != _selectedObjects.end()) {
                                    _selectedObjects.erase(it);
                                }
                            }
                            else {
                                object->_isSelected = true;
                                _selectedObjects.push_back(std::make_shared<SelectedGameObjectOnMap>(object, MapEditor::editor->_cursor_on_map->_globalPosition - object->_position));
                            }
                        }
                    }
                    else {
                        for (auto& object : selectedGameObjects) {
                            object->_isSelected = true;
                            _selectedObjects.push_back(std::make_shared<SelectedGameObjectOnMap>(object, MapEditor::editor->_cursor_on_map->_globalPosition - object->_position));
                        }
                    }


                }
            }
        }
    }
    
    if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Right) {
        if (!_selectedObjects.empty()) {
            for (auto& object : _selectedObjects) {
                if (!object->_object.expired()) {
                    object->_object.lock()->_isSelected = false;
                }
            }
            _selectedObjects.clear();
            return;
        }
    }
    
    if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
        
        _prevSelectedObjects.clear();
        for (auto& object : _selectedObjects) {
            if (!object->_object.expired()) {
                if (object->_object.lock()->_isSelected) {
                    _prevSelectedObjects.push_back(object);
                }
            }
        }

		_isDragging = false;
        _isSelecting = false;
        _selectionRect.size = sf::Vector2i(0, 0);
    }

	if(_object.expired())
		return;

    if (Main::windows_manager->get_back())
        return;

    if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Right) {
        if (MapEditor::editor->_palette->_tools->_selectedTool != nullptr) {
            MapEditor::editor->_palette->_tools->setTool(MapEditor::editor->_palette->_tools->_tools[0], ToolType::None);
        }

        if (MapEditor::editor->_palette->_slots->_selectedSlot != nullptr) {
            MapEditor::editor->_palette->_slots->selectSlot(-1);
        }

        if (_state == Cursors::CursorWithObjectState::Drawing) {
            _state = Cursors::CursorWithObjectState::Idle;
        }

        _object = std::weak_ptr<Object>();
        return;
    }

    if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
           
        if (_state == Cursors::CursorWithObjectState::Drawing) {
            _state = Cursors::CursorWithObjectState::Idle;
        }
			
    }

    if (!(GUI_manager->Element_pressed == nullptr || GUI_manager->Element_pressed == MapEditor::editor->_map))
        return;

	if (_object.lock()->_type == ObjectType::Terrain) {

        bool conditionToDraw = 
            GUI_manager->Element_hovered == MapEditor::editor->_map &&
            sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && 
            MapEditor::editor->_map->getChunkByGlobalPosition() != nullptr &&
            (MapEditor::editor->_palette->_tools->_toolType == ToolType::Circle || MapEditor::editor->_palette->_tools->_toolType == ToolType::Rect);

		if (conditionToDraw) {

			std::shared_ptr<Map> mapa = std::dynamic_pointer_cast<Map>(MapEditor::editor->_map);

            sf::Vector2i tileCoords;
			tileCoords.x = (_globalPosition.x - mapa->getRect().position.x) / Tile::tileSize;
			tileCoords.y = (_globalPosition.y - mapa->getRect().position.y) / Tile::tileSize;

			int type = std::dynamic_pointer_cast<Terrain>(MapEditor::editor->_cursor_on_map->_object.lock())->_id;

			std::set<std::shared_ptr<Chunk>> chunksToEdit;

            int brushSize = MapEditor::editor->_palette->_brushSize;
            std::vector<std::vector<bool>> brush;
            
            if (MapEditor::editor->_palette->_tools->_toolType == ToolType::Rect)
                brush = Cursors::square_brushes[brushSize];

            if (MapEditor::editor->_palette->_tools->_toolType == ToolType::Circle)
                brush = Cursors::circle_brushes[brushSize];
           

            for (int yy = 0; yy < brush.size(); yy++) {
                for (int xx = 0; xx < brush[yy].size(); xx++) {
                    if (brush[yy][xx]) {

						int tileX = tileCoords.x + (xx - brush[yy].size() / 2);
						int tileY = tileCoords.y + (yy - brush.size() / 2);

                        std::shared_ptr<Chunk> c = mapa->getChunkByTileGlobalCoords(tileX, tileY);
                        if (!c) continue;
               
                        std::shared_ptr<Tile> t = c->getTileByTileGlobalCoords(tileX, tileY);
                        if (!t) continue;

                        t->_type = type;
                        chunksToEdit.insert(c);

                        if (mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y - 1)) chunksToEdit.insert(mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y - 1));
                        if (mapa->getChunkByCoords(c->_coords.x, c->_coords.y - 1)) chunksToEdit.insert(mapa->getChunkByCoords(c->_coords.x, c->_coords.y - 1));
                        if (mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y - 1)) chunksToEdit.insert(mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y - 1));

                        if (mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y)) chunksToEdit.insert(mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y));
                        if (mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y)) chunksToEdit.insert(mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y));

                        if (mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y + 1)) chunksToEdit.insert(mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y + 1));
                        if (mapa->getChunkByCoords(c->_coords.x, c->_coords.y + 1)) chunksToEdit.insert(mapa->getChunkByCoords(c->_coords.x, c->_coords.y + 1));
                        if (mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y + 1)) chunksToEdit.insert(mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y + 1));
                    }
                }
            }

			for (auto& c : chunksToEdit)
				c->generateVertexArray(
					mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y - 1), mapa->getChunkByCoords(c->_coords.x, c->_coords.y - 1), mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y - 1),
					mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y), mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y),
					mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y + 1), mapa->getChunkByCoords(c->_coords.x, c->_coords.y + 1), mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y + 1)
				);
		
            if(!chunksToEdit.empty())
                _state = Cursors::CursorWithObjectState::Drawing;
        }

		return;
	}

	if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {

		if (GUI_manager->Element_pressed == MapEditor::editor->_map) {
			std::shared_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(_object.lock());
			std::shared_ptr<Animations> animations = prefab->getAnimations().lock();
            
            float frameWidth = 128;
            float frameHeight = 128;

            if (animations) {
				sf::IntRect frameRect = animations->getFrameRect(0, 0);
                frameWidth = (float)(frameRect.size.x);
                frameHeight = (float)(frameRect.size.y);
            }

            if (prefab->_type == ObjectType::Building) {
				std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(prefab);
                frameWidth = bp->getPreviewOutsideTexture()->getSize().x;
                frameHeight = bp->getPreviewOutsideTexture()->getSize().y;
            }

			// position of object on the map, aligning to the grid
			sf::Vector2i position;
			position.x = (_globalPosition.x - (int)frameWidth / 2) / Tile::tileSize * Tile::tileSize;
			position.y = (_globalPosition.y - (int)frameHeight / 2) / Tile::tileSize * Tile::tileSize;

			if (prefab->_type == ObjectType::Monster) {
				position.x += prefab->getOrigin().x;
				position.y += prefab->getOrigin().y;
			}

            if (prefab->_type == ObjectType::Building) {
				position.y += 96; // offset for building preview
            }

			// create object on map by type 
			std::shared_ptr<GameObjectOnMap> objectOnMap;

            if (prefab->_type == ObjectType::Building) {
                static int i = 0;
				std::shared_ptr<BuildingPrefab> buildingPrefab = std::make_shared<BuildingPrefab>(L"building " + std::to_wstring(i++), *std::dynamic_pointer_cast<BuildingPrefab>(prefab));
                objectOnMap = std::make_shared<Building>(buildingPrefab);
				std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(objectOnMap);
                objectOnMap->setPosition(position);
                building->generate(position);
                building->addWallsToGameObjects();
                prefabs_manager->addPrefab(buildingPrefab);
                MapEditor::editor->_map->getChunkByGlobalPosition(position)->addGameObjectOnMap(objectOnMap);
                MapEditor::editor->_map->setVisibleChunks();
                return;
            }
			else if (prefab->_type == ObjectType::Monster) objectOnMap = std::make_shared<Monster>(prefab);
			else if (prefab->_type == ObjectType::Nature) objectOnMap = std::make_shared<Nature>(prefab);
			else objectOnMap = std::make_shared<GameObjectOnMap>(prefab);

			// positioning and adding object to map
			objectOnMap->setPosition(position);
			MapEditor::editor->_map->getChunkByGlobalPosition(position)->addGameObjectOnMap(objectOnMap);
            MapEditor::editor->_map->setVisibleChunks();
			return;
		}

	}

}


void CursorOnMap::draw()
{

	if(!(GUI_manager->Element_pressed == MapEditor::editor->_map || GUI_manager->Element_hovered == MapEditor::editor->_map || GUI_manager->Element_hovered == nullptr))
		return;

    if(MapEditor::editor->_main_menu->_state != Components::MainMenuStates::Closed)
		return;

    Main::render_window->setView(MapEditor::editor->_camera->_view);

    if (_selectionRect.size.x != 0 || _selectionRect.size.y != 0) {
        sf::RectangleShape selectionRectShape(sf::Vector2f(_selectionRect.size));
        selectionRectShape.setPosition(sf::Vector2f(_selectionRect.position));
        selectionRectShape.setFillColor(sf::Color(0, 0, 255, 50));
        selectionRectShape.setOutlineThickness(1);
        selectionRectShape.setOutlineColor(sf::Color(0, 0, 255, 200));
		Main::render_window->draw(selectionRectShape);

        return;
    }

    if (_object.expired())
        return;

    if (_object.lock()->_type == ObjectType::Terrain) {

		int brushSize = MapEditor::editor->_palette->_brushSize;
		
        std::vector<std::vector<bool>> brush;

        if (MapEditor::editor->_palette->_tools->_toolType == ToolType::Rect)
            brush = Cursors::square_brushes[brushSize];
        
		if (MapEditor::editor->_palette->_tools->_toolType == ToolType::Circle)
            brush = Cursors::circle_brushes[brushSize];

		std::shared_ptr<Map> mapa = std::dynamic_pointer_cast<Map>(MapEditor::editor->_map);

        for(int yy = 0; yy < brush.size(); yy++) {
            for(int xx = 0; xx < brush[yy].size(); xx++) {
                if (brush[yy][xx]) {

					int tx = _globalPosition.x / Tile::tileSize + (xx - brush[yy].size() / 2);
					int ty = _globalPosition.y / Tile::tileSize + (yy - brush.size() / 2);

                    std::shared_ptr<Chunk> c = mapa->getChunkByTileGlobalCoords(tx, ty);
                    if (!c) continue;

                    std::shared_ptr<Tile> t = c->getTileByTileGlobalCoords(tx, ty);
                    if (!t) continue;

                    sf::RectangleShape outlineRect(sf::Vector2f(Tile::tileSize, Tile::tileSize));
                    outlineRect.setPosition(sf::Vector2f(
                        tx * Tile::tileSize,
                        ty * Tile::tileSize
                    ));
                    outlineRect.setFillColor(sf::Color(255, 47, 47, 127));
                    //outlineRect.setOutlineThickness(2);
                    //outlineRect.setOutlineColor(sf::Color::Green);
                    Main::render_window->draw(outlineRect);
                }
            }
		}

        return;
    }

	if(dynamic_pointer_cast<GameObject>(_object.lock()) != nullptr) {

		std::shared_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(_object.lock());
		std::shared_ptr<Animations> animations = prefab->getAnimations().lock();
        
        float frameWidth = 128;
        float frameHeight = 128;
		sf::IntRect frameRect(sf::Vector2i(0, 0), sf::Vector2i(frameWidth, frameHeight));

        if (animations) {
            frameRect = animations->getFrameRect(0, 0);
            frameWidth = (float)(frameRect.size.x);
            frameHeight = (float)(frameRect.size.y);
        }

        if (prefab->_type == ObjectType::Building) {
			std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(prefab);
            frameWidth = (float)(buildingPrefab->getPreviewOutsideTexture()->getSize().x);
            frameHeight = (float)(buildingPrefab->getPreviewOutsideTexture()->getSize().y);
			frameRect.size = sf::Vector2i((int)frameWidth, (int)frameHeight);
        }

		sf::Vector2i position;
		position.x = (_globalPosition.x - (int)frameWidth/2) / Tile::tileSize * Tile::tileSize;
		position.y = (_globalPosition.y - (int)frameHeight/2) / Tile::tileSize * Tile::tileSize;

		sf::RectangleShape outlineRect(sf::Vector2f(frameRect.size));
		outlineRect.setPosition(sf::Vector2f(position));
		outlineRect.setFillColor(sf::Color::Transparent);
		outlineRect.setOutlineThickness(2);
		outlineRect.setOutlineColor(sf::Color::Green);
        Main::render_window->draw(outlineRect);

        if (animations) {
            sf::Sprite sprite(*animations->getTexture()->_texture);
            sprite.setTextureRect(frameRect);

            sprite.setPosition(sf::Vector2f(position));
            Main::render_window->draw(sprite);
            return;
        }
		
        if (prefab->_type == ObjectType::Building) {

            std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(prefab);

            if (!buildingPrefab)
                return;

            std::shared_ptr<sf::Texture> texture = buildingPrefab->getPreviewOutsideTexture();

            if (!texture)
                return;

            sf::Sprite sprite(*texture);

            sprite.setPosition(sf::Vector2f(position));
            Main::render_window->draw(sprite);

            return;
        }
	}


}
