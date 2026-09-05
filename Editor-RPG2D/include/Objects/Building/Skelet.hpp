#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.hpp"
#include "Objects/PlacedGameObject.hpp"

class Building;

class SkeletPrefab : public GameObject {
public:
	int _id = -1;
	sf::Texture _texture;

	SkeletPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh, int id);
	~SkeletPrefab();
};

class SkeletSet {
public:
	std::vector<std::shared_ptr<SkeletPrefab>> _skelets;

	SkeletSet();
	~SkeletSet();

	std::shared_ptr<SkeletPrefab> getSkelet(int id);
};

extern std::shared_ptr<SkeletSet> skeletset;

class Skelet : public PlacedGameObject {
public:
	sf::IntRect _rect;
	std::weak_ptr<Building> _building;

	Skelet(std::weak_ptr<GameObject> prefab, sf::IntRect rect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(0, 0)), std::weak_ptr<Building> building = std::weak_ptr<Building>());
	~Skelet();

	virtual void setPosition(sf::Vector2i position);

	void draw(sf::RenderTarget& target, float scale, int drawType);
	virtual void draw();
};