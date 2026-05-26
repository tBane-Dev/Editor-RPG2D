#include "AnimationsManager.hpp"
#include "DebugLog.hpp"

Animations::Animations(std::wstring path, int animationsCount, int framesCount, bool& loadingStatus) {
	
	_path = path;
	
	_animationsCount = animationsCount;
	_framesCount = framesCount;
	
	_texture = textures_manager->getTexture(path);
        if (!_texture) {
                loadingStatus = false;
        }
}

Animations::~Animations() {
	
}
    
sf::IntRect Animations::getFrameRect(int animation, int frame) {
	sf::IntRect frameRect;
	frameRect.size.x = _texture->getSize().x / _framesCount;
	frameRect.size.y = _texture->getSize().y / _animationsCount;
	
	frameRect.position.x = frame * frameRect.size.x;
	frameRect.position.y = animation * frameRect.size.y;
	
	return frameRect;
}

std::shared_ptr<Texture> Animations::getTexture() {
	return _texture;
}

AnimationsManager::AnimationsManager() {
	_animations.clear();
	
	loadAllAnimations();
}

AnimationsManager::~AnimationsManager() {
	
}

std::shared_ptr<Animations> AnimationsManager::getAnimations(std::wstring path) {
	for(auto& anim : _animations) {
		if(anim->_path == path) {
		    return anim;
		}
	}
	
	return nullptr;
}

void AnimationsManager::loadAnimations(std::wstring path, int animationsCount, int framesCount) {
	
	    bool loadingStatus = true;
        std::shared_ptr<Animations> animations = std::make_shared<Animations>(path, animationsCount, framesCount,  loadingStatus);
        
        if(loadingStatus)
                _animations.push_back(animations);
}
    
void AnimationsManager::loadAllAnimations() {
	
	struct Data {
		std::wstring _path;
		int _animationsCount;
		int _framesCount;
		
		Data(std::wstring path, int animationsCount, int framesCount) {
			_path = path;
		    _animationsCount = animationsCount;
		    _framesCount = framesCount;
		}
	};
        
		
		// textures
		std::vector<Data> datas;

        datas.emplace_back(L"assets\\tex\\monsters\\golem.png", 4, 4);
        datas.emplace_back(L"assets\\tex\\monsters\\dziobak.png", 4, 4);
        datas.emplace_back(L"assets\\tex\\monsters\\goblin.png", 4, 4);
        datas.emplace_back(L"assets\\tex\\monsters\\bies.png", 4, 4);

        datas.emplace_back(L"assets\\tex\\tree_1.png", 1, 1);
        datas.emplace_back(L"assets\\tex\\boulder_1.png", 1, 1);
        datas.emplace_back(L"assets\\tex\\boulder_2.png", 1, 1);


        // load all textures
        for (auto& data : datas) {
                loadAnimations(data._path, data._animationsCount, data._framesCount);
        }

        // Loaded textures
        DebugLog(L"Loading animations:");
        for (auto& data : datas) {
                if (getAnimations(data._path)) {
                        DebugStat(data._path);
                }
        }

        // Failed textures
        bool failed = false;
        for (auto& data : datas) {
                if (!getAnimations(data._path)) {
                        if (!failed) {
                                DebugError(L"Failed to load animations:");
                        }
                        DebugError(data._path);
                        failed = true;
                }
        }

        if (failed) {
                exit(0);
        }

}

std::shared_ptr<AnimationsManager> animations_manager = nullptr;
