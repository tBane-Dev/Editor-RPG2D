#include "Editors/MapEditor/Editor.hpp"
#include "DebugLog.hpp"
#include "Objects/Monster.hpp"
#include "Objects/Nature.hpp"
#include "PrefabsManager.hpp"
#include "WindowsManager.hpp"
#include "History.hpp"
#include <SFML/OpenGL.hpp>

namespace MapEditor {

	Editor::Editor() {

	}

	Editor::~Editor() {

	}

	void Editor::createMap(int width, int height) {

		_map = std::make_shared<Map>();

		_map->create(width, height);

		_map->drawCircle(sf::Vector2i(24, 28), 16, 4);
		_map->drawCircle(sf::Vector2i(48, 16), 16, 4);
		_map->drawCircle(sf::Vector2i(42, 28), 16, 4);
	}

	void Editor::createCursorOnMap() {
		_cursor_on_map = std::make_shared<CursorOnMap>();
	}

	void Editor::createGameObjects() {
		_game_objects = std::make_shared<PlacedGameObjects>();
	}

	void Editor::createCamera() {

		_camera = std::make_shared<CameraOnMap>();

		_camera->_position = sf::Vector2f((float)(_map->getRect().size.x / 2), (float)(_map->getRect().size.y / 2));
		_camera->update();
	}

	void Editor::setVisibleChunks() {
		_map->setVisibleChunks();

		auto& visibleObjects = MapEditor::editor->_game_objects->_visiblePlacedGameObjects;

		for (const auto& selectedObject :
			MapEditor::editor->_cursor_on_map->_selectedObjects) {

			auto object = selectedObject->_object;
			if (!object)
				continue;

			if (
				std::find(visibleObjects.begin(), visibleObjects.end(), object) == visibleObjects.end()) {
				
				visibleObjects.push_back(selectedObject->_object);

				if (selectedObject->_object->_type == ObjectType::Building) {
					std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(selectedObject->_object);
					b->addDoorsToVisibleGameObjects(MapEditor::editor);
					b->addWallsToVisibleGameObjects(MapEditor::editor);
					b->addWindowsToVisibleGameObjects(MapEditor::editor);
					b->addSkeletsToVisibleGameObjects(MapEditor::editor);
					b->addWallMountedToVisibleGameObjects(MapEditor::editor);
					b->addOutsideToVisibleGameObjects(MapEditor::editor);
				}
			}
		}
	}

	void Editor::createMainMenu() {
		_main_menu = std::make_shared<MainMenu>();
	}

	void Editor::createPalette() {
		_palette = std::make_shared<Palette>();
	}

	void Editor::cursorHover() {

		if (Main::windows_manager->get_back())
			return;

		if (_main_menu->_state != Components::MainMenuStates::Closed) {
			_main_menu->cursorHover();
			return;
		}

		_game_objects->_hoveredPlacedGameObject = std::weak_ptr<PlacedGameObject>();
		_game_objects->cursorHover();

		_map->cursorHover();
		_palette->cursorHover();
		_main_menu->cursorHover();
	}

	void Editor::handleEvent(const sf::Event& event) {

		if (Main::windows_manager->get_back())
			return;

		if(const auto& kp = event.getIf<sf::Event::KeyPressed>(); kp && kp->code == sf::Keyboard::Key::Delete) {
			
			if (!_cursor_on_map->_selectedObjects.empty()) {

				for(auto& selectedObject : _cursor_on_map->_selectedObjects) {
					if (auto object = selectedObject->_object) {
						_game_objects->removeGameObject(object);
						for(auto& chunk : _map->_chunks) {
							chunk->removePlacedGameObject(object);
						}
					}
				}

				std::shared_ptr<RemoveGameObjectsOnMap> step = std::make_shared<RemoveGameObjectsOnMap>(_cursor_on_map->_selectedObjects);
				history->addStep(step);
				for (auto& selected : _cursor_on_map->_selectedObjects) {
					selected->_object->_isSelected = false;
				}
				_cursor_on_map->_selectedObjects.clear();
				return;
			}
			
		}

		_main_menu->handleEvent(event);

		if (_main_menu->_state != Components::MainMenuStates::Closed)
			return;

		_camera->handleEvent(event);

		_map->handleEvent(event);
		_cursor_on_map->handleEvent(event);
		_palette->handleEvent(event);


	}

	void Editor::update() {

		_main_menu->update();
		_palette->update();

		_camera->update();
		_cursor_on_map->update();
		
		_map->update();

		_game_objects->update();

		_game_objects->sort();

	}

	void Editor::draw() {

		_camera->setView();
		_map->draw();
		_game_objects->draw();
		_cursor_on_map->draw();
		
		Main::render_window->pushGLStates();

		struct Vertice {
			float x;
			float y;
			float z;
			float rx;
			float ry;
			float rz;
			float u;
			float v;
		};

		float size = 64.0f;
		size /= 2.0f;
		std::vector<Vertice> vertices;
		// Front face
		vertices.emplace_back(-size, -size, size, 0.f, 0.f, 1.f, 0.f, 0.f);
		vertices.emplace_back(size, -size, size, 0.f, 0.f, 1.f, 1.f, 0.f);
		vertices.emplace_back(size, size, size, 0.f, 0.f, 1.f, 1.f, 1.f);
		vertices.emplace_back(-size, -size, size, 0.f, 0.f, 1.f, 0.f, 0.f);
		vertices.emplace_back(size, size, size, 0.f, 0.f, 1.f, 1.f, 1.f);
		vertices.emplace_back(-size, size, size, 0.f, 0.f, 1.f, 0.f, 1.f);
		// Back face
		vertices.emplace_back(-size, -size, -size, 0.f, 0.f, -1.f, 1.f, 0.f);
		vertices.emplace_back(size, -size, -size, 0.f, 0.f, -1.f, 0.f, 0.f);
		vertices.emplace_back(size, size, -size, 0.f, 0.f, -1.f, 0.f, 1.f);
		vertices.emplace_back(-size, -size, -size, 0.f, 0.f, -1.f, 1.f, 0.f);
		vertices.emplace_back(size, size, -size, 0.f, 0.f, -1.f, 0.f, 1.f);
		vertices.emplace_back(-size, size, -size, 0.f, 0.f, -1.f, 1.f, 1.f);
		// Left face
		vertices.emplace_back(-size, -size, -size, -1., 0., 0., 0., 0.);
		vertices.emplace_back(-size, -size, size, -1., 0., 0., 1., 0.);
		vertices.emplace_back(-size, size, size, -1., 0., 0., 1., 1.);
		vertices.emplace_back(-size, -size, -size, -1., 0., 0., 0., 0.);
		vertices.emplace_back(-size, size, size, -1., 0., 0., 1., 1.);
		vertices.emplace_back(-size, size, -size, -1., 0., 0., 0., 1.);
		// Right face
		vertices.emplace_back(size, -size, -size, 1., 0., 0., 1., 0.);
		vertices.emplace_back(size, -size, size, 1., 0., 0., 0., 0.);
		vertices.emplace_back(size, size, size, 1., 0., 0., 0., 1.);
		vertices.emplace_back(size, -size, -size, 1., 0., 0., 1., 0.);
		vertices.emplace_back(size, size, size, 1., 0., 0., 0., 1.);
		vertices.emplace_back(size, size, -size, 1., 0., 0., 1., 1.);
		// Top face
		vertices.emplace_back(-size, size, -size, 0., 1., 0., 0., 1.);
		vertices.emplace_back(size, size, size, 0., 1., 0., 1., 0.);
		vertices.emplace_back(-size, size, size, 0., 1., 0., 0., 0.);
		vertices.emplace_back(-size, size, -size, 0., 1., 0., 0., 1.);
		vertices.emplace_back(size, size, size, 0., 1., 0., 1., 0.);
		vertices.emplace_back(size, size, -size, 0., 1., 0., 1., 1.);
		// Bottom face
		vertices.emplace_back(-size, -size, -size, 0., -1., 0., 1., 1.);
		vertices.emplace_back(-size, -size, size, 0., -1., 0., 1., 0.);
		vertices.emplace_back(size, -size, size, 0., -1., 0., 0., 0.);
		vertices.emplace_back(-size, -size, -size, 0., -1., 0., 1., 1.);
		vertices.emplace_back(size, -size, size, 0., -1., 0., 0., 0.);
		vertices.emplace_back(size, -size, -size, 0., -1., 0., 0., 1.);

		// PROJECTION

		float halfWidth = Main::render_window->getSize().x / (2.0f * MapEditor::editor->_camera->_zoom);
		float halfHeight = Main::render_window->getSize().y / (2.0f * MapEditor::editor->_camera->_zoom);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-halfWidth, halfWidth, halfHeight, -halfHeight, -1000.0, 1000.0);

		// ModelView
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		// Transform
		glTranslatef(-MapEditor::editor->_camera->_position.x, -MapEditor::editor->_camera->_position.y, -15.0f);

		// Cube position on tile
		sf::Vector2i tilePos(4*16, 4*16);
		glTranslatef(tilePos.x * 16.0f + 8.0f, tilePos.y * 16.0f + 8.0f, 0.0f);

		static float rotY = 0.0f;
		glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(rotY, 0.0f, 1.0f, 0.0f);
		rotY += 0.1;
		if (rotY >= 360.0f)
			rotY = rotY - 360.0f;

		//glEnable(GL_TEXTURE_2D);
		// OpenGL states for cube
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_DEPTH_TEST);
		glColor4f(0.5f, 0.5f, 0.5f, 1.f);
		//glBindTexture(GL_TEXTURE_2D, _texture ? _texture->_texture->getNativeHandle() : 0);

		glBegin(GL_TRIANGLES);

		for (auto& vertex : vertices) {

			glNormal3f(vertex.rx, vertex.ry, vertex.rz);
			//glTexCoord2f(vertex.u, 1.0f - vertex.v);
			glVertex3f(vertex.x, vertex.y, vertex.z);

		}

		glEnd();

		Main::render_window->popGLStates();
		Main::render_window->resetGLStates();

		GUI_manager->setView();
		_main_menu->draw();
		_palette->draw();
	}

	std::shared_ptr<Editor> editor = nullptr;
}