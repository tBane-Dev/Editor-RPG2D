#include "TexturesManager.hpp"
#include "DebugLog.hpp"

Texture::Texture(std::wstring path, bool& loadingStatus) {
        _path = path;
        _texture = std::make_shared<sf::Texture>();
        if (!_texture->loadFromFile(path)) {
                loadingStatus = false;
        }
}

Texture::~Texture() {

}

sf::Vector2i Texture::getSize() {
	return sf::Vector2i(_texture->getSize());
}

TexturesManager::TexturesManager() {
	_textures.clear();
	
	loadAllTextures();
}

TexturesManager::~TexturesManager() {
	
}

std::shared_ptr<Texture> TexturesManager::getTexture(std::wstring path) {
        for (auto& t : _textures) {
                if (t->_path == path)
                        return t;
        }

        return nullptr;
}

void TexturesManager::loadTexture(std::wstring path) {

        bool loadingStatus = true;
        std::shared_ptr<Texture> texture = std::make_shared<Texture>(path, loadingStatus);

        if(loadingStatus)
                _textures.push_back(texture);
}

void TexturesManager::loadAllTextures() {

        std::vector<std::wstring> texturePaths;

		// controls
        texturePaths.push_back(L"assets\\tex\\controls\\scrollbar\\up.png");
        texturePaths.push_back(L"assets\\tex\\controls\\scrollbar\\center.png");
        texturePaths.push_back(L"assets\\tex\\controls\\scrollbar\\down.png");
        texturePaths.push_back(L"assets\\tex\\controls\\scrollbar\\slider_top.png");
        texturePaths.push_back(L"assets\\tex\\controls\\scrollbar\\slider_center.png");
        texturePaths.push_back(L"assets\\tex\\controls\\scrollbar\\slider_bottom.png");

        // main menu
        texturePaths.push_back(L"assets\\tex\\main_menu\\new_project.png");
        texturePaths.push_back(L"assets\\tex\\main_menu\\open_project.png");
        texturePaths.push_back(L"assets\\tex\\main_menu\\save_project.png");
        texturePaths.push_back(L"assets\\tex\\main_menu\\exit.png");
        texturePaths.push_back(L"assets\\tex\\main_menu\\undo.png");
        texturePaths.push_back(L"assets\\tex\\main_menu\\redo.png");
        
        // check
        texturePaths.push_back(L"assets\\tex\\checked.png");
        texturePaths.push_back(L"assets\\tex\\checked_hover.png");
        texturePaths.push_back(L"assets\\tex\\unchecked.png");
        texturePaths.push_back(L"assets\\tex\\unchecked_hover.png");

        // palette
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\prev.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\prev_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\prev_press.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\next.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\next_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\next_press.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\category.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\category_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\category_press.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\selected.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\selected_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\selected_press.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\terrain.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\natures.png");
        texturePaths.push_back(L"assets\\tex\\palette\\categories\\monsters.png");

        texturePaths.push_back(L"assets\\tex\\palette\\tools\\prev.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\prev_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\prev_press.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\next.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\next_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\next_press.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\tool.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\tool_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\tool_press.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\selected.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\selected_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\selected_press.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\cursor.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\circle.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\rect.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\decrease.png");
        texturePaths.push_back(L"assets\\tex\\palette\\tools\\increase.png");
        
        texturePaths.push_back(L"assets\\tex\\palette\\slots\\slot_80.png");
        texturePaths.push_back(L"assets\\tex\\palette\\slots\\slot_80_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\slots\\slot_80_press.png");
        texturePaths.push_back(L"assets\\tex\\palette\\slots\\selected_80.png");
        texturePaths.push_back(L"assets\\tex\\palette\\slots\\selected_80_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\slots\\selected_80_press.png");

        texturePaths.push_back(L"assets\\tex\\palette\\slots\\slot_160.png");
        texturePaths.push_back(L"assets\\tex\\palette\\slots\\slot_160_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\slots\\slot_160_press.png");
        texturePaths.push_back(L"assets\\tex\\palette\\slots\\selected_160.png");
        texturePaths.push_back(L"assets\\tex\\palette\\slots\\selected_160_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\slots\\selected_160_press.png");

        // preview panel
        texturePaths.push_back(L"assets\\tex\\editors_ui\\first.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\first_hover.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\first_press.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\prev.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\prev_hover.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\prev_press.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\play.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\play_hover.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\play_press.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\pause.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\pause_hover.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\pause_press.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\next.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\next_hover.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\next_press.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\last.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\last_hover.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\last_press.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_prev.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_prev_hover.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_prev_press.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_next.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_next_hover.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_next_press.png");

        texturePaths.push_back(L"assets\\tex\\editors_ui\\bottomButton.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\bottomButton_hover.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\bottomButton_press.png");

        texturePaths.push_back(L"assets\\tex\\editors_ui\\mediumButton.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\mediumButton_hover.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\mediumButton_press.png");

        texturePaths.push_back(L"assets\\tex\\editors_ui\\largeButton.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\largeButton_hover.png");
        texturePaths.push_back(L"assets\\tex\\editors_ui\\largeButton_press.png");

        // tileset
        texturePaths.push_back(L"assets\\tex\\tileset.png");
        
        // prefabs
        texturePaths.push_back(L"assets\\tex\\monsters\\golem.png");
        texturePaths.push_back(L"assets\\tex\\monsters\\troll.png");
        texturePaths.push_back(L"assets\\tex\\monsters\\dziobak.png");
        texturePaths.push_back(L"assets\\tex\\monsters\\goblin.png");
        texturePaths.push_back(L"assets\\tex\\monsters\\bies.png");
        texturePaths.push_back(L"assets\\tex\\monsters\\hero.png");
		texturePaths.push_back(L"assets\\tex\\tree_1.png");
		texturePaths.push_back(L"assets\\tex\\boulder_1.png");
		texturePaths.push_back(L"assets\\tex\\boulder_2.png");
		texturePaths.push_back(L"assets\\tex\\monsters\\monster.png");


        // load all textures
        for (auto& path : texturePaths) {
                loadTexture(path);
        }

        // Loaded textures
        DebugLog(L"Loading textures:");
        for (auto& path : texturePaths) {
                if (getTexture(path)) {
                        DebugStat(path);
                }
        }

        // Failed textures
        bool failed = false;
        for (auto& path : texturePaths) {
                if (!getTexture(path)) {
                        if (!failed) {
                                DebugError(L"Failed to load textures:");
                        }
                        DebugError(path);
                        failed = true;
                }
        }

        if (failed) {
                exit(0);
        }

}

std::shared_ptr<TexturesManager> textures_manager = nullptr;