#include "ShaderManager.hpp"

std::shared_ptr<sf::Shader> terrain_shader;
std::shared_ptr<sf::Shader> grid_shader;
std::shared_ptr<sf::Shader> checkerboard_shader;

void loadShaders() {
	terrain_shader = std::make_shared<sf::Shader>(L"assets\\shr\\vertex.vert", L"assets\\shr\\water.frag");
	grid_shader = std::make_shared<sf::Shader>(L"assets\\shr\\vertex.vert", L"assets\\shr\\grid.frag");
	checkerboard_shader = std::make_shared<sf::Shader>(L"assets\\shr\\vertex.vert", L"assets\\shr\\water.frag");
}