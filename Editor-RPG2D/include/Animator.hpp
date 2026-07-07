#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationsManager.hpp"

class Animator {
public:
	std::weak_ptr<Animations> _animations;
	int _animation;
	int _frame;
	float _timer;
	float _interval;
	bool _isPlaying;

	Animator(std::weak_ptr<Animations> animations, float interval);
	~Animator();

	
	void setAnimation(int animation);
	void setFrame(int frame);
	void prevAnimation();
	void nextAnimation();
	void firstFrame();
	void prevFrame();
	void nextFrame();
	void lastFrame();
	void setRandTime();
	void setRandFrame();
	void play();
	void stop();
	void pause();
	void update();

	std::weak_ptr<Animations> getAnimations();


};