#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define COLS 20
#define ROWS 10
#define MAX_SNAKE_SIZE 1000

// Function to hide the cursor
void hideCursor() {
  printf("\e[?25l");
}

// Function to show the cursor
void showCursor() {
  printf("\e[?25h");
}

// Function to switch to canonical mode
void switchToCanonicalMode() {
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

// Function to render the game board
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

// Function to draw the apple
void drawApple(int applex, int appley) {
  printf("\e[%iB\e[%iC❤", appley + 1, applex + 1);
  printf("\e[%iF", appley + 1);
}

// Function to clear the snake tail
void clearSnakeTail(int tailX, int tailY) {
  printf("\e[%iB\e[%iC·", tailY + 1, tailX + 1);
  printf("\e[%iF", tailY + 1);
}

// Function to draw the snake head
void drawSnakeHead(int headX, int headY) {
  printf("\e[%iB\e[%iC▓", headY + 1, headX + 1);
  printf("\e[%iF", headY + 1);
  fflush(stdout);
}

// Function to display the game over message
void gameOverMessage(int heartsEaten) {
  printf("\e[%iB\e[%iC Game Over! ", ROWS / 2, COLS / 2 - 5);
  printf("\e[%iB\e[%iC Hearts Eaten: %d", ROWS + 1, COLS / 2 - 5, heartsEaten);
  printf("\e[%iF", ROWS + 1);
  printf("\e[%iF", ROWS / 2);
  fflush(stdout);
  getchar();
}

// Function to initialize the game
void initializeGame(struct SnakeGame* game) {
  hideCursor();
  switchToCanonicalMode();

  game->quit = false;
  game->head = 0;
  game->tail = 0;
  game->x[game->head] = COLS / 2;
  game->y[game->head] = ROWS / 2;
  game->gameover = false;
  game->xdir = 1;
  game->ydir = 0;
  game->applex = -1;
  game->heartsEaten = 0;
  game->snakeLength = 5;
  game->speed = 100000;
}

// Function to clean up the game
void cleanupGame(struct SnakeGame* game) {
  showCursor();
  struct termios oldt;
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

// Function to create an apple
void createApple(struct SnakeGame* game) {
  game->applex = rand() % COLS;
  game->appley = rand() % ROWS;

  for (int i = game->tail; i != game->head; i = (i + 1) % MAX_SNAKE_SIZE)
    if (game->x[i] == game->applex && game->y[i] == game->appley)
      game->applex = -1;

  if (game->applex >= 0) {
    drawApple(game->applex, game->appley);
    printf("\e[%iF", game->appley + 1);
  }
}

// Function to move the snake
void moveSnake(struct SnakeGame* game) {
  int newhead = (game->head + 1) % MAX_SNAKE_SIZE;
  game->x[newhead] = (game->x[game->head] + game->xdir + COLS) % COLS;
  game->y[newhead] = (game->y[game->head] + game->ydir + ROWS) % ROWS;
  game->head = newhead;

  for (int i = game->tail; i != game->head; i = (i + 1) % MAX_SNAKE_SIZE)
    if (game->x[i] == game->x[game->head] && game->y[i] == game->y[game->head])
      game->gameover = true;
}

// Function to handle user input
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
      game->quit = true;
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

// Function to check collision
void checkCollision(struct SnakeGame* game) {
  if (game->applex >= 0 && game->x[game->head] == game->applex && game->y[game->head] == game->appley) {
    game->heartsEaten++;
    createApple(game);
  } else {
    clearSnakeTail(game->x[game->tail], game->y[game->tail]);
    game->tail = (game->tail + 1) % MAX_SNAKE_SIZE;
  }
}

// Function to run the game
void runGame(struct SnakeGame* game) {
  createApple(game);

  while (!game->quit && !game->gameover) {
    handleInput(game);

    if (!game->quit) {
      moveSnake(game);
      checkCollision(game);
      drawSnakeHead(game->x[game->head], game->y[game->head]);
      usleep(game->speed);
    }
  }

  if (game->gameover) {
    gameOverMessage(game->heartsEaten);
  }
}

// Function to run an easy game
void runEasyGame() {
  struct SnakeGame game;
  initializeGame(&game);
  renderGameBoard();
  runGame(&game);
  cleanupGame(&game);
}

// Function to run a medium game
void runMediumGame() {
  struct SnakeGame game;
  initializeGame(&game);
  // Customize settings for Medium difficulty
  game.snakeLength = 5;
  game.speed = 150000;
  renderGameBoard();
  runGame(&game);
  cleanupGame(&game);
}

// Function to run a hard game
void runHardGame() {
  struct SnakeGame game;
  initializeGame(&game);
  // Customize settings for Hard difficulty
  game.snakeLength = 8;
  game.speed = 200000;
  renderGameBoard();
  runGame(&game);
  cleanupGame(&game);
}

// Function to run the snake game
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

// Main function
int main() {
  runSnakeGame();
  return 0;
}
