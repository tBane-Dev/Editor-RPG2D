#include "Editors/BuildingsEditor/BuildingsListPanel.hpp"
#include "Editors/BuildingsEditor/Editor.hpp"
#include "Editors/BuildingsEditor/MainMenu.hpp"
#include "PrefabsManager.hpp"
#include "Theme.hpp"

namespace BuildingsEditor {

	BuildingsListPanel::BuildingsListPanel(sf::Vector2i margin) : Components::ListPanel(margin, sf::Vector2i(420, 600), sf::Vector2i(margin.x, BuildingsEditor::editor->_main_menu->getSize().y + margin.y)) {
		_title->setString(L"Buildings List");
		loadAll();
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
					if (_selectedItemIndex != index) {
						selectItem(index);
						auto panel = BuildingsEditor::editor->_building_panel;
						panel->_buildingPrefab = buildingPrefab;
						panel->_building->_building->loadPrefab(buildingPrefab);

						sf::Vector2i centeredPosition = panel->getPosition() + (panel->getSize() / 2 - std::dynamic_pointer_cast<BuildingPrefab>(buildingPrefab)->_floorSize * 16 / 2);
						panel->_building->_scale = 1.0f;
						panel->_building->setPosition(centeredPosition);
						panel->_building->_building->setPosition(centeredPosition);
						panel->_building->generateEdgePoints();
					}
					};
			}
			else {
				item->setName(L"");
				item->_onclick_func = {};
			}
		}
	}
}
