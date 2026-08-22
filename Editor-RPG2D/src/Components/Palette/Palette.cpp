#include "Components/Palette/Palette.hpp"
#include "Editors/MapEditor/Editor.hpp"
#include "Objects/Monster.hpp"
#include "PrefabsManager.hpp"
#include "RenderWindow.hpp"
#include "Cursor.hpp"
#include "Editors/MapEditor/Map/CursorOnMap.hpp"
#include "DebugLog.hpp"
#include "Editors/BuildingsEditor/Editor.hpp""
#include "Wallset.hpp"
#include <queue>



namespace Components {

	std::vector<std::shared_ptr<Terrain>> Palette::terrains;
	std::vector<std::shared_ptr<Floor>> Palette::floors;
	std::vector<std::shared_ptr<Wall>> Palette::walls;
	std::vector<std::shared_ptr<BuildingPrefab>> Palette::buildings;

	Palette::Palette() : Element() {


		if (Palette::terrains.empty()) {
			for (int i = 0; i < tileset->groups.size(); i++){
				Palette::terrains.emplace_back(std::make_shared<Terrain>(i, sf::IntRect(sf::Vector2i(15 * 64, i * 64), sf::Vector2i(16, 16))));
			}
		}

		if (Palette::floors.empty()) {
			for (int i = 0; i < textures_manager->getTexture(L"assets\\tex\\floorset.png")->_texture->getSize().x / 64; i += 1) {
				Palette::floors.emplace_back(std::make_shared<Floor>(i, sf::IntRect(sf::Vector2i(i * 64, 0), sf::Vector2i(64, 64))));
			}
		}

		if(Palette::walls.empty()) {
			for (int i = 0; i < wallset->_groups.size(); i += 1) {
				Palette::walls.emplace_back(std::make_shared<Wall>(wallset->getPrefab(i)));
			}
		}

		if(Palette::buildings.empty()) {
			createBuildingsPrefabs();
		}

		sf::Vector2i size;
		size.x = 600;
		size.y = Main::render_window->getSize().y - MapEditor::editor->_main_menu->getSize().y;
		_rect = sf::IntRect(sf::Vector2i(Main::render_window->getSize().x - size.x, MapEditor::editor->_main_menu->getSize().y), size);

		_categories = std::make_shared<Categories>();
		_tools = std::make_shared<Tools>();
		_slots = std::make_shared<Slots>();

		_minBrushSize = 0;
		_maxBrushSize = 5;
		_brushSize = 2;

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr,
			ObjectType::None,
			[this]() {
				loadAll(ObjectType::None);
			}
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr, 
			ObjectType::None,
			[this]() {
				loadAll(ObjectType::None);
			}
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr, 
			ObjectType::None,
			[this]() {
				loadAll(ObjectType::None);
			}
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr, 
			ObjectType::None,
			[this]() {
				loadAll(ObjectType::None);
			}
		);

		_categories->addCategory(
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_hover.png"),
			textures_manager->getTexture(L"assets\\tex\\palette\\categories\\category_press.png"),
			nullptr,
			ObjectType::None,
			[this]() {
				loadAll(ObjectType::None);
			}
		);

		setPosition(sf::Vector2i(_rect.position));

		// set the active group
		loadAll(ObjectType::None); // TO-DO - must be - selectCategory
		_tools->setTool(nullptr, ToolType::None);
	}

	Palette::~Palette() {

	}

	void Palette::createBuildingsPrefabs() {

		auto generateFloor = [](std::vector<std::vector<int>> walls, int tile) {
			if (walls.empty() || walls[0].empty())
				return std::vector<std::vector<int>>();

			int wallsW = (int)walls[0].size();
			int wallsH = (int)walls.size();

			// floor jest 2x większy od walls
			int w = wallsW * 2;
			int h = wallsH * 2;

			// -1 = brak floor
			std::vector<std::vector<int>> floor(h, std::vector<int>(w, -1));

			// Maska ścian już w rozdzielczości floor
			std::vector<std::vector<bool>> solid(h, std::vector<bool>(w, false));

			for (int y = 0; y < wallsH; y++) {
				for (int x = 0; x < wallsW; x++) {

					if (walls[y][x] >= 0) {

						int fx = x * 2;
						int fy = y * 2;

						solid[fy][fx] = true;
						solid[fy][fx + 1] = true;
						solid[fy + 1][fx] = true;
						solid[fy + 1][fx + 1] = true;
					}
				}
			}

			int fw = w + 2;
			int fh = h + 2;

			std::vector<std::vector<bool>> outside(
				fh,
				std::vector<bool>(fw, false)
			);

			std::queue<sf::Vector2i> q;

			q.push(sf::Vector2i(0, 0));
			outside[0][0] = true;

			const int dx[4] = { 1, -1, 0, 0 };
			const int dy[4] = { 0, 0, 1, -1 };

			while (!q.empty()) {

				sf::Vector2i p = q.front();
				q.pop();

				for (int dir = 0; dir < 4; dir++) {

					int nx = p.x + dx[dir];
					int ny = p.y + dy[dir];

					if (nx < 0 || ny < 0 || nx >= fw || ny >= fh) continue;

					if (outside[ny][nx]) continue;

					int ox = nx - 1;
					int oy = ny - 1;

					bool blocked = false;

					if (ox >= 0 && oy >= 0 && ox < w && oy < h)
						blocked = solid[oy][ox];

					if (blocked) continue;

					outside[ny][nx] = true;
					q.push(sf::Vector2i(nx, ny));
				}
			}

			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++) {
					bool isOutside = outside[y + 1][x + 1];
					if (!isOutside)
						floor[y][x] = tile;
				}
			}

			return floor;
		};

		for(int wallType = 0; wallType < wallset->_groups.size(); wallType += 1) {
			for (int floorType = 1; floorType < floors.size(); floorType += 1) {

				{
					std::shared_ptr<BuildingPrefab> buildingPrefab = std::make_shared<BuildingPrefab>(L"New Building1", sf::Vector2i(8, 8));
					int X = wallType;
					int O = -1;
					buildingPrefab->_walls = {
						{ X, X, X, X, X, X, X, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, X, X, X, X, X, X, X}
					};
					buildingPrefab->_floor = generateFloor(buildingPrefab->_walls, floorType);
					Palette::buildings.emplace_back(buildingPrefab);
				}

				{
					std::shared_ptr<BuildingPrefab> buildingPrefab = std::make_shared<BuildingPrefab>(L"New Building1", sf::Vector2i(8, 8));
					int X = wallType;
					int O = -1;
					buildingPrefab->_walls = {
						{ X, X, X, X, X, X, X, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, X},
						{ X, X, X, X, X, X, X, X}
					};
					buildingPrefab->_floor = generateFloor(buildingPrefab->_walls, floorType);
					Palette::buildings.emplace_back(buildingPrefab);
				}

				{
					std::shared_ptr<BuildingPrefab> buildingPrefab = std::make_shared<BuildingPrefab>(L"New Building2", sf::Vector2i(8, 8));
					int X = wallType;
					int O = -1;
					buildingPrefab->_walls = {
						{ X, X, X, X, X, X, X, X, X, X, X, X},
						{ X, O, O, O, O, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, O, O, O, O, X},
						{ X, X, X, X, X, X, X, X, X, X, X, X}
					};
					buildingPrefab->_floor = generateFloor(buildingPrefab->_walls, floorType);
					Palette::buildings.emplace_back(buildingPrefab);
				}

				{
					std::shared_ptr<BuildingPrefab> buildingPrefab = std::make_shared<BuildingPrefab>(L"New Building3", sf::Vector2i(8, 8));
					int X = wallType;
					int O = -1;
					buildingPrefab->_walls = {
						{ X, X, X, X, X, X, X, X, X, X},
						{ X, O, O, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, O, O, X},
						{ X, O, O, O, O, X, X, X, X, X},
						{ X, O, O, O, O, X, O, O, O, O},
						{ X, O, O, O, O, X, O, O, O, O},
						{ X, X, X, X, X, X, O, O, O, O}
					};
					buildingPrefab->_floor = generateFloor(buildingPrefab->_walls, floorType);
					Palette::buildings.emplace_back(buildingPrefab);
				}

				{
					std::shared_ptr<BuildingPrefab> buildingPrefab = std::make_shared<BuildingPrefab>(L"New Building4", sf::Vector2i(8, 8));
					int X = wallType;
					int O = -1;
					buildingPrefab->_walls = {
						{ X, X, X, X, X, X, X, X, X, X},
						{ X, O, O, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, O, O, X},
						{ X, O, O, O, O, O, O, O, O, X},
						{ X, X, X, X, X, O, O, O, O, X},
						{ O, O, O, O, X, O, O, O, O, X},
						{ O, O, O, O, X, O, O, O, O, X},
						{ O, O, O, O, X, X, X, X, X, X}
					};
					buildingPrefab->_floor = generateFloor(buildingPrefab->_walls, floorType);
					Palette::buildings.emplace_back(buildingPrefab);
				}

			}

			

		}

		
	}

	sf::Vector2i Palette::getSize() {
		return _rect.size;
	}

	void Palette::loadAll(ObjectType type) {
		_categories->setCategory(type);

		sf::Vector2i slotsPosition = sf::Vector2i(_rect.position.x, _categories->getPosition().y + _categories->getSize().y);
		if (_categories->_selectedType == ObjectType::Terrain || _categories->_selectedType == ObjectType::Floor) {
			slotsPosition.y += _tools->getSize().y + 16;
		}
		_slots->setPosition(slotsPosition);
		_slots->setCategory(type);

		
	}

	void Palette::setPosition(sf::Vector2i position) {

		_rect.position = position;

		int margin = 16;

		_categories->setPosition(position);

		_tools->setPosition(position + sf::Vector2i(0, _categories->getSize().y + margin));

		if (_categories->_selectedCategory != nullptr && (_categories->_selectedType == ObjectType::Terrain || _categories->_selectedType == ObjectType::Floor))
			_slots->setPosition(position + sf::Vector2i(0, _categories->getSize().y + margin + _tools->getSize().y + margin));
		else
			_slots->setPosition(position + sf::Vector2i(0, _categories->getSize().y + margin));


	}

	void Palette::cursorHover() {
		if (MapEditor::editor->_main_menu->_tool_palette->_checkbox->_value != 0)
			return;

		if (_rect.contains(Cursors::cursor->_position)) {
			GUI_manager->Element_hovered = shared_from_this();
		}

		_categories->cursorHover();

		if (_categories->_selectedType == ObjectType::Terrain || _categories->_selectedType == ObjectType::Floor)
			_tools->cursorHover();

		_slots->cursorHover();

	}

	void Palette::handleEvent(const sf::Event& event) {

		if (MapEditor::editor->_main_menu->_tool_palette->_checkbox->_value != 0)
			return;

		if (GUI_manager->Element_hovered.get() == this) {
			if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
				GUI_manager->Element_pressed = shared_from_this();
			}
		}

		if (GUI_manager->Element_pressed.get() == this) {
			if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
				GUI_manager->Element_pressed = nullptr;
			}
		}

		_categories->handleEvent(event);

		if (_categories->_selectedType == ObjectType::Terrain || _categories->_selectedType == ObjectType::Floor)
			_tools->handleEvent(event);

		_slots->handleEvent(event);

	}

	void Palette::update() {

		_categories->update();
		_tools->update();
		_slots->update();

	}

	void Palette::draw() {

		GUI_manager->setView();

		sf::RectangleShape rect(sf::Vector2f(_rect.size));
		rect.setFillColor(sf::Color(31, 31, 31));
		rect.setPosition(sf::Vector2f(_rect.position));
		Main::render_window->draw(rect);

		_categories->draw();
		if (_categories->_selectedType == ObjectType::Terrain || _categories->_selectedType == ObjectType::Floor)
			_tools->draw();
		_slots->draw();

		GUI_manager->setView();




	}

}