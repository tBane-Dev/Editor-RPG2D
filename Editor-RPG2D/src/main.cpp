// for std::wcout 
#include <io.h>       // _setmode, _fileno
#include <fcntl.h>   // _O_U16TEXT
#include <stdio.h>   // stdout

// includes :-) 
#include <SFML/Graphics.hpp>
#include "RenderWindow.hpp"
#include "DebugLog.hpp"
#include "Time.hpp"

#include "Theme.hpp"

#include "Tooltip.hpp" // to move

#include "TexturesManager.hpp"
#include "AnimationsManager.hpp"
#include "ShadersManager.hpp"
#include "Tileset.hpp"
#include "Wallset.hpp"
#include "PrefabsManager.hpp"

#include "Cursor.hpp"
#include "WindowsManager.hpp"

#include "EditorsManager.hpp"
#include "Editors/MapEditor/Editor.hpp"

#include "Editors/MapEditor/Map/CameraOnMap.hpp"
#include "Editors/MapEditor/Map/Map.hpp"

#include "Objects/Object.hpp"
#include "Objects/GameObject.hpp"
#include "Objects/Monster.hpp"
#include "Editors/MapEditor/Map/GameObjectOnMap.hpp"

#include <windows.h>
#include <commdlg.h>
#include <iostream>


int main() {
    (void)_setmode(_fileno(stdout), _O_U16TEXT); // wide char UTF-16 output

    Main::render_window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Editor-RPG2D");

    loadTheme();

    textures_manager = std::make_shared<TexturesManager>();
    animations_manager = std::make_shared<AnimationsManager>();
    prefabs_manager = std::make_shared<PrefabsManager>();

    prefabs_manager->loadBasicPrefabs();

    loadShaders();

    Main::cursor = std::make_shared<Main::Cursor>();
    GUI_manager = std::make_shared<GUIManager>();
    Main::tooltip = std::make_shared<Main::Tooltip>();

    Main::windows_manager = std::make_shared<Main::WindowsManager>();
    Main::editor_manager = std::make_shared<Main::EditorsManager>();

	tileset = std::make_shared<Tileset>();
	wallset = std::make_shared<Wallset>();
	//floorset = std::make_shared<Floorset>();

    MapEditor::editor = std::make_shared<MapEditor::Editor>();
    MapEditor::editor->createMap(5, 3);
    MapEditor::editor->createGameObjects();
    MapEditor::editor->createCamera();
    MapEditor::editor->createCursorOnMap();
    MapEditor::editor->createMainMenu();
    MapEditor::editor->createPalette();
    

    Main::editor_manager->push_back(MapEditor::editor);

	// TO-DO - to delete (center the camera on the map at the start of the program)
    sf::Vector2f camPos;
	camPos.x = MapEditor::editor->_map->getRect().size.x / 2 + MapEditor::editor->_palette->getSize().x/2;
    camPos.y = MapEditor::editor->_camera->_position.y;
    MapEditor::editor->_camera->_position = camPos;
    MapEditor::editor->_camera->update();
    //

    MapEditor::editor->setVisibleChunks();

    // init FPS clock
    sf::Clock FPSClock;
    int frames = 0;
    float elapsed = 0.0f;


    while (Main::render_window->isOpen()) {

        prevTime = currentTime;
        currentTime = mainClock.getElapsedTime();
        deltaTime = currentTime - prevTime;

        if(deltaTime.asSeconds() > 0.1f) {
            deltaTime = sf::seconds(0.1f);
		}

        Main::cursor->update();

        // FPS TIMER
        float dtFPS = FPSClock.restart().asSeconds();
        frames++;
        elapsed += dtFPS;

        if (elapsed >= 0.5f) {
            float FPS = frames / elapsed;
            float ms = 1000.0f / FPS;

            std::ostringstream ss;
            ss << "Editor - RPG2D - " << std::fixed << std::setprecision(2) << FPS << " FPS / " << std::setprecision(3) << ms << " ms";
            Main::render_window->setTitle(ss.str());
            frames = 0;
            elapsed = 0.0f;
        }

        GUI_manager->Element_hovered = nullptr;
        Main::editor_manager->cursorHover();
		Main::windows_manager->cursorHover();

        // handle events
        while (const std::optional event = Main::render_window->pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                Main::render_window->close();
            }

            if(event->is<sf::Event::Resized>()) {
				sf::Vector2u newSize = event->getIf<sf::Event::Resized>()->size;
                Main::render_window->setSize(newSize);
                GUI_manager->resize(sf::Vector2i(newSize));
			}

            Main::editor_manager->handleEvent(*event);
            Main::windows_manager->handleEvent(*event);

        }

        // update
        Main::editor_manager->update();
        Main::windows_manager->update();
		Main::tooltip->update();

        // TO-DO - to delete
        // I Am The One! .. B-)
        //DebugStat(L"I am The One ..");

        // draw
        Main::render_window->clear(sf::Color(47, 47, 47));
        Main::editor_manager->draw();
        Main::windows_manager->draw();
        Main::tooltip->draw();
        Main::render_window->display();

        
    }
}
