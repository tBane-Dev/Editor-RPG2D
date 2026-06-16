#include "Animator.hpp"
#include "Time.hpp"

Animator::Animator(std::weak_ptr<Animations> animations, float interval) {
	_animations = animations;
	_animation = 0;
	_frame = 0;
	_timer = 0.0f;
	_interval = interval;
	_isPlaying = false;
}

Animator::~Animator() {

}

void Animator::setAnimation(int animation) {

	if(_animations.expired())
		return;

	if (animation < 0 || animation >= _animations.lock()->_animationsCount)
		return;

	_animation = animation;
	_frame = 0;
	_timer = 0.0f;
}

void Animator::setFrame(int frame) {

	if (frame < 0 || frame >= _animations.lock()->_framesCount) 
		return;

	_frame = frame;
	_timer = 0.0f;
}

void Animator::prevAnimation() {
	_animation--;
	if (_animation < 0)
		_animation = _animations.lock()->_animationsCount - 1;
}

void Animator::nextAnimation() {
	_animation++;
	if (_animation >= _animations.lock()->_animationsCount)
		_animation = 0;
}

void Animator::firstFrame() {
	_frame = 0;
	_timer = 0.0f;
}

void Animator::prevFrame() {
	_frame--;
	if (_frame < 0)
		_frame = _animations.lock()->_framesCount - 1;
}

void Animator::nextFrame() {

	if (_animations.expired())
		return;

	_frame++;
	if (_frame >= _animations.lock()->_framesCount)
		_frame = 0;
}

void Animator::lastFrame() {
	_frame = _animations.lock()->_framesCount - 1;
	_timer = 0.0f;
}

void Animator::setRandFrame() {

	if (_animations.expired())
		return;

	_frame = rand() % _animations.lock()->_framesCount;
	_timer = 0.0f;
}

void Animator::play() {
	_isPlaying = true;
}

void Animator::pause() {
	_isPlaying = false;
}

void Animator::stop() {
	_isPlaying = false;
	_animation = 0;
	_frame = 0;
	_timer = 0.0f;
}

void Animator::update() {
	if (!_isPlaying) return;
	if (_animations.expired()) return;

	_timer += deltaTime.asSeconds();

	if (_timer >= _interval) {

		_timer = 0.0f;
		_frame++;

		if (_frame >= _animations.lock()->_framesCount) {
			_frame = 0;
		}
	}
}

std::weak_ptr<Animations> Animator::getAnimations() {
	return _animations;
}