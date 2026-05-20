#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "Cursor.hpp"
#include "Editors/MapEditor/Map/CameraOnMap.hpp"
#include "Editors/MapEditor/Map/Map.hpp"
#include "Editors/Editor.hpp"
#include "Editors/MapEditor/MapEditor.hpp"
#include "Editors/MapEditor/Objects/GameObject.hpp"
#include "Editors/MapEditor/Objects/Monster.hpp"
#include "Editors/MapEditor/Objects/Nature.hpp"
#include "Editors/MapEditor/Map/GameObjectsOnMap.hpp"
#include "Editors/MapEditor/Objects/Terrain.hpp"
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
	
	if(_object == nullptr)
		return;

    if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Right) {
        if (map_editor->_palette->_tools->_selectedTool != nullptr) {
			map_editor->_palette->_tools->setTool(map_editor->_palette->_tools->_tools[0], ToolType::None);
        }

        if (map_editor->_palette->_slots->_selectedSlot != nullptr) {
			map_editor->_palette->_slots->selectSlot(-1);
        }

        _object = nullptr;
        return;
    }

    if (GUI_manager->Element_pressed != map_editor->_map)
        return;

	if (_object->_type == ObjectType::Terrain) {
		bool conditionToDraw = false;

		const auto* mv = event.getIf<sf::Event::MouseMoved>();
		const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>();

		if ((mv || mbp) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && 
            (map_editor->_palette->_tools->_toolType == ToolType::Circle || map_editor->_palette->_tools->_toolType == ToolType::Rect))
			conditionToDraw = true;

		if (conditionToDraw) {

			std::shared_ptr<Map> mapa = std::dynamic_pointer_cast<Map>(map_editor->_map);

			std::shared_ptr<Chunk> hoveredChunk = mapa->getChunkByGlobalPosition();
			if (!hoveredChunk) return;

			std::shared_ptr<Tile> hoveredTile = hoveredChunk->getTileByGlobalPosition();
			if (!hoveredTile) return;

			int type = std::dynamic_pointer_cast<Terrain>(map_editor->_cursor_on_map->_object)->_id;

			std::set<std::shared_ptr<Chunk>> chunksToEdit;

            int brushSize = map_editor->_palette->_brushSize;
            std::vector<std::vector<bool>> brush;
            
            if (map_editor->_palette->_tools->_toolType == ToolType::Rect)
                brush = square_brushes[brushSize];

            if (map_editor->_palette->_tools->_toolType == ToolType::Circle)
                brush = circle_brushes[brushSize];
           

            for (int yy = 0; yy < brush.size(); yy++) {
                for (int xx = 0; xx < brush[yy].size(); xx++) {
                    if (brush[yy][xx]) {

						int tileX = hoveredTile->_coords.x + (xx - brush[yy].size() / 2);
						int tileY = hoveredTile->_coords.y + (yy - brush.size() / 2);

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

		if (GUI_manager->Element_pressed == map_editor->_map) {
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

			if (prefab->_type == ObjectType::Monster) objectOnMap = std::make_shared<Monster>(prefab);
			else if (prefab->_type == ObjectType::Nature) objectOnMap = std::make_shared<Nature>(prefab);
			else objectOnMap = std::make_shared<GameObjectOnMap>(prefab);


			// positioning and adding object to map
			objectOnMap->setPosition(position);
			map_editor->_game_objects->addGameObject(objectOnMap);
			return;
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

    if (_object->_type == ObjectType::Terrain) {

		int brushSize = map_editor->_palette->_brushSize;
		
        std::vector<std::vector<bool>> brush;

        if (map_editor->_palette->_tools->_toolType == ToolType::Rect)
            brush = square_brushes[brushSize];
        
		if (map_editor->_palette->_tools->_toolType == ToolType::Circle)
            brush = circle_brushes[brushSize];

		std::shared_ptr<Map> mapa = std::dynamic_pointer_cast<Map>(map_editor->_map);

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
                    window->draw(outlineRect);
                }
            }
		}

        return;
    }

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
