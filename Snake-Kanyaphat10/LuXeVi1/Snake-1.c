#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define COLS  20
#define ROWS  10
#define MAX_SNAKE_SIZE 1000

struct SnakeGame {
  int x[MAX_SNAKE_SIZE];
  int y[MAX_SNAKE_SIZE];
  int quit;         // Add this variable for quit
  int head;         // Add this variable for snake head
  int tail;         // Add this variable for snake tail
  int xdir;         // Add this variable for snake x direction
  int ydir;         // Add this variable for snake y direction
  int applex;       // Add this variable for apple x position
  int appley;       // Add this variable for apple y position
  int gameover;     // Add this variable for game over
  int heartsEaten;  // Add this variable for hearts eaten
  int snakeLength;  // Add this variable for snake length
  int speed;        // Add this variable for snake speed
};

void hideCursor() {
  printf("\e[?25l");
}

void showCursor() {
  printf("\e[?25h");
}

void switchToCanonicalMode() {
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void renderGameBoard() {
  printf("┌");
  for (int i = 0; i < COLS; i++)
    printf("─");
  printf("┐\n");

  for (int j = 0; j < ROWS; j++) {
    printf("│");
    for (int i = 0; i < COLS; i++)
      printf("·");
    printf("│\n");
  }

  printf("└");
  for (int i = 0; i < COLS; i++)
    printf("─");
  printf("┘\n");
}

void drawApple(struct SnakeGame* game) {
  printf("\e[%iB\e[%iC❤", game->appley + 1, game->applex + 1);
  printf("\e[%iF", game->appley + 1);
}

void clearSnakeTail(struct SnakeGame* game) {
  printf("\e[%iB\e[%iC·", game->y[game->tail] + 1, game->x[game->tail] + 1);
  printf("\e[%iF", game->y[game->tail] + 1);
}

void drawSnakeHead(struct SnakeGame* game) {
  printf("\e[%iB\e[%iC▓", game->y[game->head] + 1, game->x[game->head] + 1);
  printf("\e[%iF", game->y[game->head] + 1);
  fflush(stdout);
}

void gameOverMessage(struct SnakeGame* game) {
  printf("\e[%iB\e[%iC Game Over! ", ROWS / 2, COLS / 2 - 5);
  printf("\e[%iB\e[%iC Hearts Eaten: %d", ROWS + 1, COLS / 2 - 5, game->heartsEaten);
  printf("\e[%iF", ROWS + 1);
  printf("\e[%iF", ROWS / 2);
  fflush(stdout);
  getchar();
}

void initializeGame(struct SnakeGame* game) {
  hideCursor();
  switchToCanonicalMode();

  game->quit = 0;
  game->head = 0;
  game->tail = 0;
  game->x[game->head] = COLS / 2;
  game->y[game->head] = ROWS / 2;
  game->gameover = 0;
  game->xdir = 1;
  game->ydir = 0;
  game->applex = -1;
  game->heartsEaten = 0;
  (*game).snakeLength = 5;
  (*game).speed = 100000;
}

void cleanupGame(struct SnakeGame* game) {
  showCursor();
  struct termios oldt;
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void createApple(struct SnakeGame* game) {
  game->applex = rand() % COLS;
  game->appley = rand() % ROWS;

  for (int i = game->tail; i != game->head; i = (i + 1) % MAX_SNAKE_SIZE)
    if (game->x[i] == game->applex && game->y[i] == game->appley)
      game->applex = -1;

  if (game->applex >= 0) {
    drawApple(game);
    printf("\e[%iF", game->appley + 1);
  }
}

void moveSnake(struct SnakeGame* game) {
  int newhead = (game->head + 1) % MAX_SNAKE_SIZE;
  game->x[newhead] = (game->x[game->head] + game->xdir + COLS) % COLS;
  game->y[newhead] = (game->y[game->head] + game->ydir + ROWS) % ROWS;
  game->head = newhead;

  for (int i = game->tail; i != game->head; i = (i + 1) % MAX_SNAKE_SIZE)
    if (game->x[i] == game->x[game->head] && game->y[i] == game->y[game->head])
      game->gameover = 1;
}

void handleInput(struct SnakeGame* game) {
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;

  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

  if (FD_ISSET(STDIN_FILENO, &fds)) {
    int ch = getchar();
    if (ch == 27 || ch == 'q') {
      game->quit = 1;
    } else if (ch == 'a' && game->xdir != 1) {
      game->xdir = -1;
      game->ydir = 0;
    } else if (ch == 'd' && game->xdir != -1) {
      game->xdir = 1;
      game->ydir = 0;
    } else if (ch == 'w' && game->ydir != 1) {
      game->xdir = 0;
      game->ydir = -1;
    } else if (ch == 's' && game->ydir != -1) {
      game->xdir = 0;
      game->ydir = 1;
    }
  }
}

void checkCollision(struct SnakeGame* game) {
  if (game->applex >= 0 && game->x[game->head] == game->applex && game->y[game->head] == game->appley) {
    game->heartsEaten++;
    createApple(game);
  } else {
    clearSnakeTail(game);
    game->tail = (game->tail + 1) % MAX_SNAKE_SIZE;
  }
}

void runGame(struct SnakeGame* game) {
  createApple(game);

  while (!game->quit && !game->gameover) {
    handleInput(game);

    if (!game->quit) {
      moveSnake(game);
      checkCollision(game);
      drawSnakeHead(game);
      usleep(100000);
    }
  }

  if (game->gameover) {
    gameOverMessage(game);
  }
}

void runEasyGame() {
  struct SnakeGame game;
  initializeGame(&game);
  renderGameBoard();
  runGame(&game);
  cleanupGame(&game);
}

void runMediumGame() {
  struct SnakeGame game;
  initializeGame(&game);
  // customize settings for Medium difficulty
  game.snakeLength = 5;
  game.speed = 150000;
  renderGameBoard();
  runGame(&game);
  cleanupGame(&game);
}

void runHardGame() {
  struct SnakeGame game;
  initializeGame(&game);
  // customize settings for Hard difficulty
  game.snakeLength = 8;
  game.speed = 200000;
  renderGameBoard();
  runGame(&game);
  cleanupGame(&game);
}


void runSnakeGame() {
  struct SnakeGame game;

  int option;
  printf("Choose difficulty level:\n1. Easy\n2. Medium\n3. Hard\n");
  scanf("%d", &option);

  switch (option) {
    case 1:
      runEasyGame();
      break;
    case 2:
      runMediumGame();
      break;
    case 3:
      runHardGame();
      break;
    default:
      printf("Invalid option\n");
  }
}

int main() {
  runSnakeGame();
  return 0;
}
