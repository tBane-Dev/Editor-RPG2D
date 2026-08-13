#include "Editors/BuildingsEditor/ActionsPanel.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/BuildingsListPanel.hpp"
#include "Theme.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"
#include "PrefabsManager.hpp"
#include "Editors/MapEditor/Editor.hpp"

namespace BuildingsEditor {
	
	ActionsPanel::ActionsPanel(sf::Vector2i margin) : Panel(
		sf::Vector2i(420, 300), 
		sf::Vector2i(BuildingsEditor::editor->_list_panel->getPosition().x, BuildingsEditor::editor->_list_panel->getPosition().y + BuildingsEditor::editor->_list_panel->getSize().y + 4)) {

		// title
		_title = std::make_unique<sf::Text>(basicFont, L"Actions", 20);
		_title->setFillColor(basic_text_color);
		_title->setPosition(sf::Vector2f(_rect.position.x + 16, _rect.position.y + 16));
		
		sf::Vector2i startPosition;
		startPosition.x = getPosition().x;
		startPosition.y = getPosition().y + margin.y;

		int marginBetweenButtons = 8;

		_saveBtn = std::make_shared<ButtonWithTextAndSprite>(
			L"Save Building",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_inactive.png"),
			sf::Vector2i(startPosition.x+16, startPosition.y)
		);

		_addBtn = std::make_shared<ButtonWithTextAndSprite>(
			L"Add Building",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_inactive.png"),
			sf::Vector2i(startPosition.x +16, _saveBtn->getPosition().y + _saveBtn->getSize().y + marginBetweenButtons)
		);

		_removeBtn = std::make_shared<ButtonWithTextAndSprite>(
			L"Remove Building",
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_press.png"),
			textures_manager->getTexture(L"assets\\tex\\editors_ui\\largeButton_inactive.png"), 
			sf::Vector2i(startPosition.x + 16, _addBtn->getPosition().y + _addBtn->getSize().y + marginBetweenButtons)
		);

		_saveBtn->_onclick_func = []() {

			auto panel = BuildingsEditor::editor->_building_panel;

			std::shared_ptr<BuildingPrefab> oldPrefab = panel->_buildingPrefab;
			std::shared_ptr<BuildingPrefab> newPrefab = panel->_editablePrefab;

			if (!oldPrefab || !newPrefab)
				return;

			newPrefab->_name = BuildingsEditor::editor->_name_panel->_name->getText();

			auto visibleObjects = MapEditor::editor->_game_objects->_visibleGameObjectsOnMap;
			for (auto& object : visibleObjects) {
				std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(object);

				if (building && building->_prefab.lock() == oldPrefab) {
					building->removeWallsFromGameObjects();
				}
			}

			prefabs_manager->replacePrefab(oldPrefab, newPrefab);

			visibleObjects = MapEditor::editor->_game_objects->_visibleGameObjectsOnMap;
			for (auto& object : visibleObjects) {

				std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(object);

				if (building && building->_prefab.lock() == oldPrefab) {
					building->loadPrefab(newPrefab);
					std::dynamic_pointer_cast<BuildingPrefab>(building->_prefab.lock())->generate(building->getPosition(), 1.0f, building);
					building->setPosition(building->getPosition());
					building->addWallsToGameObjects();
				}
			}

			panel->_buildingPrefab = newPrefab;
		};

		_addBtn->_onclick_func = [this]() {
			std::shared_ptr<BuildingPrefab> newPrefab = std::make_shared<BuildingPrefab>(BuildingsEditor::editor->_name_panel->_name->getText(), *BuildingsEditor::editor->_building_panel->_editablePrefab);
			prefabs_manager->addPrefab(newPrefab);

			BuildingsEditor::editor->_list_panel->loadAll(prefabs_manager->getPrefabs(ObjectType::Building).size());
			
			std::vector<std::shared_ptr<GameObject>> prefabs = prefabs_manager->getPrefabs(ObjectType::Building);
			int newID = std::find(prefabs.begin(), prefabs.end(), newPrefab) - prefabs.begin();
			editor->_list_panel->selectItem(newID);
		};

		_removeBtn->_onclick_func = [this]() {

			int buildingID = editor->_list_panel->_selectedItemIndex;
			if (buildingID >= 0) {

				for (auto& object : MapEditor::editor->_game_objects->_visibleGameObjectsOnMap) {
					std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(object);
					if (building && building->_prefab.lock().get() == BuildingsEditor::editor->_building_panel->_building->_building->_prefab.lock().get()) {
						building->removeWallsFromGameObjects();
						MapEditor::editor->_game_objects->removeGameObject(building);
					}
					
				}

				for(auto& chunk : MapEditor::editor->_map->_chunks) {
					for(auto& object : chunk->_gameObjectsOnMap) {
						std::shared_ptr<Building> building = std::dynamic_pointer_cast<Building>(object);
						if (building && building->_prefab.lock() == BuildingsEditor::editor->_building_panel->_buildingPrefab) {
							chunk->removeGameObjectOnMap(building);
						}
					}
				}


				if (prefabs_manager->getPrefabs(ObjectType::Building).empty())
					return;

				prefabs_manager->removePrefab(editor->_building_panel->_building->_building->_prefab.lock());

				std::vector<std::shared_ptr<GameObject>> prefabs = prefabs_manager->getPrefabs(ObjectType::Building);

				editor->_list_panel->loadAll(prefabs_manager->getPrefabs(ObjectType::Building).size());
				int newID = editor->_list_panel->_selectedItemIndex;
				if (newID >= prefabs.size())
					newID = prefabs.size() - 1;


				editor->_list_panel->selectItem(newID);

				if (newID > -1) {
					auto panel = BuildingsEditor::editor->_building_panel;
					panel->_buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(prefabs[newID]);
					panel->_editablePrefab = std::make_shared<BuildingPrefab>(panel->_buildingPrefab->getName(), *panel->_buildingPrefab);
					panel->_building->_building->loadPrefab(panel->_buildingPrefab);


					sf::Vector2i floorSize(panel->_editablePrefab->_floor[0].size(), panel->_editablePrefab->_floor.size());
					sf::Vector2i centeredPosition = panel->getPosition() + (panel->getSize() / 2 - floorSize * 16 / 2);
					panel->_building->_rect.position = centeredPosition;
					panel->_building->_rect.size = floorSize * 16;
					panel->_building->_scale = 1.0f;
					panel->_building->_building->setPosition(centeredPosition);
					panel->_building->generateEdgePoints();
				}
				else {
					auto panel = BuildingsEditor::editor->_building_panel;
					panel->_buildingPrefab = nullptr;
					panel->_editablePrefab = nullptr;
					panel->_building->_building->loadPrefab(panel->_editablePrefab);
				}
				
			}
			else {
				//editor->_animations = nullptr;
				//editor->_tempAnimations = nullptr;
				//editor->_animator = nullptr;
			}

		};

		setButtonsActivity();
		setTooltips();
	}

	ActionsPanel::~ActionsPanel() {

	}

	void ActionsPanel::setButtonsActivity() {

	}

	void ActionsPanel::setTooltips() {
	}

	void ActionsPanel::cursorHover() {
		Components::Panel::cursorHover();
	
		_saveBtn->cursorHover();
		_addBtn->cursorHover();
		_removeBtn->cursorHover();	
	}

	void ActionsPanel::handleEvent(const sf::Event& event) {
		Components::Panel::handleEvent(event);

		_saveBtn->handleEvent(event);
		_addBtn->handleEvent(event);
		_removeBtn->handleEvent(event);
	}

	void ActionsPanel::update() {
		Components::Panel::update();
		
		_saveBtn->update();
		_addBtn->update();
		_removeBtn->update();
	}

	void ActionsPanel::draw() {
		Components::Panel::draw();

		Main::render_window->draw(*_title);

		_saveBtn->draw();
		_addBtn->draw();
		_removeBtn->draw();
	}
}