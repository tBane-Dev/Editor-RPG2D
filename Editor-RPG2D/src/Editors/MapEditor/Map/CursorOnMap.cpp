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
#include "Editors/MapEditor/Map/GameObjectsOnMap.hpp"
#include <typeinfo>
#include "Animator.hpp"
#include "DebugLog.hpp"

std::vector<std::vector<std::vector<bool>>> circle_brushes = {

    // 0 - 1x1
    {
        {1}
    },

    // 1 - 3x3
    {
        {0, 1, 0},
        {1, 1, 1},
        {0, 1, 0}
    },

    // 2 - 5x5
    {
        {0, 1, 1, 1, 0},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {0, 1, 1, 1, 0}
    },

    // 3 - 7x7
    {
        {0, 0, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 0, 0}
    },

    // 4 - 9x9
    {
        {0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0}
    },

    // 5 - 11x11
    {
        {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},

    }
};

std::vector<std::vector<std::vector<bool>>> square_brushes = {

    // 0 - 1x1
    {
        {1}
    },

    // 1 - 3x3
    {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    },

    // 2 - 5x5
    {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1}
    },

    // 3 - 7x7
    {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1}
    },

    // 4 - 9x9
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1}
    },

    // 5 - 11x11
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    }
};


CursorOnMap::CursorOnMap() {

    Main::render_window->setView(MapEditor::editor->_camera->_view);
    _position = sf::Vector2i(Main::render_window->mapPixelToCoords(Main::cursor->_position));

	_object = std::weak_ptr<Object>();
}

CursorOnMap::~CursorOnMap() {

}

void CursorOnMap::update() {


    Main::render_window->setView(MapEditor::editor->_camera->_view);
    _position = sf::Vector2i(Main::render_window->mapPixelToCoords(Main::cursor->_position));
}

void CursorOnMap::handleEvent(const sf::Event& event) {
	
	if(_object.expired())
		return;

    if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Right) {
        if (MapEditor::editor->_palette->_tools->_selectedTool != nullptr) {
            MapEditor::editor->_palette->_tools->setTool(MapEditor::editor->_palette->_tools->_tools[0], ToolType::None);
        }

        if (MapEditor::editor->_palette->_slots->_selectedSlot != nullptr) {
            MapEditor::editor->_palette->_slots->selectSlot(-1);
        }

        _object = std::weak_ptr<Object>();
        return;
    }

    if (!(GUI_manager->Element_pressed == nullptr || GUI_manager->Element_pressed == MapEditor::editor->_map))
        return;

	if (_object.lock()->_type == ObjectType::Terrain) {

        bool conditionToDraw = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && (MapEditor::editor->_palette->_tools->_toolType == ToolType::Circle || MapEditor::editor->_palette->_tools->_toolType == ToolType::Rect);

		if (conditionToDraw) {

			std::shared_ptr<Map> mapa = std::dynamic_pointer_cast<Map>(MapEditor::editor->_map);

            sf::Vector2i tileCoords;
			tileCoords.x = (_position.x - mapa->getRect().position.x) / Tile::tileSize;
			tileCoords.y = (_position.y - mapa->getRect().position.y) / Tile::tileSize;

			int type = std::dynamic_pointer_cast<Terrain>(MapEditor::editor->_cursor_on_map->_object.lock())->_id;

			std::set<std::shared_ptr<Chunk>> chunksToEdit;

            int brushSize = MapEditor::editor->_palette->_brushSize;
            std::vector<std::vector<bool>> brush;
            
            if (MapEditor::editor->_palette->_tools->_toolType == ToolType::Rect)
                brush = square_brushes[brushSize];

            if (MapEditor::editor->_palette->_tools->_toolType == ToolType::Circle)
                brush = circle_brushes[brushSize];
           

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
		}

		return;
	}

	if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {

		if (GUI_manager->Element_pressed == MapEditor::editor->_map) {
			std::weak_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(_object.lock());
			std::weak_ptr<Animations> animations = prefab.lock()->getAnimations().lock();
            
            float frameWidth = 128;
            float frameHeight = 128;

            if (!animations.expired()) {
				sf::IntRect frameRect = animations.lock()->getFrameRect(0, 0);
                frameWidth = (float)(frameRect.size.x);
                frameHeight = (float)(frameRect.size.y);
            }

			// position of object on the map, aligning to the grid
			sf::Vector2i position;
			position.x = (_position.x - (int)frameWidth / 2) / Tile::tileSize * Tile::tileSize;
			position.y = (_position.y - (int)frameHeight / 2) / Tile::tileSize * Tile::tileSize;

			if (dynamic_cast<MonsterPrefab*>(prefab.lock().get())) {
				position.x += prefab.lock()->getOrigin().x;
				position.y += prefab.lock()->getOrigin().y;
			}

			// create object on map by type 
			std::shared_ptr<GameObjectOnMap> objectOnMap;

			if (prefab.lock()->_type == ObjectType::Monster) objectOnMap = std::make_shared<Monster>(prefab);
			else if (prefab.lock()->_type == ObjectType::Nature) objectOnMap = std::make_shared<Nature>(prefab);
			else objectOnMap = std::make_shared<GameObjectOnMap>(prefab);


			// positioning and adding object to map
			objectOnMap->setPosition(position);
            MapEditor::editor->_game_objects->addGameObject(objectOnMap);
			return;
		}

	}

}


void CursorOnMap::draw()
{
	
    if(_object.expired())
		return;

	if(!(GUI_manager->Element_hovered == MapEditor::editor->_map || GUI_manager->Element_hovered == nullptr))
		return;

    Main::render_window->setView(MapEditor::editor->_camera->_view);

    if (_object.lock()->_type == ObjectType::Terrain) {

		int brushSize = MapEditor::editor->_palette->_brushSize;
		
        std::vector<std::vector<bool>> brush;

        if (MapEditor::editor->_palette->_tools->_toolType == ToolType::Rect)
            brush = square_brushes[brushSize];
        
		if (MapEditor::editor->_palette->_tools->_toolType == ToolType::Circle)
            brush = circle_brushes[brushSize];

		std::shared_ptr<Map> mapa = std::dynamic_pointer_cast<Map>(MapEditor::editor->_map);

        for(int yy = 0; yy < brush.size(); yy++) {
            for(int xx = 0; xx < brush[yy].size(); xx++) {
                if (brush[yy][xx]) {

					int tx = _position.x / Tile::tileSize + (xx - brush[yy].size() / 2);
					int ty = _position.y / Tile::tileSize + (yy - brush.size() / 2);

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

		std::weak_ptr<GameObject> prefab = std::dynamic_pointer_cast<GameObject>(_object.lock());
		std::weak_ptr<Animations> animations = prefab.lock()->getAnimations();
        
        float frameWidth = 128;
        float frameHeight = 128;
		sf::IntRect frameRect(sf::Vector2i(0, 0), sf::Vector2i(frameWidth, frameHeight));

        if (!animations.expired()) {
            frameRect = animations.lock()->getFrameRect(0, 0);
            frameWidth = (float)(frameRect.size.x);
            frameHeight = (float)(frameRect.size.y);
        }

		sf::Vector2i position;
		position.x = (_position.x - (int)frameWidth/2) / Tile::tileSize * Tile::tileSize;
		position.y = (_position.y - (int)frameHeight/2) / Tile::tileSize * Tile::tileSize;

		sf::RectangleShape outlineRect(sf::Vector2f(frameRect.size));
		outlineRect.setPosition(sf::Vector2f(position));
		outlineRect.setFillColor(sf::Color::Transparent);
		outlineRect.setOutlineThickness(2);
		outlineRect.setOutlineColor(sf::Color::Green);
        Main::render_window->draw(outlineRect);

        if (!animations.expired()) {
            sf::Sprite sprite(*animations.lock()->getTexture()->_texture);
            sprite.setTextureRect(frameRect);

            sprite.setPosition(sf::Vector2f(position));
            Main::render_window->draw(sprite);
        }
		return;
	}


}
