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
        int s = int(32.f * BuildingsEditor::editor->_building_panel->_building->_scale);

        int tx = (_globalPosition.x - BuildingsEditor::editor->_building_panel->_building->getPosition().x) / s;
        int ty = (_globalPosition.y - BuildingsEditor::editor->_building_panel->_building->getPosition().y) / s;

        if (BuildingsEditor::editor->_building_panel->_building->_walls[ty * BuildingsEditor::editor->_building_panel->_building->_wallsSize.x + tx] != -1) {
            BuildingsEditor::editor->_building_panel->_building->_walls[ty * BuildingsEditor::editor->_building_panel->_building->_wallsSize.x + tx] = -1;
            BuildingsEditor::editor->_building_panel->_building->generateWalls();
            return;
        }
    }

    if (_object.expired())
        return;

    if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Right) {
        if (BuildingsEditor::editor->_palette->_tools->_selectedTool != nullptr) {
            BuildingsEditor::editor->_palette->_tools->setTool(BuildingsEditor::editor->_palette->_tools->_tools[0], ToolType::None);
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
    
        bool conditionToDrawFloor = 
            GUI_manager->Element_hovered == BuildingsEditor::editor->_building_panel->_building &&
            sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && 
            (BuildingsEditor::editor->_palette->_tools->_toolType == ToolType::Circle || BuildingsEditor::editor->_palette->_tools->_toolType == ToolType::Rect);
    
        if (conditionToDrawFloor) {
            std::shared_ptr<BuildingsEditor::EditableBuilding> building = BuildingsEditor::editor->_building_panel->_building;

            float scale = building->_scale;
            int floorTileSize = int(16.f * scale);
            sf::IntRect buildingRect = sf::IntRect(building->getPosition(), building->getSize());

            int brushSize = BuildingsEditor::editor->_palette->_brushSize;
            std::vector<std::vector<bool>> brush;

            if (BuildingsEditor::editor->_palette->_tools->_toolType == ToolType::Rect)
                brush = Cursors::square_brushes[brushSize];

            if (BuildingsEditor::editor->_palette->_tools->_toolType == ToolType::Circle)
                brush = Cursors::circle_brushes[brushSize];


            int index = std::dynamic_pointer_cast<Floor>(_object.lock())->_id;
            bool editedFloor = false;
            for (int yy = 0; yy < brush.size(); yy++) {
                for (int xx = 0; xx < brush[yy].size(); xx++) {
                    if (brush[yy][xx]) {

                        int tx = (_globalPosition.x - buildingRect.position.x) / floorTileSize + (xx - brush[yy].size() / 2);
                        int ty = (_globalPosition.y - buildingRect.position.y) / floorTileSize + (yy - brush.size() / 2);

                        if(tx < 0 || tx > building->_floorSize.x - 1 || ty < 0 || ty > building->_floorSize.y - 1)
							continue;

                        building->_floor[ty * building->_floorSize.x + tx] = index;
						editedFloor = true;
                    }
                }
            }

			if(editedFloor) 
                building->generateFloorVertexArray();
        }
		return;
	}

    

    if (_object.lock()->_type == ObjectType::Wall) {

        bool conditionToDrawWalls = GUI_manager->Element_hovered == BuildingsEditor::editor->_building_panel->_building &&
            sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && _object.lock()->_type == ObjectType::Wall;
            
        if (conditionToDrawWalls) {

            std::shared_ptr<BuildingsEditor::EditableBuilding> building = BuildingsEditor::editor->_building_panel->_building;


            int s = int(32.f * building->_scale);

            int tx = (_globalPosition.x - building->getPosition().x) / s;
            int ty = (_globalPosition.y - building->getPosition().y) / s;

            if (tx < 0 || tx > building->_wallsSize.x - 1 || ty < 0 || ty > building->_wallsSize.y - 1)
                return;

            std::shared_ptr<Wall> wall = std::dynamic_pointer_cast<Wall>(_object.lock());
            std::shared_ptr<WallPrefab> wallPrefab = std::dynamic_pointer_cast<WallPrefab>(wall->_prefab.lock());

            BuildingsEditor::editor->_building_panel->_building->_walls[ty * BuildingsEditor::editor->_building_panel->_building->_wallsSize.x + tx] = wallPrefab->_id;
            BuildingsEditor::editor->_building_panel->_building->generateWalls();
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

		int brushSize = BuildingsEditor::editor->_palette->_brushSize;
		
        std::vector<std::vector<bool>> brush;
    
        if (BuildingsEditor::editor->_palette->_tools->_toolType == ToolType::Rect)
            brush = Cursors::square_brushes[brushSize];
        
		if (BuildingsEditor::editor->_palette->_tools->_toolType == ToolType::Circle)
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
    //
	//if(dynamic_pointer_cast<GameObject>(_object.lock()) != nullptr) {
    //
	//	std::shared_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(_object.lock());
	//	std::shared_ptr<Animations> animations = prefab->getAnimations().lock();
    //    
    //    float frameWidth = 128;
    //    float frameHeight = 128;
	//	sf::IntRect frameRect(sf::Vector2i(0, 0), sf::Vector2i(frameWidth, frameHeight));
    //
    //    if (animations) {
    //        frameRect = animations->getFrameRect(0, 0);
    //        frameWidth = (float)(frameRect.size.x);
    //        frameHeight = (float)(frameRect.size.y);
    //    }
    //
	//	sf::Vector2i position;
	//	position.x = (_globalPosition.x - (int)frameWidth/2) / Tile::tileSize * Tile::tileSize;
	//	position.y = (_globalPosition.y - (int)frameHeight/2) / Tile::tileSize * Tile::tileSize;
    //
	//	sf::RectangleShape outlineRect(sf::Vector2f(frameRect.size));
	//	outlineRect.setPosition(sf::Vector2f(position));
	//	outlineRect.setFillColor(sf::Color::Transparent);
	//	outlineRect.setOutlineThickness(2);
	//	outlineRect.setOutlineColor(sf::Color::Green);
    //    Main::render_window->draw(outlineRect);
    //
    //    if (animations) {
    //        sf::Sprite sprite(*animations->getTexture()->_texture);
    //        sprite.setTextureRect(frameRect);
    //
    //        sprite.setPosition(sf::Vector2f(position));
    //        Main::render_window->draw(sprite);
    //    }
	//	return;
	//}


}
