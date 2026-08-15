#include "Components/Palette/BuildingSlot.hpp"
#include "RenderWindow.hpp"


BuildingSlot::BuildingSlot(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> inactiveTexture, sf::Vector2i position, std::shared_ptr<BuildingPrefab> buildingPrefab) : Slot(texture, hoverTexture, pressTexture, inactiveTexture, position) {
	
	_object = buildingPrefab;
	_animator = nullptr;

	generate();
}

BuildingSlot::~BuildingSlot() {

}

void BuildingSlot::generate() {

	if (_object.expired())
		return;

	std::shared_ptr<BuildingPrefab> buildingPrefab = std::dynamic_pointer_cast<BuildingPrefab>(_object.lock());

	if (!buildingPrefab)
		return;

	buildingPrefab->generate(sf::Vector2i(0, 0), 1.0f, nullptr);

}
void BuildingSlot::cursorHover() {
	ButtonWithSprite::cursorHover();
}

void BuildingSlot::handleEvent(const sf::Event& event) {
	ButtonWithSprite::handleEvent(event);
}

void BuildingSlot::update() {
	ButtonWithSprite::update();
}

void BuildingSlot::draw() {
	Slot::draw();

	std::shared_ptr<BuildingPrefab> bp = std::dynamic_pointer_cast<BuildingPrefab>(_object.lock());

	if (bp) {
		sf::Vector2f size(bp->getPreviewOutsideTexture()->getSize());
		float scale = std::min(180.f / size.x, 180.f / size.y);


		sf::Sprite sprite((GUI_manager->Element_hovered == shared_from_this()) ? *bp->getPreviewInsideTexture() : *bp->getPreviewOutsideTexture());
		sprite.setScale(sf::Vector2f(scale, scale));
		sprite.setPosition(sf::Vector2f(_rect.position.x + (240.f - size.x * scale) / 2.f, _rect.position.y + (240.f - size.y * scale) / 2.f));

		Main::render_window->draw(sprite);
	}
	
}
