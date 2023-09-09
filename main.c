#include "about_menu.h"
#include "game_window.h"
#include "globals.h"
#include "main_menu.h"
#include <stdlib.h>
#include <time.h>

int init_snake_length = 5;
char snake_body_ch = '.';
char snake_head_ch = '+';

int main() {
  srand(time(0));
  enum main_menu_choice main_choice;
  main_choice = run_main_menu();
  while (1) {
    switch (main_choice) {
    case NEW_GAME:
      run_game();
      main_choice = DEFAULT;
      break;
    case CONTINUE:
      break;
    case OPTIONS:
      break;
    case ABOUT:
      print_about();
      main_choice = DEFAULT;
      break;
    case EXIT:
      return 0;
    case DEFAULT:
      main_choice = run_main_menu();
      break;
    }
  }
  return 0;
}
