#pragma once
#include <SFML/Graphics.hpp>
#include "TexturesManager.hpp"

class Animations {
public:
    std::wstring _path;
    std::shared_ptr<Texture> _texture;
    int _animationsCount;
    int _framesCount;
    
    Animations(std::wstring path, std::shared_ptr<Texture> texture, int animationsCount, int framesCount);
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
    
    int addAnimations(std::wstring name, std::shared_ptr<Texture> texture, int animationsCount, int framesCount);
    std::weak_ptr<Animations> getAnimations(std::wstring path);
    std::weak_ptr<Animations> getAnimations(int index);
    int getAnimationsID(std::wstring path);
    void removeAnimations(int index);
    int getAnimationsCount();

    void loadAnimations(std::wstring path, int animationsCount, int framesCount);
    void loadAllAnimations();
};

extern std::shared_ptr<AnimationsManager> animations_manager;
