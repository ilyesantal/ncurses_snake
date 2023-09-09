#include "game_window.h"
#include <curses.h>
#include <ncurses.h>

#define WIDTH 100
#define HEIGHT 50

enum direction { UP, DOWN, LEFT, RIGHT };

struct snake_part {
  int x;
  int y;
  struct snake_part *next;
};

void print_game(WINDOW *game_window, struct snake_part *head);
void move_part(struct snake_part *part, enum direction d);
void move_snake(struct snake_part *head, enum direction d);

void run_game() {
  WINDOW *game_window;
  int c;
  struct snake_part head;
  head.x = WIDTH / 2;
  head.y = HEIGHT / 2;
  head.next = NULL;
  enum direction d = UP;
  int startx = 0;
  int starty = 0;

  struct snake_part a1;
  a1.x = WIDTH / 2;
  a1.y = HEIGHT / 2 + 1;
  a1.next = NULL;
  head.next = &a1;

  initscr();
  clear();
  noecho();
  cbreak();
  curs_set(0);

  game_window = newwin(HEIGHT, WIDTH, starty, startx);
  keypad(game_window, true);

  refresh();
  print_game(game_window, &head);

  while (1) {
    c = wgetch(game_window);
    switch (c) {
    case KEY_UP:
      if (d == DOWN)
        break;
      d = UP;
      break;
    case KEY_DOWN:
      if (d == UP)
        break;
      d = DOWN;
      break;
    case KEY_LEFT:
      if (d == RIGHT)
        break;
      d = LEFT;
      break;
    case KEY_RIGHT:
      if (d == LEFT)
        break;
      d = RIGHT;
      break;
    default:
      break;
    }
    move_snake(&head, d);
    print_game(game_window, &head);
  }
  curs_set(1);
}

void print_game(WINDOW *game_window, struct snake_part *head) {
  wclear(game_window);
  box(game_window, 0, 0);
  struct snake_part *current = head;
  mvwprintw(game_window, head->y, head->x, "+");
  while ((current = current->next) != NULL) {
    mvwprintw(game_window, current->y, current->x, ".");
  }
  wrefresh(game_window);
}

void move_part(struct snake_part *part, enum direction d) {
  switch (d) {
  case UP:
    part->y -= 1;
    break;
  case DOWN:
    part->y += 1;
    break;
  case RIGHT:
    part->x += 1;
    break;
  case LEFT:
    part->x -= 1;
    break;
  }
}

void move_snake(struct snake_part *head, enum direction d) {
  struct snake_part *current = head;
  int prev_x = current->x;
  int prev_y = current->y;
  move_part(head, d);
  while ((current = current->next) != NULL) {
    prev_x += current->x;
    current->x = prev_x - current->x;
    prev_x -= current->x;

    prev_y += current->y;
    current->y = prev_y - current->y;
    prev_y -= current->y;
  }
}
