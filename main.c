#include "about_menu.h"
#include "game_window.h"
#include "main_menu.h"

int main() {
  enum main_menu_choice main_choice;
  main_choice = run_main_menu();
  while (1) {
    switch (main_choice) {
    case NEW_GAME:
      run_game();
      break;
    case CONTINUE:
      break;
    case OPTIONS:
      break;
    case ABOUT:
      print_about();
      main_choice = run_main_menu();
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
