#include "Theme.hpp"
#include "DebugLog.hpp"

sf::Font basicFont;

// text
int basic_text_size;
sf::Color basic_text_color;
int basic_text_rect_height;

int small_text_size;
sf::Color small_text_color;
int small_text_rect_height;


// menu boxes (main menu)
int menu_height;
int menu_horizontal_margin;
int menu_padding;
int menu_font_size;
sf::Color menu_bar_color;
sf::Color menu_text_color;
sf::Color menu_text_inactive_color;
sf::Color menubox_open_color;
sf::Color menubox_idle_color;
sf::Color menubox_hover_color;
sf::Color menubox_press_color;

// option boxes (main menu)
int optionbox_height;
int optionbox_font_size;
int optionbox_left_margin;
int optionbox_right_margin;
int optionbox_spacing;
int optionbox_border_width;
sf::Color optionbox_border_color;
sf::Color optionbox_select_color;
sf::Color optionbox_idle_color;
sf::Color optionbox_hover_color;
sf::Color optionbox_press_color;

// buttons
sf::Color button_inactive_color;
sf::Color button_select_idle_color;
sf::Color button_select_hover_color;
sf::Color button_select_press_color;
sf::Color button_idle_color;
sf::Color button_hover_color;
sf::Color button_press_color;
sf::Color button_text_color;

void loadTheme() {

	if (!basicFont.openFromFile("C:/Windows/Fonts/consola.ttf")) {
		DebugError(L"Failed to load basic font");
		exit(0);
	}

	// text
	basic_text_size = 20;
	basic_text_color = sf::Color(191, 191, 191);
	basic_text_rect_height = 32;

	small_text_size = 16;
	small_text_color = sf::Color(191, 191, 191);
	small_text_rect_height = 24;

	//menu boxes (main menu)
	menu_height = 32;
	menu_horizontal_margin = 8;
	menu_font_size = 20;
	menu_bar_color = sf::Color(15, 15, 15);
	menu_text_color = sf::Color(191, 191, 191);
	menu_text_inactive_color = sf::Color(127, 127, 127);

	menubox_open_color = sf::Color(31, 31, 31);
	menubox_idle_color = sf::Color(15, 15, 15);
	menubox_hover_color = sf::Color(47, 47, 47);
	menubox_press_color = sf::Color(39, 39, 39);

	// option boxes (main menu)
	optionbox_height = 32;
	optionbox_font_size = 20;
	optionbox_left_margin = 2;
	optionbox_right_margin = 2;
	optionbox_spacing = 48;
	optionbox_border_width = 2;
	optionbox_border_color = sf::Color(63, 63, 63);
	optionbox_select_color = sf::Color(79, 31, 31);
	optionbox_idle_color = sf::Color(31, 31, 31);
	optionbox_hover_color = sf::Color(47, 47, 47);
	optionbox_press_color = sf::Color(39, 39, 39);

	// buttons
	button_inactive_color = sf::Color(15, 15, 15);
	button_select_idle_color = sf::Color(79, 31, 31);
	button_select_hover_color = sf::Color(111, 31, 31);
	button_select_press_color = sf::Color(95, 31, 31);
	button_idle_color = sf::Color(23, 23, 23);
	button_hover_color = sf::Color(47, 47, 47);
	button_press_color = sf::Color(39, 39, 39);
	button_text_color = sf::Color(191, 191, 191);
}
