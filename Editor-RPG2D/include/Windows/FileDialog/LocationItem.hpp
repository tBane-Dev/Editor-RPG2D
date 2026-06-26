#pragma once
#include <SFML/Graphics.hpp>
#include "GUIManager.hpp"
#include <functional>
#include "Controls/Button.hpp"
#include "Controls/Checkbox.hpp"
#include "Windows/FileDialog/Location.hpp"

enum class LocationItemType { Empty, PC, Drive, Directory };

class LocationItem : public Button {
public:
	std::wstring _name;
	std::shared_ptr<Location> _location;

	LocationItemType _type;
	std::shared_ptr<Checkbox> _opener;
	std::unique_ptr<sf::Text> _text;

	LocationItem();
	~LocationItem();

	void setSize(sf::Vector2i size);
	void setPosition(sf::Vector2i position);
	void setFile(std::wstring name, std::shared_ptr<Location> location);

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};