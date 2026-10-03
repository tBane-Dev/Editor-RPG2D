#include "Editors/BuildingsEditor/CursorOnBuilding.hpp"
#include "Cursor.hpp"
#include "EditorsManager.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/EditableBuilding.hpp"
#include "Objects/GameObject.hpp"
#include "Objects/Floor.hpp"
#include <typeinfo>
#include "DebugLog.hpp"
#include <set>
#include "WindowsManager.hpp"

namespace BuildingsEditor {

    CursorOnBuilding::CursorOnBuilding() : CursorWithObject() {

    }

    CursorOnBuilding::~CursorOnBuilding() {

    }

    bool CursorOnBuilding::canPlaceDoor(sf::Vector2i wallPosition) {

        std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(BuildingsEditor::editor->_building_panel->_building->_building->_prefab.lock());

        bool isInsideWalls =
            wallPosition.y >= 0 && wallPosition.y < bp->_walls.size() &&
            wallPosition.x >= 0 && wallPosition.x < bp->_walls[wallPosition.y].size();

        bool hasEmptyDoorSpace =
            isInsideWalls &&
            wallPosition.x - 1 >= 0 &&
            bp->_walls[wallPosition.y][wallPosition.x - 1] < 0 &&
            bp->_walls[wallPosition.y][wallPosition.x] < 0;

        bool hasRequiredWalls =
            isInsideWalls &&
            bp->_walls[wallPosition.y][wallPosition.x] >= 0 &&
            wallPosition.x - 2 >= 0 &&
            bp->_walls[wallPosition.y][wallPosition.x - 2] >= 0 &&
            wallPosition.x + 1 < bp->_walls[wallPosition.y].size() &&
            bp->_walls[wallPosition.y][wallPosition.x + 1] >= 0;

        bool hasFreeSpaceBelow = isInsideWalls &&
            (
                wallPosition.y + 1 >= bp->_walls.size() ||
                (
                    wallPosition.x - 1 >= 0 &&
                    wallPosition.x < bp->_walls[wallPosition.y + 1].size() &&
                    bp->_walls[wallPosition.y + 1][wallPosition.x - 1] == -1 &&
                    bp->_walls[wallPosition.y + 1][wallPosition.x] == -1
                    )
                );

        return hasEmptyDoorSpace || (hasRequiredWalls && hasFreeSpaceBelow);
    }

    int CursorOnBuilding::getBottomWallY(int x) {
        std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(BuildingsEditor::editor->_building_panel->_building->_building->_prefab.lock());

        int wallY = -1;

        for (int y = int(bp->_walls.size()) - 1; y >= 0; --y) {
            if (bp->_walls[y][x] >= 0) {
                wallY = y;
                break;
            }
        }

        return wallY;
    }

    bool CursorOnBuilding::canPlaceWindow(sf::Vector2i windowPosition) {

        std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(BuildingsEditor::editor->_building_panel->_building->_building->_prefab.lock());

        if (windowPosition.x < 1 || windowPosition.x >= bp->_walls[0].size() - 1)
            return false;

        const int wallY = getBottomWallY(windowPosition.x);
        if (wallY == -1 || windowPosition.y < wallY - bp->_wallHeight + 1 || windowPosition.y > wallY)
            return false;

        int height = bp->_wallHeight;

        for (int y = 0; y < bp->_walls.size(); ++y) {

            if (bp->_walls[y][windowPosition.x] < 0 || bp->_walls[y][windowPosition.x-1] < 0 || bp->_walls[y][windowPosition.x + 1] < 0)
                continue;

            int top = y - height + 1;
            int bottom = y;

            if (windowPosition.y >= top &&
                windowPosition.y <= bottom)
            {
                return true;
            }
        }

        return false;
    }

    void CursorOnBuilding::update() {
        CursorWithObject::update();
    }

    void CursorOnBuilding::handleEvent(const sf::Event& event) {



        if (Main::windows_manager->get_back())
            return;

        if (GUI_manager->Element_hovered == BuildingsEditor::editor->_building_panel->_building && sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && _object.expired()) {
            std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(BuildingsEditor::editor->_building_panel->_building->_building->_prefab.lock());
            bool conditionToRemoveDoors = false;
            std::erase_if(bp->_doors, [&](const std::shared_ptr<Door>& door) {
				bool condition = door->_prefab.lock()->getMesh()->isPointInside(_globalPosition, door->_position); // TO-DO - add the scale
                if (condition) conditionToRemoveDoors = true;
                return condition;
                });
            
            if (conditionToRemoveDoors) {
                std::shared_ptr<Building> building = BuildingsEditor::editor->_building_panel->_building->_building;
                std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(building->_prefab.lock());

                buildingPrefab->generate(building->getPosition(), BuildingsEditor::editor->_building_panel->_building->_scale, building);
                building->generate();

                BuildingsEditor::editor->_building_panel->_game_objects->_visiblePlacedGameObjects.clear();
                BuildingsEditor::editor->_building_panel->_building->_building->addDoorsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addWindowsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addWallsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addSkeletsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addOutsideToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_game_objects->sort();
                return;
            }
                
        }

        bool conditionToRemoveWalls = GUI_manager->Element_hovered == BuildingsEditor::editor->_building_panel->_building && sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && _object.expired();

        if (conditionToRemoveWalls) {

            std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(BuildingsEditor::editor->_building_panel->_building->_building->_prefab.lock());
            if (!bp) return;

            int s = int(32.f * BuildingsEditor::editor->_building_panel->_building->_scale);

            int tx = (_globalPosition.x - BuildingsEditor::editor->_building_panel->_building->getPosition().x) / s;
            int ty = (_globalPosition.y - BuildingsEditor::editor->_building_panel->_building->getPosition().y) / s;

            if (tx < 0 || tx >= bp->_walls[0].size() || ty < 0 || ty >= bp->_walls.size())
                return;

            if (bp->_walls[ty][tx] != -1) {
                bp->_walls[ty][tx] = -1;

                std::shared_ptr<Building> building = BuildingsEditor::editor->_building_panel->_building->_building;
                std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(building->_prefab.lock());
				
				buildingPrefab->generate(building->getPosition(), BuildingsEditor::editor->_building_panel->_building->_scale, building);
                building->generate();
                
                BuildingsEditor::editor->_building_panel->_game_objects->_visiblePlacedGameObjects.clear();
                BuildingsEditor::editor->_building_panel->_building->_building->addDoorsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addWindowsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addWallsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addSkeletsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addOutsideToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_game_objects->sort();
                return;
            }
        }

        if (_object.expired())
            return;

        if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Right) {
            if (auto tools = std::dynamic_pointer_cast<ToolsTerrain>(BuildingsEditor::editor->_palette->_tools); tools && tools->_selectedTool != nullptr) {
                tools->setTool(tools->_tools[0], ToolTerrainType::None);
            }

            if (BuildingsEditor::editor->_palette->_slots->_selectedSlot != nullptr) {
                BuildingsEditor::editor->_palette->_slots->selectSlot(-1);
            }

            if (_state == Cursors::CursorWithObjectState::Drawing) {
                _state = Cursors::CursorWithObjectState::Idle;
            }

            _object = std::weak_ptr<Object>();
            return;
        }

        if (const auto* mbl = event.getIf<sf::Event::MouseButtonReleased>(); mbl && mbl->button == sf::Mouse::Button::Left) {
            if (_state == Cursors::CursorWithObjectState::Drawing) {
                _state = Cursors::CursorWithObjectState::Idle;
            }

        }


        if (!(GUI_manager->Element_pressed == nullptr || GUI_manager->Element_pressed == BuildingsEditor::editor->_building_panel->_building))
            return;

        if (_object.lock()->_type == ObjectType::Floor) {
            std::shared_ptr<ToolsTerrain> tools = std::dynamic_pointer_cast<ToolsTerrain>(BuildingsEditor::editor->_palette->_tools);

            bool conditionToDrawFloor =
                GUI_manager->Element_hovered == BuildingsEditor::editor->_building_panel->_building &&
                sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
                (tools->_toolType == ToolTerrainType::Circle || tools->_toolType == ToolTerrainType::Rect);

            if (conditionToDrawFloor) {
                std::shared_ptr<BuildingsEditor::EditableBuilding> building = BuildingsEditor::editor->_building_panel->_building;

                float scale = building->_scale;
                int floorTileSize = int(16.f * scale);
                sf::IntRect buildingRect = sf::IntRect(building->getPosition(), building->getSize());

                int brushSize = BuildingsEditor::editor->_palette->_brushSize;
                std::vector<std::vector<bool>> brush;

                if (tools->_toolType == ToolTerrainType::Rect)
                    brush = Cursors::square_brushes[brushSize];

                if (tools->_toolType == ToolTerrainType::Circle)
                    brush = Cursors::circle_brushes[brushSize];


                std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(BuildingsEditor::editor->_building_panel->_building->_building->_prefab.lock());
                if (!bp) return;

                int index = std::dynamic_pointer_cast<Floor>(_object.lock())->_id;
                bool editedFloor = false;

                for (int yy = 0; yy < brush.size(); yy++) {
                    for (int xx = 0; xx < brush[yy].size(); xx++) {
                        if (brush[yy][xx]) {

                            int tx = (_globalPosition.x - buildingRect.position.x) / floorTileSize + (xx - brush[yy].size() / 2);
                            int ty = (_globalPosition.y - buildingRect.position.y) / floorTileSize + (yy - brush.size() / 2);

                            if (tx < 0 || tx > bp->_floor[0].size() - 1 || ty < 0 || ty > bp->_floor.size() - 1)
                                continue;

                            bp->_floor[ty][tx] = index;
                            editedFloor = true;
                        }
                    }
                }

                if (editedFloor) {
                    std::shared_ptr<Building> building = BuildingsEditor::editor->_building_panel->_building->_building;
                    std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(building->_prefab.lock());
                    buildingPrefab->generateFloorVertexArray(scale);
                }

            }
            return;
        }



        if (_object.lock()->_type == ObjectType::Wall) {

            bool conditionToDrawWalls = GUI_manager->Element_hovered == BuildingsEditor::editor->_building_panel->_building &&
                sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && _object.lock()->_type == ObjectType::Wall;

            if (conditionToDrawWalls) {



                std::shared_ptr<BuildingsEditor::EditableBuilding> building = BuildingsEditor::editor->_building_panel->_building;

                std::shared_ptr<Building> bb = std::dynamic_pointer_cast<Building>(building->_building);
                std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(bb->_prefab.lock());
                if (!bp) return;



                int s = int(32.f * building->_scale);

                int tx = (_globalPosition.x - building->getPosition().x) / s;
                int ty = (_globalPosition.y - building->getPosition().y) / s;

                if (tx < 0 || tx > bp->_walls[0].size() - 1 || ty < 0 || ty > bp->_walls.size() - 1)
                    return;

                std::shared_ptr<Wall> wall = std::dynamic_pointer_cast<Wall>(_object.lock());
                std::shared_ptr<WallPrefab> wallPrefab = std::dynamic_pointer_cast<WallPrefab>(wall->_prefab.lock());

                if(bp->_walls[ty][tx] == wallPrefab->_id)
					return;

                bp->_walls[ty][tx] = wallPrefab->_id;
				bp->generate(bb->_position, building->_scale, bb);
				bb->generate();

                BuildingsEditor::editor->_building_panel->_game_objects->_visiblePlacedGameObjects.clear();
                BuildingsEditor::editor->_building_panel->_building->_building->addDoorsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addWindowsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addWallsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addSkeletsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addOutsideToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_game_objects->sort();
            }

            return;
        }

        


        if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {

            if (_object.lock()->_type == ObjectType::Door) {

                std::shared_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(_object.lock());
                std::shared_ptr<Animations> animations = prefab->getAnimations().lock();

                float frameWidth = 64;
                float frameHeight = 64;
                sf::IntRect frameRect(sf::Vector2i(0, 0), sf::Vector2i(frameWidth, frameHeight));

                if (animations) {
                    frameRect = animations->getFrameRect(0, 0);
                    frameWidth = (float)(frameRect.size.x);
                    frameHeight = (float)(frameRect.size.y);
                }

                float scale = BuildingsEditor::editor->_building_panel->_building->_scale;
                float gridSize = 32.f * scale;
                sf::Vector2f buildingPosition(BuildingsEditor::editor->_building_panel->_building->getPosition());

                sf::Vector2i wallPosition(
                    std::round((_globalPosition.x - buildingPosition.x) / gridSize),
                    std::floor((_globalPosition.y - buildingPosition.y) / gridSize)
                );

                

                if (canPlaceDoor(wallPosition)) {
                    

                    std::shared_ptr<BuildingsEditor::EditableBuilding> building = BuildingsEditor::editor->_building_panel->_building;
                    std::shared_ptr<Building> bb = std::dynamic_pointer_cast<Building>(building->_building);
                    std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(bb->_prefab.lock());
                    
                    std::shared_ptr<Door> door = std::make_shared<Door>(prefab, bb);

                    sf::Vector2i position(
                        (int)(wallPosition.x * 32.f - frameWidth / 2.f),
                        (int)(wallPosition.y * 32.f - frameHeight / 2.f + 48.f)
                    );

                    door->setPosition(position);

                    std::erase_if(bp->_doors, [&](const std::shared_ptr<Door>& door) {
                        return door->_position == position || door->_position + sf::Vector2i(32, 0) == position || door->_position - sf::Vector2i(32,0) == position;
                        });

                    bp->_doors.push_back(door);

                    bp->generate(bb->_position, building->_scale, bb);
                    bb->generate();

                    BuildingsEditor::editor->_building_panel->_game_objects->_visiblePlacedGameObjects.clear();
                    BuildingsEditor::editor->_building_panel->_building->_building->addDoorsToVisibleGameObjects(BuildingsEditor::editor);
                    BuildingsEditor::editor->_building_panel->_building->_building->addWindowsToVisibleGameObjects(BuildingsEditor::editor);
                    BuildingsEditor::editor->_building_panel->_building->_building->addWallsToVisibleGameObjects(BuildingsEditor::editor);
                    BuildingsEditor::editor->_building_panel->_building->_building->addSkeletsToVisibleGameObjects(BuildingsEditor::editor);
                    BuildingsEditor::editor->_building_panel->_building->_building->addOutsideToVisibleGameObjects(BuildingsEditor::editor);
                    BuildingsEditor::editor->_building_panel->_game_objects->sort();
                    return;
                }

                return;
            }

            if (_object.lock()->_type == ObjectType::Window) {


                std::shared_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(_object.lock());
                std::shared_ptr<Animations> animations = prefab->getAnimations().lock();

                float frameWidth = 32;
                float frameHeight = 32;
                sf::IntRect frameRect(sf::Vector2i(0, 0), sf::Vector2i(frameWidth, frameHeight));

                float scale = BuildingsEditor::editor->_building_panel->_building->_scale;
                float gridSize = 32.f * scale;
                sf::Vector2f buildingPosition(BuildingsEditor::editor->_building_panel->_building->getPosition());

                sf::Vector2i objectPosition(
                    std::floor((_globalPosition.x - buildingPosition.x) / gridSize),
                    std::floor((_globalPosition.y - buildingPosition.y) / gridSize)
                );

                sf::Vector2f position(
                    buildingPosition.x + objectPosition.x * gridSize,
                    buildingPosition.y + objectPosition.y * gridSize
                );

                if (!canPlaceWindow(objectPosition)) {
                    sf::RectangleShape rect(sf::Vector2f(32.f * scale, 32.f * scale));
                    rect.setPosition(sf::Vector2f(position));
                    rect.setFillColor(sf::Color(255, 47, 47, 127));
                    Main::render_window->draw(rect);
                    return;
                }

                std::shared_ptr<BuildingsEditor::EditableBuilding> building = BuildingsEditor::editor->_building_panel->_building;
                std::shared_ptr<Building> bb = std::dynamic_pointer_cast<Building>(building->_building);
                std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(bb->_prefab.lock());
                int wallY = -1;
                int level = -1;

                for (int y = 0; y < bp->_walls.size(); ++y) {

                    if (bp->_walls[y][objectPosition.x] < 0 ||
                        bp->_walls[y][objectPosition.x - 1] < 0 ||
                        bp->_walls[y][objectPosition.x + 1] < 0)
                    {
                        continue;
                    }

                    int top = y - bp->_wallHeight + 1;

                    if (objectPosition.y >= top &&
                        objectPosition.y <= y)
                    {
                        wallY = y;
                        level = y - objectPosition.y;
                        break;
                    }
                }

                if (wallY < 0)
                    return;

                sf::Vector2i windowPosition(
                    objectPosition.x * 32,
                    wallY * 32
                );

                std::shared_ptr<Window> window =
                    std::make_shared<Window>(prefab, bb, level);

                window->setPosition(windowPosition);

                std::erase_if(bp->_windows, [&](const std::shared_ptr<Window>& window) {
                    return 
                        (window->_position == windowPosition && window->_level == level) ||
                        (window->_position + sf::Vector2i(32, 0) == windowPosition && window->_level == level) ||
                        (window->_position - sf::Vector2i(32, 0) == windowPosition && window->_level == level);
                    });

                bp->_windows.push_back(window);

                bp->generate(bb->_position, building->_scale, bb);
                bb->generate();

                BuildingsEditor::editor->_building_panel->_game_objects->_visiblePlacedGameObjects.clear();
                BuildingsEditor::editor->_building_panel->_building->_building->addDoorsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addWindowsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addWallsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addSkeletsToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_building->_building->addOutsideToVisibleGameObjects(BuildingsEditor::editor);
                BuildingsEditor::editor->_building_panel->_game_objects->sort();
                return;
            }

            if (GUI_manager->Element_pressed == BuildingsEditor::editor->_building_panel->_building) {
                std::shared_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(_object.lock());
                std::shared_ptr<Animations> animations = prefab->getAnimations().lock();

                float frameWidth = 32;
                float frameHeight = 32;

                if (animations) {
                    sf::IntRect frameRect = animations->getFrameRect(0, 0);
                    frameWidth = (float)(frameRect.size.x);
                    frameHeight = (float)(frameRect.size.y);
                }

                // position of object on the map, aligning to the grid
                int floorSize = 16;
                sf::Vector2i position;
                position.x = (_globalPosition.x - (int)frameWidth / 2) / floorSize * floorSize;
                position.y = (_globalPosition.y - (int)frameHeight / 2) / floorSize * floorSize;

                // create object on map by type 
                std::shared_ptr<PlacedGameObject> objectOnMap;
                objectOnMap = std::make_shared<PlacedGameObject>(prefab);
                // positioning and adding object to map
                objectOnMap->setPosition(position);

                return;
            }

        }

    }


    void CursorOnBuilding::draw()
    {


        if (_object.expired())
            return;


        if (BuildingsEditor::editor->_main_menu->_state != Components::MainMenuStates::Closed)
            return;

        GUI_manager->setView();

        if (_object.lock()->_type == ObjectType::Door) {

            std::shared_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(_object.lock());
            std::shared_ptr<Animations> animations = prefab->getAnimations().lock();

            float frameWidth = 64;
            float frameHeight = 64;
            sf::IntRect frameRect(sf::Vector2i(0, 0), sf::Vector2i(frameWidth, frameHeight));

            if (animations) {
                frameRect = animations->getFrameRect(0, 0);
                frameWidth = (float)(frameRect.size.x);
                frameHeight = (float)(frameRect.size.y);
            }

            float scale = BuildingsEditor::editor->_building_panel->_building->_scale;
            float gridSize = 32.f * scale;
            sf::Vector2f buildingPosition(BuildingsEditor::editor->_building_panel->_building->getPosition());

            sf::Vector2i wallPosition(
                std::round((_globalPosition.x - buildingPosition.x) / gridSize),
                std::floor((_globalPosition.y - buildingPosition.y) / gridSize)
            );

            sf::Vector2f position(
                buildingPosition.x + wallPosition.x * gridSize - frameWidth * scale / 2.f,
                buildingPosition.y + wallPosition.y * gridSize - frameHeight * scale / 2.f + 48.f * scale
            );

            if (!canPlaceDoor(wallPosition)) {

                std::shared_ptr<EditableBuilding> building = BuildingsEditor::editor->_building_panel->_building;
                sf::IntRect area = building->_rect;
                area.position.x += 32;
                area.position.y += 48;
                area.size.x -= 64;
                area.size.y += - 48;
                
                area.size.x = (float)area.size.x * scale;
                area.size.y = (float)area.size.y * scale;

                if (area.findIntersection(sf::IntRect(sf::Vector2i(position.x, position.y), sf::Vector2i(64.f*scale, 64.f*scale)))) {
                    sf::RectangleShape rect(sf::Vector2f(64.0f * scale, 64.0f * scale));
                    rect.setPosition(sf::Vector2f(position.x, position.y - 48.f * scale));
                    rect.setFillColor(sf::Color(255, 47, 47, 127));
                    Main::render_window->draw(rect);
                }
                return;
            }
                

            if (animations) {
                sf::Sprite sprite(*animations->getTexture()->_texture);
                sprite.setTextureRect(frameRect);
				sprite.setScale(sf::Vector2f(scale, scale));
                sprite.setPosition(sf::Vector2f(position));
                sprite.setOrigin(sf::Vector2f(0, 48));
                Main::render_window->draw(sprite);
                return;
            }
        }

        if (_object.lock()->_type == ObjectType::Window) {

            std::shared_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(_object.lock());
            std::shared_ptr<Animations> animations = prefab->getAnimations().lock();

            float frameWidth = 32;
            float frameHeight = 32;
            sf::IntRect frameRect(sf::Vector2i(0, 0), sf::Vector2i(frameWidth, frameHeight));

            float scale = BuildingsEditor::editor->_building_panel->_building->_scale;
            float gridSize = 32.f * scale;
            sf::Vector2f buildingPosition(BuildingsEditor::editor->_building_panel->_building->getPosition());

            sf::Vector2i windowPosition(
                std::floor((_globalPosition.x - buildingPosition.x) / gridSize),
                std::floor((_globalPosition.y - buildingPosition.y) / gridSize)
            );

            sf::Vector2f position(
                buildingPosition.x + windowPosition.x * gridSize,
                buildingPosition.y + windowPosition.y * gridSize
            );


            if (!canPlaceWindow(windowPosition)) {
                std::shared_ptr<EditableBuilding> building = BuildingsEditor::editor->_building_panel->_building;
                std::shared_ptr<BuildingPrefab> prefab = std::dynamic_pointer_cast<BuildingPrefab>(BuildingsEditor::editor->_building_panel->_building->_building->_prefab.lock());
                
                sf::IntRect area = building->_rect;
                area.position.y -= prefab->_wallHeight * 32;
                area.size.y += prefab->_wallHeight * 32;

                area.size.x = (float)area.size.x * scale;
                area.size.y = (float)area.size.y * scale;

                if (area.findIntersection(sf::IntRect(sf::Vector2i(position), sf::Vector2i(32.f * scale, 32.f * scale)))) {
                    sf::RectangleShape rect(sf::Vector2f(32.f * scale, 32.f * scale));
                    rect.setPosition(sf::Vector2f(position));
                    rect.setFillColor(sf::Color(255, 47, 47, 127));
                    Main::render_window->draw(rect);
                }
                
                return;
            }


            if (animations) {
                sf::Sprite sprite(*animations->getTexture()->_texture);
                sprite.setTextureRect(frameRect);
                sprite.setScale(sf::Vector2f(scale, scale));
                sprite.setPosition(sf::Vector2f(position));
                Main::render_window->draw(sprite);
                return;
            }
        }

        if (!(GUI_manager->Element_hovered == BuildingsEditor::editor->_building_panel->_building))
            return;

        if (_object.lock()->_type == ObjectType::Floor) {

            std::shared_ptr<ToolsTerrain> tools = std::dynamic_pointer_cast<ToolsTerrain>(BuildingsEditor::editor->_palette->_tools);
            int brushSize = BuildingsEditor::editor->_palette->_brushSize;
            std::vector<std::vector<bool>> brush;

            if (tools->_toolType == ToolTerrainType::Rect)
                brush = Cursors::square_brushes[brushSize];

            if (tools->_toolType == ToolTerrainType::Circle)
                brush = Cursors::circle_brushes[brushSize];

            std::shared_ptr<BuildingsEditor::EditableBuilding> building = BuildingsEditor::editor->_building_panel->_building;

            float scale = building->_scale;
            int floorTileSize = int(16.f * scale);
            sf::IntRect buildingRect = sf::IntRect(building->getPosition(), building->getSize());

            for (int yy = 0; yy < brush.size(); yy++) {
                for (int xx = 0; xx < brush[yy].size(); xx++) {
                    if (brush[yy][xx]) {

                        int tx = (_globalPosition.x - buildingRect.position.x) / floorTileSize + (xx - brush[yy].size() / 2);
                        int ty = (_globalPosition.y - buildingRect.position.y) / floorTileSize + (yy - brush.size() / 2);

                        sf::IntRect tileRect(sf::Vector2i(buildingRect.position.x + tx * floorTileSize, buildingRect.position.y + ty * floorTileSize), sf::Vector2i(floorTileSize, floorTileSize));

                        if (buildingRect.findIntersection(tileRect)) {
                            sf::RectangleShape rect(sf::Vector2f(tileRect.size));
                            rect.setPosition(sf::Vector2f(tileRect.position));
                            rect.setFillColor(sf::Color(255, 47, 47, 127));
                            Main::render_window->draw(rect);
                        }


                    }
                }
            }

            return;
        }
    }

}