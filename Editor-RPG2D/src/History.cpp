#include "History.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "DebugLog.hpp"
#include "Editors/MapEditor/Map/Tile.hpp"
#include "Editors/MapEditor/Map/Chunk.hpp"
#include "Editors/MapEditor/Map/Map.hpp"
#include <set>

Step::Step() {

}

Step::~Step() {

}

void Step::undo() {

}

void Step::redo() {

}

EditTerrain::EditTerrain(std::map<sf::Vector2i, MapEditor::CursorOnMap::TileValues, MapEditor::CursorOnMap::Vector2iCompare> editedTiles) {
    _editedTiles = editedTiles;
}

EditTerrain::~EditTerrain() {

}

void EditTerrain::undo() {

    
    std::shared_ptr<MapEditor::Map> mapa = MapEditor::editor->_map;
    std::set<std::shared_ptr<MapEditor::Chunk>> chunksToEdit;
    
    for (auto& tile : _editedTiles) {
    
        int tileX = tile.first.x;
        int tileY = tile.first.y;
    
        std::shared_ptr<MapEditor::Chunk> c = mapa->getChunkByTileGlobalCoords(tileX, tileY);
        if (!c) continue;
    
        std::shared_ptr<MapEditor::Tile> t = c->getTileByTileGlobalCoords(tileX, tileY);
        if (!t) continue;
    
        t->_type = tile.second.oldValue;
    
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
    
    for (auto& c : chunksToEdit)
        c->generateVertexArray(
            mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y - 1), mapa->getChunkByCoords(c->_coords.x, c->_coords.y - 1), mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y - 1),
            mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y), mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y),
            mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y + 1), mapa->getChunkByCoords(c->_coords.x, c->_coords.y + 1), mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y + 1)
        );
}

void EditTerrain::redo() {
    std::shared_ptr<MapEditor::Map> mapa = MapEditor::editor->_map;
    std::set<std::shared_ptr<MapEditor::Chunk>> chunksToEdit;

    for (auto& tile : _editedTiles) {

        int tileX = tile.first.x;
        int tileY = tile.first.y;

        std::shared_ptr<MapEditor::Chunk> c = mapa->getChunkByTileGlobalCoords(tileX, tileY);
        if (!c) continue;

        std::shared_ptr<MapEditor::Tile> t = c->getTileByTileGlobalCoords(tileX, tileY);
        if (!t) continue;

        t->_type = tile.second.newValue;

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

    for (auto& c : chunksToEdit)
        c->generateVertexArray(
            mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y - 1), mapa->getChunkByCoords(c->_coords.x, c->_coords.y - 1), mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y - 1),
            mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y), mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y),
            mapa->getChunkByCoords(c->_coords.x - 1, c->_coords.y + 1), mapa->getChunkByCoords(c->_coords.x, c->_coords.y + 1), mapa->getChunkByCoords(c->_coords.x + 1, c->_coords.y + 1)
        );
}


AddGameObjectOnMap::AddGameObjectOnMap(std::shared_ptr<PlacedGameObject> object, sf::Vector2i position) : Step() {
	_object = object;
	_position = position;
}

AddGameObjectOnMap::~AddGameObjectOnMap() {

}

void AddGameObjectOnMap::undo() {
	MapEditor::editor->_map->getChunkByGlobalPosition(_object->getPosition())->removePlacedGameObject(_object);
	MapEditor::editor->setVisibleChunks();
}

void AddGameObjectOnMap::redo() {
	_object->setPosition(_position);
	MapEditor::editor->_map->getChunkByGlobalPosition(_position)->addPlacedGameObject(_object);
	MapEditor::editor->setVisibleChunks();
}

RemoveGameObjectsOnMap::RemoveGameObjectsOnMap(std::vector<std::shared_ptr<SelectedPlacedGameObject>> objects) : Step() {
    _objects = objects;
}

RemoveGameObjectsOnMap::~RemoveGameObjectsOnMap() {

}

void RemoveGameObjectsOnMap::undo() {
    for (auto& object : _objects) {
        if (auto obj = object) {
            MapEditor::editor->_map->getChunkByGlobalPosition(obj->_object->_position)->addPlacedGameObject(obj->_object);
        }
    }
    MapEditor::editor->setVisibleChunks();
}

void RemoveGameObjectsOnMap::redo() {
    for (auto& object : _objects) {
        if (auto obj = object) {
            MapEditor::editor->_game_objects->removeGameObject(obj->_object);
            for (auto& chunk : MapEditor::editor->_map->_chunks) {
                chunk->removePlacedGameObject(obj->_object);
            }
        }
    }
}

MoveGameObjectsOnMap::MoveGameObjectsOnMap(std::vector<std::shared_ptr<SelectedPlacedGameObject>> objects) : Step() {
    _objects = objects;

    for (auto& obj : _objects) {
        _oldPositions.push_back(obj->_prevPosition);
        _newPositions.push_back(obj->_object->_position);
    }
}

MoveGameObjectsOnMap::~MoveGameObjectsOnMap() {

}

void MoveGameObjectsOnMap::undo() {

    for (int i = 0; i < _objects.size(); i++) {
        MapEditor::editor->_map->getChunkByGlobalPosition(_objects[i]->_object->_position)->removePlacedGameObject(_objects[i]->_object);
        _objects[i]->_object->setPosition(_oldPositions[i]);
        MapEditor::editor->_map->getChunkByGlobalPosition(_objects[i]->_object->_position)->addPlacedGameObject(_objects[i]->_object);
    }

    MapEditor::editor->setVisibleChunks();
}

void MoveGameObjectsOnMap::redo() {

    for (int i = 0; i < _objects.size(); i++) {
        MapEditor::editor->_map->getChunkByGlobalPosition(_objects[i]->_object->_position)->removePlacedGameObject(_objects[i]->_object);
        _objects[i]->_object->setPosition(_newPositions[i]);
        MapEditor::editor->_map->getChunkByGlobalPosition(_objects[i]->_object->_position)->addPlacedGameObject(_objects[i]->_object);;
    }

    MapEditor::editor->setVisibleChunks();
}

History::History() {

}

History::~History() {

}

bool History::canUndo() {
    return _current >= 0 || !MapEditor::editor->_cursor_on_map->_selectedObjects.empty();
}

bool History::canRedo() {
    return _current < (int)(_steps.size()) - 1;
}

void History::undo() {

    if (!canUndo())
        return;

    if (!MapEditor::editor->_cursor_on_map->_selectedObjects.empty()) {
        for (auto& selected : MapEditor::editor->_cursor_on_map->_selectedObjects) {
            selected->_object->_isSelected = false;
        }

        MapEditor::editor->_cursor_on_map->_selectedObjects.clear();
        return;
    }

    _steps[_current]->undo();
    _current -= 1;
}

void History::redo() {
    if (!canRedo())
        return;

    _current += 1;
    _steps[_current]->redo();
}

void History::addStep(std::shared_ptr<Step> step) {

    std::wstring type = L"none";
    if(std::dynamic_pointer_cast<AddGameObjectOnMap>(step)) type = L"Add";
    if(std::dynamic_pointer_cast<RemoveGameObjectsOnMap>(step)) type = L"Remove";
    if(std::dynamic_pointer_cast<MoveGameObjectsOnMap>(step)) type = L"Move";
    if(std::dynamic_pointer_cast<EditTerrain>(step)) type = L"Edit Terrain";
    DebugLog(L"Add Step: " + type);


    if (_current + 1 < (int)_steps.size()) {
        _steps.erase(
            _steps.begin() + _current + 1,
            _steps.end()
        );
    }

    _steps.push_back(std::move(step));
    _current += 1;
}

std::shared_ptr<History> history = nullptr;