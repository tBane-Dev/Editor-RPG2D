#include "Editors/BuildingsEditor/BuildingsListPanel.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/MainMenu.hpp"
#include "PrefabsManager.hpp"
#include "Theme.hpp"

namespace BuildingsEditor {

	BuildingsListPanel::BuildingsListPanel(sf::Vector2i margin) : Components::ListPanel(margin, sf::Vector2i(420, 600), sf::Vector2i(margin.x, BuildingsEditor::editor->_main_menu->getSize().y + margin.y), prefabs_manager->getPrefabs(ObjectType::Building).size()) {
		_title->setString(L"Buildings List");
		loadAll(prefabs_manager->getPrefabs(ObjectType::Building).size());
	}

	BuildingsListPanel::~BuildingsListPanel() {

	}

	void BuildingsListPanel::loadList() {
		resetItems();

		std::vector<std::shared_ptr<GameObject>> prefabs = prefabs_manager->getPrefabs(ObjectType::Building);

		int max_id = (prefabs.size() < _items.size()) ? prefabs.size() : _items.size();

		for (int i = 0; i < max_id; i++) {
			std::shared_ptr<Components::ListPanelItem> item = _items[i];

			int index = i;
			if (_scrollbar) index += _scrollbar->getValue() / basic_text_rect_height;

			if (index < prefabs.size()) {

				std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(prefabs[index]);

				item->setName(buildingPrefab->getName());
				item->_onclick_func = [this, index, item, buildingPrefab]() {

						selectItem(index);

						auto panel = BuildingsEditor::editor->_building_panel;
						std::shared_ptr<BuildingPrefab> editablePrefab = std::make_shared<BuildingPrefab>(buildingPrefab->getName(), *buildingPrefab);

						panel->_buildingPrefab = buildingPrefab;
						panel->_editablePrefab = editablePrefab;
						panel->_building->_building->loadPrefab(editablePrefab);
						panel->_building->_building->generate(panel->_building->_building->getPosition());

						BuildingsEditor::editor->_name_panel->_name->setText(buildingPrefab->getName());

						sf::Vector2i centeredPosition = panel->getPosition() + (panel->getSize() / 2 - sf::Vector2i(editablePrefab->_floor[0].size(), editablePrefab->_floor.size()) * 16 / 2);
						panel->_building->_rect.position = centeredPosition;
						panel->_building->_rect.size = sf::Vector2i(editablePrefab->_floor[0].size(), editablePrefab->_floor.size()) * 16;
						panel->_building->_scale = 1.0f;
						panel->_building->_building->setPosition(centeredPosition);
						panel->_building->generateEdgePoints();
				};
			}
			else {
				item->setName(L"");
				item->_onclick_func = {};
			}
		}
	}
}
