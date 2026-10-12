#pragma once
#include "SFML/Graphics.hpp"
#include "Objects/PlacedGameObject.hpp"
#include "Objects/SelectedPlacedGameObject.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Editors/MapEditor/Map/CursorOnMap.hpp"

class Step : public std::enable_shared_from_this<Step> {
public:
	Step();
	virtual ~Step();
	virtual void undo();
	virtual void redo();
};

class EditTerrain : public Step {
public:
	std::map<sf::Vector2i, MapEditor::CursorOnMap::TileValues, MapEditor::CursorOnMap::Vector2iCompare> _editedTiles;

	EditTerrain(std::map<sf::Vector2i, MapEditor::CursorOnMap::TileValues, MapEditor::CursorOnMap::Vector2iCompare> editedTiles);
	~EditTerrain();
	virtual void undo();
	virtual void redo();
};

class AddGameObjectOnMap : public Step {
public:

	std::shared_ptr<PlacedGameObject> _object;
	sf::Vector2i _position;

	AddGameObjectOnMap(std::shared_ptr<PlacedGameObject> object, sf::Vector2i position);
	~AddGameObjectOnMap();
	virtual void undo();
	virtual void redo();
};

class RemoveGameObjectsOnMap : public Step {
public:

	std::vector<std::shared_ptr<SelectedPlacedGameObject>> _objects;

	RemoveGameObjectsOnMap(std::vector<std::shared_ptr<SelectedPlacedGameObject>> objects);
	~RemoveGameObjectsOnMap();
	virtual void undo();
	virtual void redo();
};

class MoveGameObjectsOnMap : public Step {
public:

	std::vector<std::shared_ptr<SelectedPlacedGameObject>> _objects;
	std::vector<sf::Vector2i> _oldPositions;
	std::vector<sf::Vector2i> _newPositions;

	MoveGameObjectsOnMap(std::vector<std::shared_ptr<SelectedPlacedGameObject>> objects);
	~MoveGameObjectsOnMap();
	virtual void undo();
	virtual void redo();
};

class History {
public:
	std::vector<std::shared_ptr<Step>> _steps;
	int _current = -1;

	History();
	~History();
	bool canUndo();
	bool canRedo();
	void undo();
	void redo();
	void addStep(std::shared_ptr<Step> step);
};

extern std::shared_ptr<History> history;