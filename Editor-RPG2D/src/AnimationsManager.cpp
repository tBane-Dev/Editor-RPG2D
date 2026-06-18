#include "AnimationsManager.hpp"
#include "DebugLog.hpp"

Animations::Animations(std::wstring path, sf::Vector2i frameSize, int animationsCount, int framesCount, bool& loadingStatus, int offsetX, int offsetY) {
	
	_path = path;

	_frameSize = frameSize;
	_animationsCount = animationsCount;
	_framesCount = framesCount;

	_texture = textures_manager->getTexture(path);

	_offsetX = offsetX;
	_offsetY = offsetY;

	if (!_texture) {
        loadingStatus = false;
    }
}

Animations::Animations(std::wstring name, std::shared_ptr<Texture> texture, sf::Vector2i frameSize, int animationsCount, int framesCount, int offsetX, int offsetY) {
	_path = name;

	_frameSize = frameSize;
	_animationsCount = animationsCount;
	_framesCount = framesCount;

	_texture = texture;

	_offsetX = offsetX;
	_offsetY = offsetY;
}

Animations::~Animations() {
	
}
    
sf::IntRect Animations::getFrameRect(int animation, int frame) {
	
	if (!_texture || _framesCount <= 0 || _animationsCount <= 0)
		return sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(0, 0));

	sf::IntRect frameRect;
	
	frameRect.size = _frameSize;
	frameRect.position.x = frame * frameRect.size.x + _offsetX;
	frameRect.position.y = animation * frameRect.size.y + _offsetY;
	
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

std::weak_ptr<Animations> AnimationsManager::getAnimations(std::wstring path) {
	for (auto& anim : _animations) {
		if (anim && anim->_path == path) {
			return anim;
		}
	}

	return std::weak_ptr<Animations>();
}

std::weak_ptr<Animations> AnimationsManager::getAnimations(int index) {
	if (index >= 0 && index < (int)_animations.size()) {
		return _animations[index];
	}

	return std::weak_ptr<Animations>();
}

int AnimationsManager::getAnimationsID(std::wstring path) {
	for (int i = 0; i < (int)_animations.size(); i++) {
		if (_animations[i] && _animations[i]->_path == path) {
			return i;
		}
	}

	return -1;
}

void AnimationsManager::removeAnimations(int index) {
	if (index >= 0 && index < (int)_animations.size()) {
		_animations.erase(_animations.begin() + index);
	}
}

int AnimationsManager::getAnimationsCount() {
	return (int)_animations.size();
}

int AnimationsManager::addAnimations(std::wstring name, std::shared_ptr<Texture> texture, sf::Vector2i frameSize, int animationsCount, int framesCount) {
	std::shared_ptr<Animations> animations = std::make_shared<Animations>(name, texture, frameSize, animationsCount, framesCount);
	_animations.push_back(animations);
	return _animations.size() - 1;
}

int AnimationsManager::addAnimations(std::shared_ptr<Animations> animations) {
	_animations.push_back(animations);
	return _animations.size() - 1;
}


void AnimationsManager::loadAnimations(std::wstring path, sf::Vector2i frameSize, int animationsCount, int framesCount) {
	
	bool loadingStatus = true;
    std::shared_ptr<Animations> animations = std::make_shared<Animations>(path, frameSize, animationsCount, framesCount,  loadingStatus);
    
    if(loadingStatus)
        _animations.push_back(animations);
}
    \
void AnimationsManager::loadAllAnimations() {
	
	struct Data {
		std::wstring _path;
		sf::Vector2i _frameSize;
		int _animationsCount;
		int _framesCount;
		
		Data(std::wstring path, sf::Vector2i frameSize, int animationsCount, int framesCount) {
			_path = path;
			_frameSize = frameSize;
		    _animationsCount = animationsCount;
		    _framesCount = framesCount;
		}
	};
        
		
	// textures
	std::vector<Data> datas;

    datas.emplace_back(L"assets\\tex\\monsters\\golem.png", sf::Vector2i(128,128), 4, 4);
    datas.emplace_back(L"assets\\tex\\monsters\\troll.png", sf::Vector2i(128, 128), 4, 4);
    datas.emplace_back(L"assets\\tex\\monsters\\dziobak.png", sf::Vector2i(128, 128), 4, 4);
    datas.emplace_back(L"assets\\tex\\monsters\\goblin.png", sf::Vector2i(128, 128), 4, 4);
    datas.emplace_back(L"assets\\tex\\monsters\\bies.png", sf::Vector2i(128, 128), 4, 4);
    datas.emplace_back(L"assets\\tex\\monsters\\hero.png", sf::Vector2i(128, 208), 4, 4);
    datas.emplace_back(L"assets\\tex\\monsters\\monster.png", sf::Vector2i(128, 128), 1, 1);
    
    datas.emplace_back(L"assets\\tex\\tree_1.png", sf::Vector2i(256, 256), 1, 1);
    datas.emplace_back(L"assets\\tex\\boulder_1.png", sf::Vector2i(128, 128), 1, 1);
    datas.emplace_back(L"assets\\tex\\boulder_2.png", sf::Vector2i(128, 128), 1, 1);
    
    
    // load all textures
    for (auto& data : datas) {
        loadAnimations(data._path, data._frameSize, data._animationsCount, data._framesCount);
    }
    
    // Loaded textures
    DebugLog(L"Loading animations:");
    for (auto& data : datas) {
		if (getAnimations(data._path).expired()) {
			DebugStat(data._path);
        }
    }
    
    // Failed textures
    bool failed = false;
    for (auto& data : datas) {
		if (getAnimations(data._path).expired()) {
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
