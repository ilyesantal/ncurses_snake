#include "settings_menu.h"
#include "globals.h"
#include <ncurses.h>
#define WIDTH 30
#define HEIGHT 13

enum set {
  DEFAULT,
  START_LEN,
  HEAD_SYMBOL,
  BODY_SYMBOL,
  APPLE_SYMBOL,
  GAME_WIDTH,
  GAME_HEIGHT,
  NORMAL_INTERVAL,
  TURBO_INTERVAL,
  BACK
};

char *settings[] = {"Start length",    "Head symbol",    "Body symbol",
                    "Apple symbol",    "Game width",     "Game height",
                    "Normal interval", "Turbo interval", "Back"};

int n_settings = sizeof(settings) / sizeof(char *);

void print_settings_menu(WINDOW *settings_menu, int highlight);
void process_selected(enum set s);

void settings_menu() {
  int c;
  int highlight = 1;
  int startx;
  int starty;
  WINDOW *settings_window;
  initscr();
  clear();
  noecho();
  cbreak();
  curs_set(0);
  startx = (80 - WIDTH) / 2;
  starty = (24 - HEIGHT) / 2;
  settings_window = newwin(HEIGHT, WIDTH, starty, startx);
  keypad(settings_window, TRUE);
  refresh();
  print_settings_menu(settings_window, highlight);

  while (1) {
    c = wgetch(settings_window);
    switch (c) {
    case KEY_UP:
      if (highlight == 1)
        highlight = n_settings;
      else
        --highlight;
      break;
    case KEY_DOWN:
      if (highlight == n_settings)
        highlight = 1;
      else
        ++highlight;
      break;
    case 10:
      if (highlight == BACK)
        break;
      process_selected(highlight);
      // setting selected
      break;
    default:
      break;
    }
    print_settings_menu(settings_window, highlight);
  }
  clrtoeol();
  refresh();
  curs_set(1);
  endwin();
}

void print_settings_menu(WINDOW *settings_window, int highlight) {
  int x, y, i;
  x = 2;
  y = 2;
  box(settings_window, 0, 0);
  for (i = 0; i < n_settings; ++i) {
    if (highlight == i + 1) /* High light the present choice */
    {
      wattron(settings_window, A_REVERSE);
      mvwprintw(settings_window, y, x, "%s", settings[i]);
      wattroff(settings_window, A_REVERSE);
    } else
      mvwprintw(settings_window, y, x, "%s", settings[i]);
    ++y;
  }
  wrefresh(settings_window);
}

void process_selected(enum set s) {
  switch (s) {
  case START_LEN:
    break;
  }
}
