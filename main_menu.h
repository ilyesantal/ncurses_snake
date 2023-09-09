#ifndef MAIN_MENU
#define MAIN_MENU

#include <ncurses.h>
#include <stdio.h>
#include <string.h>

enum main_menu_choice { DEFAULT, CONTINUE, NEW_GAME, OPTIONS, ABOUT, EXIT };

enum main_menu_choice run_main_menu();

#endif // !MAIN_MENU
