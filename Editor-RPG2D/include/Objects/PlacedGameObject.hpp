#pragma once
#include "Objects/Object.hpp"
#include "Objects/GameObject.hpp"
#include "Animator.hpp"



class PlacedGameObject : public Object, public std::enable_shared_from_this<PlacedGameObject> {
public:

	std::weak_ptr<GameObject> _prefab;
	std::shared_ptr<Animator> _animator;
    sf::Vector2i _position;
	
	bool _isSelected;
	std::unique_ptr<sf::Text> _text;

	PlacedGameObject(std::weak_ptr<GameObject> prefab);
	~PlacedGameObject();

	void drawFrame(sf::Color color = sf::Color(127+64, 30, 45));

	virtual sf::Vector2i getPosition();
	virtual void setPosition(sf::Vector2i position);
	virtual void cursorHover();
	virtual void update();
	virtual void draw();
};