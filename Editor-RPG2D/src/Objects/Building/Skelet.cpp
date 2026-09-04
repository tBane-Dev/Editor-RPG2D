#pragma once
#include "Objects/Building/Skelet.hpp"
#include "RenderWindow.hpp"
#include "DebugLog.hpp"
#include "EditorsManager.hpp"
#include "Objects/Building/Building.hpp"

SkeletPrefab::SkeletPrefab(std::wstring name, std::weak_ptr<Animations> animations, sf::Vector2i origin, std::shared_ptr<Collider> collider, std::shared_ptr<Mesh> mesh, int id) : GameObject(name, animations, origin, collider, mesh) {
	_type = ObjectType::Skelet;
	_id = id;

	// create texture from skeletset.png
	sf::Texture skeletTexture = *textures_manager->getTexture(L"assets\\tex\\skeletset.png")->_texture;
	sf::RenderTexture rtex;
	rtex.resize(sf::Vector2u(16, 16));
	rtex.clear(sf::Color::Transparent);
	sf::Sprite spr(skeletTexture);
	spr.setTextureRect(sf::IntRect(sf::Vector2i(16 * _id, 0), sf::Vector2i(16, 16)));
	rtex.draw(spr);
	rtex.display();

	_texture = rtex.getTexture();

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

Skelet::Skelet(std::weak_ptr<GameObject> prefab, sf::IntRect rect, std::weak_ptr<Building> building) : GameObjectOnMap(prefab) {
	_type = ObjectType::Skelet;
	_rect = rect;
	_building = building;
}


Skelet::~Skelet() {

}

void Skelet::setPosition(sf::Vector2i position) {
	GameObjectOnMap::setPosition(position);
}

void Skelet::draw(sf::RenderTarget& target, float scale, int drawType) {
	if (_prefab.expired())
		return;

	std::shared_ptr<SkeletPrefab> skeletPrefab = std::dynamic_pointer_cast<SkeletPrefab>(_prefab.lock());

	if (!skeletPrefab)
		return;

	float leftX = _position.x + _rect.position.x * scale;
	float rightX = leftX + (_rect.size.x - 16) * scale;
	float bottomY = _position.y + _rect.position.y * scale;

	int maxI = 0;
	if (drawType == 1) maxI = 2;
	if (drawType == 2) maxI = _rect.size.y / 16;

	for (int i = 0; i < maxI; ++i) {
		float y = bottomY - i * 16.f * scale;

		sf::Sprite left(skeletPrefab->_texture);
		left.setOrigin(sf::Vector2f(0.f, 16.f));
		left.setPosition(sf::Vector2f(leftX, y));
		left.setScale(sf::Vector2f(scale, scale));
		target.draw(left);

		sf::Sprite right(skeletPrefab->_texture);
		right.setOrigin(sf::Vector2f(0.f, 16.f));
		right.setPosition(sf::Vector2f(rightX, y));
		right.setScale(sf::Vector2f(scale, scale));
		target.draw(right);
	}
}

void Skelet::draw() {

	if (_building.expired()) {
		return;
	}

	if(!_building.lock()->_renderOutsideLook)
		draw(*Main::render_window, 1.f, 1);
}