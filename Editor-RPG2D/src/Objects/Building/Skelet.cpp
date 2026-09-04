#pragma once
#include "Objects/Building/Skelet.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"
#include "EditorsManager.hpp"

SkeletPrefab::SkeletPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh, int id) : GameObject(name, animations, origin, collider, mesh) {
	_type = ObjectType::Skelet;
	_id = id;
}

SkeletPrefab::~SkeletPrefab() {

}

SkeletSet::SkeletSet() {

	_skelets.clear();

	_skelets.push_back(std::make_shared<SkeletPrefab>(L"skelet1", std::weak_ptr<Animations>(), sf::Vector2i(8, 8), std::make_shared<RectangularCollider>(0, 0, 16, 16), nullptr, 0));
	_skelets.push_back(std::make_shared<SkeletPrefab>(L"skelet2", std::weak_ptr<Animations>(), sf::Vector2i(8, 8), std::make_shared<RectangularCollider>(0, 0, 16, 16), nullptr, 1));
	_skelets.push_back(std::make_shared<SkeletPrefab>(L"skelet3", std::weak_ptr<Animations>(), sf::Vector2i(8, 8), std::make_shared<RectangularCollider>(0, 0, 16, 16), nullptr, 2));
	_skelets.push_back(std::make_shared<SkeletPrefab>(L"skelet4", std::weak_ptr<Animations>(), sf::Vector2i(8, 8), std::make_shared<RectangularCollider>(0, 0, 16, 16), nullptr, 3));
}

SkeletSet::~SkeletSet() {

}

std::shared_ptr<SkeletPrefab> SkeletSet::getSkelet(int id) {
	for (auto& skelet : _skelets) {
		if (skelet->_id == id)
			return skelet;
	}
	return nullptr;
}

std::shared_ptr<SkeletSet> skeletset = nullptr;

Skelet::Skelet(std::weak_ptr<GameObject> prefab, sf::IntRect rect) : GameObjectOnMap(prefab) {
	_type = ObjectType::Skelet;
	_rect = rect;
}


Skelet::~Skelet() {

}

void Skelet::setPosition(sf::Vector2i position) {
	_rect.position = position;
}

void Skelet::draw(sf::RenderTarget& target, float scale) {

	if (_prefab.expired())
		return;

	std::shared_ptr<SkeletPrefab> skeletPrefab = std::dynamic_pointer_cast<SkeletPrefab>(_prefab.lock());

	if (!skeletPrefab)
		return;

	DebugLog(L"draw the skelet");

	sf::Sprite left(*textures_manager->getTexture(L"assets\\tex\\building_skelet.png")->_texture);
	left.setPosition(sf::Vector2f(_rect.position));
	left.setScale(sf::Vector2f(scale, scale));
	target.draw(left);

}

void Skelet::draw(sf::RenderTarget& target, float scale, int drawType) {

	if (_prefab.expired())
		return;

	std::shared_ptr<SkeletPrefab> skeletPrefab = std::dynamic_pointer_cast<SkeletPrefab>(_prefab.lock());

	if (!skeletPrefab)
		return;

	DebugLog(L"draw the skelet with drawType: " + std::to_wstring(drawType));

	sf::Sprite left(*textures_manager->getTexture(L"assets\\tex\\building_skelet.png")->_texture);
	left.setPosition(sf::Vector2f(_rect.position));
	left.setScale(sf::Vector2f(scale, scale));
	target.draw(left);
}

void Skelet::draw() {
	draw(*Main::render_window, 1.f);
}