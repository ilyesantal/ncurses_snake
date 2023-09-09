#include "about_menu.h"
#include <curses.h>

#define WIDTH 30
#define HEIGHT 10

int startx = 0;
int starty = 0;

void print_about() {
  WINDOW *about_win;
  int c;
  int startx = 0;
  int starty = 0;

  initscr();
  clear();
  noecho();
  cbreak();
  startx = (80 - WIDTH) / 2;
  starty = (24 - HEIGHT) / 2;

  about_win = newwin(HEIGHT, WIDTH, starty, startx);
  keypad(about_win, true);

  mvwprintw(about_win, 0, 0, "Snake made by Anti");
  refresh();

  c = wgetch(about_win);
}
