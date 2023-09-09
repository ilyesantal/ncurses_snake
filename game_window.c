#include "game_window.h"
#include "globals.h"
#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>

enum direction { UP, DOWN, LEFT, RIGHT };

struct snake_part {
  int x;
  int y;
  struct snake_part *next;
};

struct apple {
  int x;
  int y;
};

struct snake_part *init_snake(int length);
struct apple new_apple();
void print_game(WINDOW *game_window, struct snake_part *head, struct apple a);
void move_part(struct snake_part *part, enum direction d);
void move_snake(struct snake_part *head, enum direction d);
void free_snake(struct snake_part *head);
void grow_snake(struct snake_part *head);

void run_game() {
  WINDOW *game_window;
  int c;
  enum direction d = UP;
  struct apple a = new_apple();
  struct snake_part *head = init_snake(init_snake_length);

  initscr();
  clear();
  noecho();
  cbreak();
  curs_set(0);

  game_window = newwin(game_height, game_width, 0, 0);
  keypad(game_window, true);

  refresh();
  print_game(game_window, head, a);

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
    case 'x':
      goto end;
    default:
      break;
    }
    move_snake(head, d);
    print_game(game_window, head, a);
    if (head->x == a.x && head->y == a.y) {
      grow_snake(head);
      a = new_apple();
    }
  }
end:
  curs_set(1);
  free_snake(head);
}

struct apple new_apple() {
  struct apple a;

  a.x = (rand() % (game_width - 2)) + 1;
  a.y = (rand() % (game_height - 2)) + 1;

  return a;
}

struct snake_part *init_snake(int length) {
  struct snake_part *head;
  struct snake_part *prev = NULL;
  for (int i = 0; i < length; i++) {
    struct snake_part *part = malloc(sizeof(struct snake_part));
    part->x = game_width / 2;
    part->y = game_height / 2 + i;
    if (i == 0) {
      head = part;
    } else {
      prev->next = part;
    }
    prev = part;
  }

  return head;
}

void print_game(WINDOW *game_window, struct snake_part *head, struct apple a) {
  wclear(game_window);
  box(game_window, 0, 0);
  mvwprintw(game_window, a.y, a.x, "%c", apple_ch);
  struct snake_part *current = head;
  mvwprintw(game_window, head->y, head->x, "%c", snake_head_ch);
  while ((current = current->next) != NULL) {
    mvwprintw(game_window, current->y, current->x, "%c", snake_body_ch);
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

void free_snake(struct snake_part *head) {
  struct snake_part *current = head;
  struct snake_part *next;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
}

void grow_snake(struct snake_part *head) {
  struct snake_part *p = malloc(sizeof(struct snake_part));
  while (head->next != NULL) {
    head = head->next;
  }
  p->x = 0;
  p->y = 0;
  head->next = p;
  p->next = NULL;
}
