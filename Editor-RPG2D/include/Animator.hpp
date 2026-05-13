#pragma once
#include <SFML/Graphics.hpp>
#include "AnimationsManager.hpp"

class Animator {
public:
	std::shared_ptr<Animations> _animations;
	int _animation;
	int _frame;
	float _timer;
	float _interval;
	bool _isPlaying;

	Animator(std::shared_ptr<Animations> animations, float interval);
	~Animator();

	
	void setAnimation(int animation);
	void setFrame(int frame);
	void nextFrame();
	void setRandFrame();
	void play();
	void stop();
	void pause();
	void update();

	std::shared_ptr<Animations> getAnimations();


};