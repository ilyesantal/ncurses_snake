#include "game_window.h"
#include "globals.h"
#include <curses.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define TERMW 80
#define TERMH 24

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
void print_game(struct snake_part *head, struct apple a);
void move_part(struct snake_part *part, enum direction d);
void move_snake(struct snake_part *head, enum direction d);
void free_snake(struct snake_part *head);
void grow_snake(struct snake_part *head);
void eat_apple(struct snake_part head, struct apple *a);
int check_walls(struct snake_part head);
int check_snake(struct snake_part head, int x, int y);
int check_snake_apple(struct snake_part head, struct apple a);
int check_snake_self(struct snake_part head);
void print_game_over();
void *get_direction();

static enum direction d;
static WINDOW *game_window;
static char exit_flag;
static char turbo_flag;
static int points;

void run_game() {
  d = UP;
  exit_flag = 0;
  points = 0;
  turbo_flag = 0;
  struct snake_part *head = init_snake(init_snake_length);
  struct apple a = new_apple();
  while (!check_snake_apple(*head, a)) {
    a = new_apple();
  }

  int startx = (TERMW - game_width) / 2;
  int starty = (TERMH - game_height) / 2;

  initscr();
  clear();
  noecho();
  cbreak();
  curs_set(0);

  game_window = newwin(game_height, game_width, starty, startx);
  keypad(game_window, true);

  refresh();
  print_game(head, a);

  pthread_t id;
  pthread_create(&id, NULL, get_direction, NULL);

  while (1) {
    if (exit_flag) {
      break;
    }
    usleep(turbo_flag ? (turbo_interval_msec * 1000)
                      : (game_interval_msec * 1000));
    move_snake(head, d);
    print_game(head, a);
    eat_apple(*head, &a);
    if (!check_walls(*head)) {
      print_game_over();
      break;
    }
    if (!check_snake_self(*head)) {
      print_game_over();
      break;
    }
  }
  curs_set(1);
  free_snake(head);
}

void *get_direction() {
  int c;
  while (1) {
    c = wgetch(game_window);
    switch (c) {
    case KEY_UP:
      turbo_flag = 0;
      if (d == DOWN)
        break;
      d = UP;
      break;
    case KEY_DOWN:
      turbo_flag = 0;
      if (d == UP)
        break;
      d = DOWN;
      break;
    case KEY_LEFT:
      turbo_flag = 0;
      if (d == RIGHT)
        break;
      d = LEFT;
      break;
    case KEY_RIGHT:
      turbo_flag = 0;
      if (d == LEFT)
        break;
      d = RIGHT;
      break;
    case ' ':
      turbo_flag = 1;
      break;
    case 'x':
      exit_flag = 1;
      pthread_exit(NULL);
      break;
    default:
      break;
    }
  }
}

struct apple new_apple() {
  struct apple a;

  a.x = (rand() % (game_width - 2)) + 1;
  a.y = (rand() % (game_height - 2)) + 1;

  return a;
}

void eat_apple(struct snake_part head, struct apple *a) {
  if (head.x == a->x && head.y == a->y) {
    grow_snake(&head);
    points++;
    *a = new_apple();
  }
}

int check_walls(struct snake_part head) {
  if (head.x == 0 || head.x == game_width - 1 || head.y == 0 ||
      head.y == game_height - 1) {
    return 0;
  }
  return 1;
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

void print_game(struct snake_part *head, struct apple a) {
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

int check_snake(struct snake_part head, int x, int y) {
  struct snake_part *ptr = &head;
  do {
    if (ptr->x == x && ptr->y == y) {
      return 0;
    }
    ptr = ptr->next;
  } while (ptr != NULL);
  return 1;
}

int check_snake_self(struct snake_part head) {
  return check_snake(*head.next, head.x, head.y);
}

int check_snake_apple(struct snake_part head, struct apple a) {
  return check_snake(head, a.x, a.y);
}

void print_game_over() {
  int startx = (TERMW - 13) / 2;
  int starty = (TERMH - 5) / 2;
  WINDOW *gameover = newwin(5, 13, starty, startx);
  keypad(gameover, true);
  refresh();
  wclear(gameover);
  box(gameover, 0, 0);
  mvwprintw(gameover, 2, 2, "GAME OVER");
  wrefresh(gameover);
  wgetch(gameover);
}
