#pragma once
#include <SFML/Graphics.hpp>
#include "TexturesManager.hpp"

class Animations {
public:
    std::wstring _path;
    std::shared_ptr<Texture> _texture;
    int _animationsCount;
    int _framesCount;
    
    Animations(std::wstring path, int animationsCount, int framesCount, bool& loadingStatus);
    ~Animations();
    

    sf::IntRect getFrameRect(int animation, int frame);
    std::shared_ptr<Texture> getTexture();
};

class AnimationsManager {
public:
    std::vector<std::shared_ptr<Animations>> _animations;
    
    AnimationsManager();
    ~AnimationsManager();
    
    std::shared_ptr<Animations> getAnimations(std::wstring path);
    void loadAnimations(std::wstring path, int animationsCount, int framesCount);
    void loadAllAnimations();
};

extern std::shared_ptr<AnimationsManager> animations_manager;
