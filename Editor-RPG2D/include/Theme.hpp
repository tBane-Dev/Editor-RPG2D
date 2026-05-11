#pragma once
#include "SFML/Graphics.hpp"

extern sf::Font basicFont;

// text
extern int basic_text_size;
extern sf::Color basic_text_color;
extern int basic_text_rect_height;

// main menu
extern int menu_height;
extern int menu_horizontal_margin;
extern int menu_padding;
extern int menu_font_size;
extern sf::Color menu_bar_color;
extern sf::Color menu_text_color;
extern sf::Color menu_text_inactive_color;
extern sf::Color menubox_open_color;
extern sf::Color menubox_idle_color;
extern sf::Color menubox_hover_color;
extern sf::Color menubox_press_color;

extern int optionbox_height;
extern int optionbox_font_size;
extern int optionbox_left_margin;
extern int optionbox_right_margin;
extern int optionbox_spacing;
extern int optionbox_border_width;
extern sf::Color optionbox_border_color;
extern sf::Color optionbox_select_color;
extern sf::Color optionbox_idle_color;
extern sf::Color optionbox_hover_color;
extern sf::Color optionbox_press_color;

void loadTheme();