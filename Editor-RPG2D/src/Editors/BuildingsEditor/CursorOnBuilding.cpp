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

CursorOnBuilding::CursorOnBuilding() : CursorWithObject() {
	
}

CursorOnBuilding::~CursorOnBuilding() {

}

void CursorOnBuilding::update() {
    CursorWithObject::update();
}

void CursorOnBuilding::handleEvent(const sf::Event& event) {

	

    if (Main::windows_manager->get_back())
        return;

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
            buildingPrefab->generateWalls(building->_position, BuildingsEditor::editor->_building_panel->_building->_scale, building);
            buildingPrefab->generateRoofs(building->_position, BuildingsEditor::editor->_building_panel->_building->_scale);
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

                        if(tx < 0 || tx > bp->_floor[0].size() - 1 || ty < 0 || ty > bp->_floor.size() - 1)
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

            bp->_walls[ty][tx] = wallPrefab->_id;
  
            bp->generateWalls(bb->_position, building->_scale, bb);
            bp->generateRoofs(bb->_position, building->_scale);
        }

        return;
    }

	if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {

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
            std::shared_ptr<GameObjectOnMap> objectOnMap;
            objectOnMap = std::make_shared<GameObjectOnMap>(prefab);
            // positioning and adding object to map
            objectOnMap->setPosition(position);

			return;
		}
    
	}

}


void CursorOnBuilding::draw()
{
	

    if(_object.expired())
		return;
    
	if(!(GUI_manager->Element_hovered == BuildingsEditor::editor->_building_panel->_building))
		return;
    
    if(BuildingsEditor::editor->_main_menu->_state != Components::MainMenuStates::Closed)
		return;
    
    GUI_manager->setView();
    
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

        for(int yy = 0; yy < brush.size(); yy++) {
            for(int xx = 0; xx < brush[yy].size(); xx++) {
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
