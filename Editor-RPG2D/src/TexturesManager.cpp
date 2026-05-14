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

        // main menu
        texturePaths.push_back(L"assets\\tex\\main_menu\\new_project.png");
        texturePaths.push_back(L"assets\\tex\\main_menu\\open_project.png");
        texturePaths.push_back(L"assets\\tex\\main_menu\\save_project.png");
        texturePaths.push_back(L"assets\\tex\\main_menu\\undo.png");
        texturePaths.push_back(L"assets\\tex\\main_menu\\redo.png");
        
        // check
        texturePaths.push_back(L"assets\\tex\\checked.png");
        texturePaths.push_back(L"assets\\tex\\checked_hover.png");
        texturePaths.push_back(L"assets\\tex\\unchecked.png");
        texturePaths.push_back(L"assets\\tex\\unchecked_hover.png");

        // palette
        texturePaths.push_back(L"assets\\tex\\palette\\prev.png");
        texturePaths.push_back(L"assets\\tex\\palette\\prev_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\next.png");
        texturePaths.push_back(L"assets\\tex\\palette\\next_hover.png");

        texturePaths.push_back(L"assets\\tex\\palette\\group.png");
        texturePaths.push_back(L"assets\\tex\\palette\\group_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\group_selected.png");
        texturePaths.push_back(L"assets\\tex\\palette\\group_selected_hover.png");

        texturePaths.push_back(L"assets\\tex\\palette\\terrain.png");
        texturePaths.push_back(L"assets\\tex\\palette\\natures.png");
        texturePaths.push_back(L"assets\\tex\\palette\\monsters.png");

        texturePaths.push_back(L"assets\\tex\\palette\\slot.png");
        texturePaths.push_back(L"assets\\tex\\palette\\slot_hover.png");
        texturePaths.push_back(L"assets\\tex\\palette\\slot_press.png");

        // tileset
        texturePaths.push_back(L"assets\\tex\\tileset.png");
        
        // prefas
        texturePaths.push_back(L"assets\\tex\\monsters\\golem.png");
        texturePaths.push_back(L"assets\\tex\\monsters\\dziobak.png");
        texturePaths.push_back(L"assets\\tex\\monsters\\goblin.png");
		texturePaths.push_back(L"assets\\tex\\tree_1.png");
		texturePaths.push_back(L"assets\\tex\\boulder_1.png");
		texturePaths.push_back(L"assets\\tex\\boulder_2.png");


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