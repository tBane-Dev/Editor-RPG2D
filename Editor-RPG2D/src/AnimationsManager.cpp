#include "AnimationsManager.hpp"
#include "DebugLog.hpp"
#include "BinaryWriter.hpp"
#include "BinaryReader.hpp"

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

void AnimationsManager::save(std::ofstream& saver) {

	BinaryWriter writer(saver);

	writer.write_int32(_animations.size());
	//DebugLog(L"Saving animations count: " + std::to_wstring(_animations.size()));
		
	for (auto& a : _animations) {
		writer.write_wstring(a->_path);
		writer.write_Vector2i(a->_frameSize);
		writer.write_int32(a->_animationsCount);
		writer.write_int32(a->_framesCount);
		writer.write_int32(a->_offsetX);
		writer.write_int32(a->_offsetY);
		writer.write_Image(a->_texture->_texture->copyToImage());

		//DebugLog(L"Saved animations: " + a->_path);
		//DebugLog(L"Frame size: " + std::to_wstring(a->_frameSize.x) + L"x" + std::to_wstring(a->_frameSize.y));
		//DebugLog(L"Animations count: " + std::to_wstring(a->_animationsCount));
		//DebugLog(L"Frames count: " + std::to_wstring(a->_framesCount));
		//DebugLog(L"Offset: " + std::to_wstring(a->_offsetX) + L"," + std::to_wstring(a->_offsetY));
		//DebugLog(L"Texture size: " + std::to_wstring(a->_texture->getSize().x) + L"x" + std::to_wstring(a->_texture->getSize().y));
		//DebugLog(L"----");
	}
}

void AnimationsManager::load(std::ifstream& loader) {
	BinaryReader reader(loader);

	_animations.clear();

	int animationsCount = reader.read_int32();

	for (int i = 0; i < animationsCount; i++) {
		std::wstring path = reader.read_wstring();
		sf::Vector2i frameSize = reader.read_Vector2i();
		int animationsCount = reader.read_int32();
		int framesCount = reader.read_int32();
		int offsetX = reader.read_int32();
		int offsetY = reader.read_int32();
		std::shared_ptr<sf::Image> image = std::make_shared<sf::Image>(reader.read_Image());
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(path, image);
		std::shared_ptr<Animations> animations = std::make_shared<Animations>(path, texture, frameSize, animationsCount, framesCount, offsetX, offsetY);
		addAnimations(animations);

		//DebugLog(L"Loaded animations: " + path);
		//DebugLog(L"Frame size: " + std::to_wstring(frameSize.x) + L"x" + std::to_wstring(frameSize.y));
		//DebugLog(L"Animations count: " + std::to_wstring(animationsCount));
		//DebugLog(L"Frames count: " + std::to_wstring(framesCount));
		//DebugLog(L"Offset: " + std::to_wstring(offsetX) + L"," + std::to_wstring(offsetY));
		//DebugLog(L"Texture size: " + std::to_wstring(texture->getSize().x) + L"x" + std::to_wstring(texture->getSize().y));
		//DebugLog(L"----");
	}
}

void AnimationsManager::loadAnimations(std::wstring path, sf::Vector2i frameSize, int animationsCount, int framesCount) {
	
	bool loadingStatus = true;
    std::shared_ptr<Animations> animations = std::make_shared<Animations>(path, frameSize, animationsCount, framesCount,  loadingStatus);
    
    if(loadingStatus)
        _animations.push_back(animations);
}
    
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
    datas.emplace_back(L"assets\\tex\\monsters\\deer.png", sf::Vector2i(128, 128), 4, 4);
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
