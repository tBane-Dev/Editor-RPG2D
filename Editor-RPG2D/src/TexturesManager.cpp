#include "TexturesManager.hpp"
#include "DebugLog.hpp"

Texture::Texture(std::wstring path, bool& loadingStatus) {
    _path = path;
    _texture = std::make_shared<sf::Texture>();

    if (!_texture->loadFromFile(path)) {
        loadingStatus = false;
    }
}

Texture::Texture(std::wstring path, std::shared_ptr<sf::Image> image) {
    _path = path;

    _texture = std::make_shared<sf::Texture>();
    _texture->loadFromImage(*image);
}

Texture::~Texture() {

}

sf::Vector2i Texture::getSize() {
    if (_texture == nullptr)
        return sf::Vector2i(0, 0);

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

    // tileset
    texturePaths.push_back(L"assets\\tex\\tileset.png");

	// controls
    texturePaths.push_back(L"assets\\tex\\controls\\scrollbar32\\up.png");
    texturePaths.push_back(L"assets\\tex\\controls\\scrollbar32\\center.png");
    texturePaths.push_back(L"assets\\tex\\controls\\scrollbar32\\down.png");
    texturePaths.push_back(L"assets\\tex\\controls\\scrollbar32\\slider_top.png");
    texturePaths.push_back(L"assets\\tex\\controls\\scrollbar32\\slider_center.png");
    texturePaths.push_back(L"assets\\tex\\controls\\scrollbar32\\slider_bottom.png");

    texturePaths.push_back(L"assets\\tex\\controls\\scrollbar24\\up.png");
    texturePaths.push_back(L"assets\\tex\\controls\\scrollbar24\\center.png");
    texturePaths.push_back(L"assets\\tex\\controls\\scrollbar24\\down.png");
    texturePaths.push_back(L"assets\\tex\\controls\\scrollbar24\\slider_top.png");
    texturePaths.push_back(L"assets\\tex\\controls\\scrollbar24\\slider_center.png");
    texturePaths.push_back(L"assets\\tex\\controls\\scrollbar24\\slider_bottom.png");

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

    // windows
    texturePaths.push_back(L"assets\\tex\\windows\\close.png");
    texturePaths.push_back(L"assets\\tex\\windows\\close_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\close_press.png");
    texturePaths.push_back(L"assets\\tex\\windows\\button.png");
    texturePaths.push_back(L"assets\\tex\\windows\\button_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\button_press.png");
    texturePaths.push_back(L"assets\\tex\\windows\\confirm.png");
    texturePaths.push_back(L"assets\\tex\\windows\\confirm_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\confirm_press.png");
    texturePaths.push_back(L"assets\\tex\\windows\\cancel.png");
    texturePaths.push_back(L"assets\\tex\\windows\\cancel_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\cancel_press.png");

    // file dialog
    
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\back.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\back_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\back_press.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\next.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\next_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\next_press.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\up.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\up_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\up_press.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\navbar_pc.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\pc.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\pc_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\pc_press.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\arrow.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\arrow_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\arrow_press.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\directory.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\directory_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\directory_press.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\file.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\file_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\file_press.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\harddrive.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\harddrive_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\harddrive_press.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\directoryToOpen.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\directoryToOpen_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\directoryToClose.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\directoryToClose_hover.png");
    texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\button.png");
	texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\button_hover.png");
	texturePaths.push_back(L"assets\\tex\\windows\\file_dialog\\button_press.png");
    

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
    texturePaths.push_back(L"assets\\tex\\palette\\slots\\slot_80_inactive.png");
    texturePaths.push_back(L"assets\\tex\\palette\\slots\\selected_80.png");
    texturePaths.push_back(L"assets\\tex\\palette\\slots\\selected_80_hover.png");
    texturePaths.push_back(L"assets\\tex\\palette\\slots\\selected_80_press.png");

    texturePaths.push_back(L"assets\\tex\\palette\\slots\\slot_160.png");
    texturePaths.push_back(L"assets\\tex\\palette\\slots\\slot_160_hover.png");
    texturePaths.push_back(L"assets\\tex\\palette\\slots\\slot_160_press.png");
    texturePaths.push_back(L"assets\\tex\\palette\\slots\\slot_160_inactive.png");
    texturePaths.push_back(L"assets\\tex\\palette\\slots\\selected_160.png");
    texturePaths.push_back(L"assets\\tex\\palette\\slots\\selected_160_hover.png");
    texturePaths.push_back(L"assets\\tex\\palette\\slots\\selected_160_press.png");
    
    texturePaths.push_back(L"assets\\tex\\palette\\none.png");

    // preview panel
    texturePaths.push_back(L"assets\\tex\\editors_ui\\first.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\first_hover.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\first_press.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\first_inactive.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\prev.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\prev_hover.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\prev_press.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\prev_inactive.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\play.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\play_hover.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\play_press.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\play_inactive.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\pause.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\pause_hover.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\pause_press.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\pause_inactive.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\next.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\next_hover.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\next_press.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\next_inactive.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\last.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\last_hover.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\last_press.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\last_inactive.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_prev.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_prev_hover.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_prev_press.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_prev_inactive.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_next.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_next_hover.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_next_press.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\anim_next_inactive.png");

    texturePaths.push_back(L"assets\\tex\\editors_ui\\bottomButton.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\bottomButton_hover.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\bottomButton_press.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\bottomButton_inactive.png");

    texturePaths.push_back(L"assets\\tex\\editors_ui\\mediumButton.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\mediumButton_hover.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\mediumButton_press.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\mediumButton_inactive.png");

    texturePaths.push_back(L"assets\\tex\\editors_ui\\largeButton.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\largeButton_hover.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\largeButton_press.png");
    texturePaths.push_back(L"assets\\tex\\editors_ui\\largeButton_inactive.png");
    
    // prefabs
    texturePaths.push_back(L"assets\\tex\\monsters\\golem.png");
    texturePaths.push_back(L"assets\\tex\\monsters\\troll.png");
    texturePaths.push_back(L"assets\\tex\\monsters\\dziobak.png");
    texturePaths.push_back(L"assets\\tex\\monsters\\deer.png");
    texturePaths.push_back(L"assets\\tex\\monsters\\goblin.png");
    texturePaths.push_back(L"assets\\tex\\monsters\\bies.png");
    
    texturePaths.push_back(L"assets\\tex\\monsters\\hero.png");
	texturePaths.push_back(L"assets\\tex\\tree_1.png");
	texturePaths.push_back(L"assets\\tex\\boulder_1.png");
	texturePaths.push_back(L"assets\\tex\\boulder_2.png");
	texturePaths.push_back(L"assets\\tex\\monsters\\monster.png");

    // Ghost Chunks
	texturePaths.push_back(L"assets\\tex\\ghost_chunk_plus.png");
	texturePaths.push_back(L"assets\\tex\\ghost_chunk_plus_hover.png");
	texturePaths.push_back(L"assets\\tex\\ghost_chunk_plus_press.png");



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