#pragma once
#include <SFML/Graphics.hpp>

extern std::shared_ptr<sf::Shader> terrain_shader;
extern std::shared_ptr<sf::Shader> grid_shader;
extern std::shared_ptr<sf::Shader> checkerboard_shader;

void loadShaders();