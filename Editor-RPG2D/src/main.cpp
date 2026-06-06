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
#include "TexturesManager.hpp"
#include "AnimationsManager.hpp"
#include "ShaderManager.hpp"
#include "PrefabsManager.hpp"

#include "WindowsManager.hpp"
#include "Windows/ConfirmDialog.hpp"    // TO-DO - to delete
#include "EditorsManager.hpp"
#include "Editors/MapEditor/Editor.hpp"

#include "Editors/MapEditor/Map/CameraOnMap.hpp"
#include "Editors/MapEditor/Map/Tileset.hpp"
#include "Editors/MapEditor/Map/Map.hpp"

#include "Objects/Object.hpp"
#include "Objects/GameObject.hpp"
#include "Objects/Monster.hpp"
#include "Editors/MapEditor/Map/GameObjectOnMap.hpp"


int main() {
    (void)_setmode(_fileno(stdout), _O_U16TEXT); // wide char UTF-16 output

    Main::render_window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Editor-RPG2D");

    loadTheme();

    textures_manager = std::make_shared<TexturesManager>();
    animations_manager = std::make_shared<AnimationsManager>();
    prefabs_manager = std::make_shared<PrefabsManager>();

    prefabs_manager->loadPrefabs();

    loadShaders();

    Main::cursor = std::make_shared<Main::Cursor>();
    GUI_manager = std::make_shared<GUIManager>();

    Main::windows_manager = std::make_shared<Main::WindowsManager>();
    Main::editor_manager = std::make_shared<Main::EditorsManager>();

    MapEditor::editor = std::make_shared<MapEditor::Editor>();
    MapEditor::editor->createTileset();
    MapEditor::editor->createMap(5, 3);
    MapEditor::editor->createGameObjects();
    MapEditor::editor->createCamera();
    MapEditor::editor->createCursorOnMap();
    MapEditor::editor->createMainMenu();
    MapEditor::editor->createPalette();

    Main::editor_manager->push_back(MapEditor::editor);

    // test windows
    Main::windows_manager->push_back(std::make_shared<ConfirmDialog>(L"Test Confirm Window", L"Lorem ipsum dolor sit amet, consectetur\nadipiscing elit, sed do eiusmod tempor\nincididunt ut labore et dolore magna\naliqua."));
    Main::windows_manager->push_back(std::make_shared<ConfirmDialog>(L"Test Confirm Window 2", L"Lorem ipsum dolor sit amet ..."));

    // init FPS clock
    sf::Clock FPSClock;
    sf::Clock FPSClockUpdate;	// clock for show FPS in main loop of Editor

    while (Main::render_window->isOpen()) {

        prevTime = currentTime;
        currentTime = mainClock.getElapsedTime();
        deltaTime = currentTime - prevTime;

        Main::cursor->update();

        // FPS timer
        float FPS = 1.0f / FPSClock.restart().asSeconds();
        if (FPSClockUpdate.getElapsedTime().asSeconds() > 0.5f) {

            std::ostringstream ss;
            ss << std::fixed << std::setprecision(2) << FPS << " FPS";
            Main::render_window->setTitle("Editor - RPG2D - " + ss.str());
            FPSClockUpdate.restart();
        }

        GUI_manager->Element_hovered = nullptr;
        Main::editor_manager->cursorHover();
		Main::windows_manager->cursorHover();

        // handle events
        while (const std::optional event = Main::render_window->pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                Main::render_window->close();
            }

            Main::editor_manager->handleEvent(*event);
            Main::windows_manager->handleEvent(*event);

        }

        // update
        Main::editor_manager->update();
        Main::windows_manager->update();
        DebugStat(L"I am The One ..");

        // draw
        Main::render_window->clear(sf::Color(47, 47, 47));
        Main::editor_manager->draw();
        Main::windows_manager->draw();
        Main::render_window->display();
    }
}
