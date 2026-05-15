// for std::wcout 
#include <io.h>       // _setmode, _fileno
#include <fcntl.h>   // _O_U16TEXT
#include <stdio.h>   // stdout

// includes :-) 
#include <SFML/Graphics.hpp>
#include "Window.hpp"
#include "DebugLog.hpp"
#include "Time.hpp"

#include "Theme.hpp"
#include "TexturesManager.hpp"
#include "AnimationsManager.hpp"
#include "ShaderManager.hpp"
#include "PrefabsManager.hpp"

#include "Editors/Editor.hpp"
#include "Editors/MapEditor/MapEditor.hpp"

#include "Editors/MapEditor/Map/CameraOnMap.hpp"
#include "Editors/MapEditor/Map/Tileset.hpp"
#include "Editors/MapEditor/Map/Map.hpp"

#include "Editors/MapEditor/Objects/Object.hpp"
#include "Editors/MapEditor/Objects/GameObject.hpp"
#include "Editors/MapEditor/Objects/GameObjectOnMap.hpp"
#include "Editors/MapEditor/Objects/Monster.hpp"



int main()
{
    (void)_setmode(_fileno(stdout), _O_U16TEXT); // wide char UTF-16 output

    window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "Editor-RPG2D");
    
    loadTheme();

    textures_manager = std::make_shared<TexturesManager>();
    animations_manager = std::make_shared<AnimationsManager>();
	prefabs_manager = std::make_shared<PrefabsManager>();

    prefabs_manager->loadPrefabs();

    shader = std::make_shared<sf::Shader>(L"assets\\shr\\vertex.vert", L"assets\\shr\\water.frag");

    cursor = std::make_shared<Cursor>();
	GUI_manager = std::make_shared<GUIManager>();

    editor_manager = std::make_shared<EditorManager>();

	map_editor = std::make_shared<MapEditor>();
	map_editor->createTileset();
    map_editor->createMap(5, 3);
	map_editor->createGameObjects();
    map_editor->createCamera();
    map_editor->createCursorOnMap();
    map_editor->createMainMenu();
    map_editor->createPalette();
    editor_manager->push_back(map_editor);

    // init FPS clock
    sf::Clock FPSClock;
    sf::Clock FPSClockUpdate;	// clock for show FPS in main loop of Editor

    while (window->isOpen()) {
    	
		prevTime = currentTime;
		currentTime = mainClock.getElapsedTime();
		deltaTime = currentTime - prevTime;

        cursor->update();

        // FPS timer
        float FPS = 1.0f / FPSClock.restart().asSeconds();
        if (FPSClockUpdate.getElapsedTime().asSeconds() > 0.5f) {

            std::ostringstream ss;
            ss << std::fixed << std::setprecision(2) << FPS << " FPS";
            window->setTitle("Editor - RPG2D - " + ss.str());
            FPSClockUpdate.restart();
        }

		GUI_manager->Element_hovered = nullptr;
        editor_manager->cursorHover();

		// Handle Events
        while (const std::optional event = window->pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window->close();
            } 

            editor_manager->handleEvent(*event);
  
        }

        editor_manager->update();


        window->clear(sf::Color(47, 47, 47));
		editor_manager->draw();
        window->display();
    }
}