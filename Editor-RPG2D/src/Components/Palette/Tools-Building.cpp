#include "Components/Palette/Tools-Building.hpp"
#include "RenderWindow.hpp"
#include "Theme.hpp"
#include "DebugLog.hpp"

ToolBuildingButton::ToolBuildingButton(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> toolTexture, sf::Vector2i position) : ButtonWithSprite(texture, hoverTexture, pressTexture, nullptr, position) {
	_toolTexture = toolTexture;
}

ToolBuildingButton::~ToolBuildingButton() {

}

void ToolBuildingButton::cursorHover() {
	if (_toolTexture)
		ButtonWithSprite::cursorHover();
}

void ToolBuildingButton::handleEvent(const sf::Event& event) {
	if (_toolTexture)
		ButtonWithSprite::handleEvent(event);
}

void ToolBuildingButton::update() {
	ButtonWithSprite::update();
}

void ToolBuildingButton::draw() {
	ButtonWithSprite::draw();

	if (_toolTexture) {
		sf::Sprite sprite(*_toolTexture->_texture);
		sprite.setPosition(sf::Vector2f(_rect.position));
		Main::render_window->draw(sprite);
	}
}

ToolsBuilding::ToolsBuilding() : Tools() {

	_text = std::make_unique<sf::Text>(basicFont, L"Build Options", 20);

	_prev = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\prev.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\prev_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\prev_press.png")
	);

	_next = std::make_shared<ButtonWithSprite>(
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\next.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\next_hover.png"),
		textures_manager->getTexture(L"assets\\tex\\palette\\tools\\next_press.png")
	);

	_inner_margin = (600 - 2 * _main_margin - 2 * _outer_margin - _prev->getSize().x - _next->getSize().x - 5 * 64) / 6;
	_rect.size = sf::Vector2i(600 - 2 * _main_margin, 64 + 2 * _main_margin + _top_margin + _inner_margin);

	_selectedTool = nullptr;
	_toolType = ToolBuildingType::None;
}

ToolsBuilding::~ToolsBuilding() {

}

void ToolsBuilding::addTool(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> hoverTexture, std::shared_ptr<Texture> pressTexture, std::shared_ptr<Texture> toolTexture, std::function<void()> function) {
	std::shared_ptr<ToolBuildingButton> toolButton = std::make_shared<ToolBuildingButton>(texture, hoverTexture, pressTexture, toolTexture);
	toolButton->_onclick_func = function;
	_tools.push_back(toolButton);
}

void ToolsBuilding::setTool(std::shared_ptr<ToolBuildingButton> button, ToolBuildingType type) {

	if (_selectedTool) {
		_selectedTool->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool.png");
		_selectedTool->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_hover.png");
		_selectedTool->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\tool_press.png");
	}

	_selectedTool = button;
	_toolType = type;

	for (auto& tool : _tools) {
		tool->setSelect(tool == button);
	}

	if (_selectedTool) {
		_selectedTool->_texture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\selected.png");
		_selectedTool->_hoverTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\selected_hover.png");
		_selectedTool->_pressTexture = textures_manager->getTexture(L"assets\\tex\\palette\\tools\\selected_press.png");
	}
}

void ToolsBuilding::setPosition(sf::Vector2i position) {

	_rect.position = position + sf::Vector2i(_main_margin, _main_margin);

	int x = _rect.position.x + _outer_margin;
	int y = _rect.position.y + _outer_margin;

	_text->setPosition(sf::Vector2f(x, y));

	y += _top_margin + _inner_margin;

	_prev->setPosition(sf::Vector2i(x, y));

	x += _prev->getSize().x + _inner_margin;

	for (auto& tool : _tools) {
		tool->setPosition(sf::Vector2i(x, y));
		x += tool->_rect.size.x + _inner_margin;
	}

	_next->setPosition(sf::Vector2i(x, y));

}

void ToolsBuilding::cursorHover() {

	_prev->cursorHover();
	_next->cursorHover();

	for (auto& tool : _tools) {
		tool->cursorHover();
	}
}

void ToolsBuilding::handleEvent(const sf::Event& event) {
	_prev->handleEvent(event);
	_next->handleEvent(event);

	for (auto& tool : _tools) {
		tool->handleEvent(event);
	}
}

void ToolsBuilding::update() {

	_prev->update();
	_next->update();

	for (auto& tool : _tools) {
		tool->update();
	}
}

void ToolsBuilding::draw() {

	int border = 2;
	sf::RectangleShape background(sf::Vector2f(_rect.size.x - 2 * border, _rect.size.y - 2 * border));
	background.setPosition(sf::Vector2f(_rect.position.x + border, _rect.position.y + border));
	background.setFillColor(sf::Color(31, 31, 31));
	background.setOutlineThickness(2);
	background.setOutlineColor(sf::Color(63, 63, 63));
	Main::render_window->draw(background);

	Main::render_window->draw(*_text);

	_prev->draw();
	_next->draw();

	for (auto& tool : _tools) {
		tool->draw();
	}
}