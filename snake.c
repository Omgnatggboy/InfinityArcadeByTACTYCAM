#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#define COLS  20
#define ROWS 10

struct SnakeGame {
  int x[1000];
  int y[1000];
  int quit;
  int head;
  int tail;
  int xdir;
  int ydir;
  int applex;
  int appley;
  int gameover;
  int heartsEaten;
};

void initializeGame(struct SnakeGame* game) {
  // Hide cursor
  printf("\e[?25l");

  // Switch to canonical mode, disable echo
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  (*game).quit = 0;
  (*game).head = 0;
  (*game).tail = 0;
  (*game).x[(*game).head] = COLS / 2;
  (*game).y[(*game).head] = ROWS / 2;
  (*game).gameover = 0;
  (*game).xdir = 1;
  (*game).ydir = 0;
  (*game).applex = -1;
  (*game).heartsEaten = 0;
}

//EASY
void runSnakeGame(struct SnakeGame* game) {
  while (!(*game).quit) {
    // Render table
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

    // Move cursor back to top
    printf("\e[%iA", ROWS + 2);

    (*game).head = 0;
    (*game).tail = 0;
    (*game).x[(*game).head] = COLS / 2;
    (*game).y[(*game).head] = ROWS / 2;
    (*game).gameover = 0;
    (*game).xdir = 1;
    (*game).ydir = 0;
    (*game).applex = -1;
    (*game).heartsEaten = 0;

    while (!(*game).quit && !(*game).gameover) {
      if ((*game).applex < 0) {
        // Create a new apple
        (*game).applex = rand() % COLS;
        (*game).appley = rand() % ROWS;

        for (int i = (*game).tail; i != (*game).head; i = (i + 1) % 1000)
          if ((*game).x[i] == (*game).applex && (*game).y[i] == (*game).appley)
            (*game).applex = -1;

        if ((*game).applex >= 0) {
          // Draw apple
          printf("\e[%iB\e[%iC❤", (*game).appley + 1, (*game).applex + 1);
          printf("\e[%iF", (*game).appley + 1);
        }
      }

      // Clear snake tail
      printf("\e[%iB\e[%iC·", (*game).y[(*game).tail] + 1, (*game).x[(*game).tail] + 1);
      printf("\e[%iF", (*game).y[(*game).tail] + 1);

       if ((*game).x[(*game).head] == (*game).applex && (*game).y[(*game).head] == (*game).appley) {
      (*game).applex = -1;
      (*game).heartsEaten++; // เพิ่ม: เพิ่มจำนวนแอปเปิลที่กิน
      printf("\a"); // แจ้งเตือนด้วยเสียง Bell
    } else
      (*game).tail = ((*game).tail + 1) % 1000;

      int newhead = ((*game).head + 1) % 1000;
      (*game).x[newhead] = ((*game).x[(*game).head] + (*game).xdir + COLS) % COLS;
      (*game).y[newhead] = ((*game).y[(*game).head] + (*game).ydir + ROWS) % ROWS;
      (*game).head = newhead;

      for (int i = (*game).tail; i != (*game).head; i = (i + 1) % 1000)
        if ((*game).x[i] == (*game).x[(*game).head] && (*game).y[i] == (*game).y[(*game).head])
          (*game).gameover = 1;

      // Draw head
      printf("\e[%iB\e[%iC▓", (*game).y[(*game).head] + 1, (*game).x[(*game).head] + 1);
      printf("\e[%iF", (*game).y[(*game).head] + 1);
      fflush(stdout);

      usleep(10 * 1000000 / 60);

      // Read keyboard
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
          (*game).quit = 1;
        } else if (ch == 'a' && (*game).xdir != 1) {
          (*game).xdir = -1;
          (*game).ydir = 0;
        } else if (ch == 'd' && (*game).xdir != -1) {
          (*game).xdir = 1;
          (*game).ydir = 0;
        } else if (ch == 's' && (*game).ydir != -1) {
          (*game).xdir = 0;
          (*game).ydir = 1;
        } else if (ch == 'w' && (*game).ydir != 1) {
          (*game).xdir = 0;
          (*game).ydir = -1;
        }
      }
    }

    if (!(*game).quit) {
      // Show game over
      printf("\e[%iB\e[%iC Game Over! ", ROWS / 2, COLS / 2 - 5);
      printf("\e[%iB\e[%iC Hearts Eaten: %d", ROWS + 1, COLS / 2 - 5, (*game).heartsEaten);
      printf("\e[%iF", ROWS / 2);
      fflush(stdout);
      getchar();
    }
  }
}

//NORMAL
void runSnakeGameNormal(struct SnakeGame* game) {
  while (!(*game).quit) {
    // Render table
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

    // Move cursor back to top
    printf("\e[%iA", ROWS + 2);

    (*game).head = 0;
    (*game).tail = 0;
    (*game).x[(*game).head] = COLS / 2;
    (*game).y[(*game).head] = ROWS / 2;
    (*game).gameover = 0;
    (*game).xdir = 1;
    (*game).ydir = 0;
    (*game).applex = -1;
    (*game).heartsEaten = 0;

    while (!(*game).quit && !(*game).gameover) {
      if ((*game).applex < 0) {
        // Create a new apple
        (*game).applex = rand() % COLS;
        (*game).appley = rand() % ROWS;

        for (int i = (*game).tail; i != (*game).head; i = (i + 1) % 1000)
          if ((*game).x[i] == (*game).applex && (*game).y[i] == (*game).appley)
            (*game).applex = -1;

        if ((*game).applex >= 0) {
          // Draw apple
          printf("\e[%iB\e[%iC❤", (*game).appley + 1, (*game).applex + 1);
          printf("\e[%iF", (*game).appley + 1);
        }
      }

      // Clear snake tail
      printf("\e[%iB\e[%iC·", (*game).y[(*game).tail] + 1, (*game).x[(*game).tail] + 1);
      printf("\e[%iF", (*game).y[(*game).tail] + 1);

      if ((*game).x[(*game).head] == (*game).applex && (*game).y[(*game).head] == (*game).appley) {
      (*game).applex = -1;
      (*game).heartsEaten++; // เพิ่ม: เพิ่มจำนวนแอปเปิลที่กิน
      printf("\a"); // แจ้งเตือนด้วยเสียง Bell
    } else
      (*game).tail = ((*game).tail + 1) % 1000;

      int newhead = ((*game).head + 1) % 1000;
      (*game).x[newhead] = ((*game).x[(*game).head] + (*game).xdir + COLS) % COLS;
      (*game).y[newhead] = ((*game).y[(*game).head] + (*game).ydir + ROWS) % ROWS;
      (*game).head = newhead;

      for (int i = (*game).tail; i != (*game).head; i = (i + 1) % 1000)
        if ((*game).x[i] == (*game).x[(*game).head] && (*game).y[i] == (*game).y[(*game).head])
          (*game).gameover = 1;

      // Draw head
      printf("\e[%iB\e[%iC▓", (*game).y[(*game).head] + 1, (*game).x[(*game).head] + 1);
      printf("\e[%iF", (*game).y[(*game).head] + 1);
      fflush(stdout);

      usleep(8 * 1000000 / 60);

      // Read keyboard
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
          (*game).quit = 1;
        } else if (ch == 'a' && (*game).xdir != 1) {
          (*game).xdir = -1;
          (*game).ydir = 0;
        } else if (ch == 'd' && (*game).xdir != -1) {
          (*game).xdir = 1;
          (*game).ydir = 0;
        } else if (ch == 's' && (*game).ydir != -1) {
          (*game).xdir = 0;
          (*game).ydir = 1;
        } else if (ch == 'w' && (*game).ydir != 1) {
          (*game).xdir = 0;
          (*game).ydir = -1;
        }
      }
    }

    if (!(*game).quit) {
      // Show game over
      printf("\e[%iB\e[%iC Game Over! ", ROWS / 2, COLS / 2 - 5);
      printf("\e[%iB\e[%iC Hearts Eaten: %d", ROWS + 1, COLS / 2 - 5, (*game).heartsEaten);
      printf("\e[%iF", ROWS + 1);
      printf("\e[%iF", ROWS / 2);
      fflush(stdout);
      getchar();
    }
  }
}

//HARD
void runSnakeGameHard(struct SnakeGame* game) {
  while (!(*game).quit) {
    // Render table
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

    // Move cursor back to top
    printf("\e[%iA", ROWS + 2);

    (*game).head = 0;
    (*game).tail = 0;
    (*game).x[(*game).head] = COLS / 2;
    (*game).y[(*game).head] = ROWS / 2;
    (*game).gameover = 0;
    (*game).xdir = 1;
    (*game).ydir = 0;
    (*game).applex = -1;
    (*game).heartsEaten = 0;

    while (!(*game).quit && !(*game).gameover) {
      if ((*game).applex < 0) {
        // Create a new apple
        (*game).applex = rand() % COLS;
        (*game).appley = rand() % ROWS;

        for (int i = (*game).tail; i != (*game).head; i = (i + 1) % 1000)
          if ((*game).x[i] == (*game).applex && (*game).y[i] == (*game).appley)
            (*game).applex = -1;

        if ((*game).applex >= 0) {
          // Draw apple
          printf("\e[%iB\e[%iC❤", (*game).appley + 1, (*game).applex + 1);
          printf("\e[%iF", (*game).appley + 1);
        }
      }

      // Clear snake tail
      printf("\e[%iB\e[%iC·", (*game).y[(*game).tail] + 1, (*game).x[(*game).tail] + 1);
      printf("\e[%iF", (*game).y[(*game).tail] + 1);

      if ((*game).x[(*game).head] == (*game).applex && (*game).y[(*game).head] == (*game).appley) {
      (*game).applex = -1;
      (*game).heartsEaten++; // เพิ่ม: เพิ่มจำนวนแอปเปิลที่กิน
      printf("\a"); // แจ้งเตือนด้วยเสียง Bell
    } else
      (*game).tail = ((*game).tail + 1) % 1000;

      int newhead = ((*game).head + 1) % 1000;
      (*game).x[newhead] = ((*game).x[(*game).head] + (*game).xdir + COLS) % COLS;
      (*game).y[newhead] = ((*game).y[(*game).head] + (*game).ydir + ROWS) % ROWS;
      (*game).head = newhead;

      for (int i = (*game).tail; i != (*game).head; i = (i + 1) % 1000)
        if ((*game).x[i] == (*game).x[(*game).head] && (*game).y[i] == (*game).y[(*game).head])
          (*game).gameover = 1;

      // Draw head
      printf("\e[%iB\e[%iC▓", (*game).y[(*game).head] + 1, (*game).x[(*game).head] + 1);
      printf("\e[%iF", (*game).y[(*game).head] + 1);
      fflush(stdout);

      usleep(5 * 1000000 / 60);

      // Read keyboard
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
          (*game).quit = 1;
        } else if (ch == 'a' && (*game).xdir != 1) {
          (*game).xdir = -1;
          (*game).ydir = 0;
        } else if (ch == 'd' && (*game).xdir != -1) {
          (*game).xdir = 1;
          (*game).ydir = 0;
        } else if (ch == 's' && (*game).ydir != -1) {
          (*game).xdir = 0;
          (*game).ydir = 1;
        } else if (ch == 'w' && (*game).ydir != 1) {
          (*game).xdir = 0;
          (*game).ydir = -1;
        }
      }
    }

    if (!(*game).quit) {
      // Show game over
      printf("\e[%iB\e[%iC Game Over! ", ROWS / 2, COLS / 2 - 5);
      printf("\e[%iB\e[%iC Hearts Eaten: %d", ROWS + 1, COLS / 2 - 5, (*game).heartsEaten);
      printf("\e[%iF", ROWS + 1);
      printf("\e[%iF", ROWS / 2);
      fflush(stdout);
      getchar();
    }
  }
}

//HARD
void runSnakeGameGod(struct SnakeGame* game) {
  while (!(*game).quit) {
    // Render table
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

    // Move cursor back to top
    printf("\e[%iA", ROWS + 2);

    (*game).head = 0;
    (*game).tail = 0;
    (*game).x[(*game).head] = COLS / 2;
    (*game).y[(*game).head] = ROWS / 2;
    (*game).gameover = 0;
    (*game).xdir = 1;
    (*game).ydir = 0;
    (*game).applex = -1;

    while (!(*game).quit && !(*game).gameover) {
      if ((*game).applex < 0) {
        // Create a new apple
        (*game).applex = rand() % COLS;
        (*game).appley = rand() % ROWS;

        for (int i = (*game).tail; i != (*game).head; i = (i + 1) % 1000)
          if ((*game).x[i] == (*game).applex && (*game).y[i] == (*game).appley)
            (*game).applex = -1;

        if ((*game).applex >= 0) {
          // Draw apple
          printf("\e[%iB\e[%iC❤", (*game).appley + 1, (*game).applex + 1);
          printf("\e[%iF", (*game).appley + 1);
        }
      }

      // Clear snake tail
      printf("\e[%iB\e[%iC·", (*game).y[(*game).tail] + 1, (*game).x[(*game).tail] + 1);
      printf("\e[%iF", (*game).y[(*game).tail] + 1);

      if ((*game).x[(*game).head] == (*game).applex && (*game).y[(*game).head] == (*game).appley) {
        (*game).applex = -1;
        (*game).heartsEaten++;
        printf("\a"); // Bell
      } else
        (*game).tail = ((*game).tail + 1) % 1000;

      int newhead = ((*game).head + 1) % 1000;
      (*game).x[newhead] = ((*game).x[(*game).head] + (*game).xdir + COLS) % COLS;
      (*game).y[newhead] = ((*game).y[(*game).head] + (*game).ydir + ROWS) % ROWS;
      (*game).head = newhead;

      for (int i = (*game).tail; i != (*game).head; i = (i + 1) % 1000)
        if ((*game).x[i] == (*game).x[(*game).head] && (*game).y[i] == (*game).y[(*game).head])
          (*game).gameover = 1;

      // Draw head
      printf("\e[%iB\e[%iC▓", (*game).y[(*game).head] + 1, (*game).x[(*game).head] + 1);
      printf("\e[%iF", (*game).y[(*game).head] + 1);
      fflush(stdout);

      usleep(3 * 1000000 / 60);

      // Read keyboard
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
          (*game).quit = 1;
        } else if (ch == 'a' && (*game).xdir != 1) {
          (*game).xdir = -1;
          (*game).ydir = 0;
        } else if (ch == 'd' && (*game).xdir != -1) {
          (*game).xdir = 1;
          (*game).ydir = 0;
        } else if (ch == 's' && (*game).ydir != -1) {
          (*game).xdir = 0;
          (*game).ydir = 1;
        } else if (ch == 'w' && (*game).ydir != 1) {
          (*game).xdir = 0;
          (*game).ydir = -1;
        }
      }
    }

    if (!(*game).quit) {
      // Show game over
      printf("\e[%iB\e[%iC Game Over! ", ROWS / 2, COLS / 2 - 5);
      printf("\e[%iB\e[%iC Hearts Eaten: %d", ROWS + 1, COLS / 2 - 5, (*game).heartsEaten);
      printf("\e[%iF", ROWS + 1);
      printf("\e[%iF", ROWS / 2);
      fflush(stdout);
      getchar();
    }
  } 
}
void cleanupGame(struct SnakeGame* game) {
  // Show cursor
  printf("\e[?25h");

  struct termios oldt;
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int main(){
    int option;
    while (option!=6)
    {
        printf("Select your difficulty from obtions below");
        printf("\n1. Easy");
        printf("\n2. Normal");
        printf("\n3. Hard");
        printf("\n4. God");
        printf("\n5. Exit game");
        printf("\n Enter option no. : ");
        scanf("%d", &option);
        if(option == 1)
        {
            struct SnakeGame game;
            initializeGame(&game);
            runSnakeGame(&game);
            cleanupGame(&game);
            return 0;
        }
        else if(option == 2)
        {
            struct SnakeGame game;
            initializeGame(&game);
            runSnakeGameNormal(&game);
            cleanupGame(&game);
            return 0;
        }
        else if(option == 3)
        {
            struct SnakeGame game;
            initializeGame(&game);
            runSnakeGameHard(&game);
            cleanupGame(&game);
            return 0;
        }
        else if(option == 4)
        {
            struct SnakeGame game;
            initializeGame(&game);
            runSnakeGameGod(&game);
            cleanupGame(&game);
            return 0;
        }
        else if(option == 5){
            printf("Thank you for your attention.\n");
            return 0;
        }
        else{
          printf("Please enter 1-5 option.\n");
        }
    }

}