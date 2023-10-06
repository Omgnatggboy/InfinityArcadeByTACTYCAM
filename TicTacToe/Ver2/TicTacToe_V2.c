#include <stdio.h>
#include <stdlib.h>

// Define a structure to represent a game result
struct GameResult {
    int player1Wins;
    int player2Wins;
    int draws;
};

char board[3][3];

void initializeBoard() {
    char cell = '1';
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = cell++;
        }
    }
}

void showBoard() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %c ", board[i][j]);
            if (j < 2) {
                printf("|");
            }
        }
        printf("\n");
        if (i < 2) {
            printf("-----------\n");
        }
    }
    printf("\n");
}

int isBoardFull() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                return 0;
            }
        }
    }
    return 1;
}

int checkWin(char player) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
            return 1;
        }
    }

    // Check diagonals
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return 1;
    }

    return 0;
}

int makeMove(int cell, char player) {
    if (cell < 1 || cell > 9) {
        printf("Invalid input. Please enter a number from 1 to 9.\n");
        return 0;
    }

    int row = (cell - 1) / 3;
    int col = (cell - 1) % 3;

    if (board[row][col] == 'X' || board[row][col] == 'O') {
        printf("Cell already occupied. Choose another cell.\n");
        return 0;
    }

    board[row][col] = player;
    return 1;
}

void saveGameResult(struct GameResult result) {
    FILE *file = fopen("game_results.csv", "a");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    fprintf(file, "%d,%d,%d\n", result.player1Wins, result.player2Wins, result.draws);
    fclose(file);
}

struct GameResult loadGameResults() {
    struct GameResult result = {0, 0, 0};
    FILE *file = fopen("game_results.csv", "r");
    if (file == NULL) {
        printf("No game results found.\n");
        return result;
    }

    int player1Wins, player2Wins, draws;
    while (fscanf(file, "%d,%d,%d", &player1Wins, &player2Wins, &draws) == 3) {
        result.player1Wins += player1Wins;
        result.player2Wins += player2Wins;
        result.draws += draws;
    }

    fclose(file);
    return result;
}

void viewGameResults() {
    struct GameResult result = loadGameResults();
    printf("Game Results:\n");
    printf("Player 1 Wins: %d\n", result.player1Wins);
    printf("Player 2 Wins: %d\n", result.player2Wins);
    printf("Draws: %d\n", result.draws);
}

int main() {
    printf("Welcome to Tic-Tac-Toe!\n");
    printf("--- \tSet up ---\n");
    int currentPlayer = 1; // 1 for Player 1 (X), 2 for Player 2 (O)
    printf("Loading : currentPlayer\n");
    int cell;
    printf("Loading : cell\n");
    int gameResult = 0;
    printf("Loading : gameResult\n");
    int restart = 1;
    printf("Loading : restart\n");
    printf("--- \tDone ---\n");
    printf("Welcome to Tic-Tac-Toe!\n");

    do {
        initializeBoard();
        currentPlayer = 1; // Start with Player 1
        restart = 1; // Reset restart flag

        do {
            showBoard();
            printf("Player %d (%c), enter a cell number (1-9): ", currentPlayer, currentPlayer == 1 ? 'X' : 'O');
            scanf("%d", &cell);

            if (makeMove(cell, currentPlayer == 1 ? 'X' : 'O')) {
                gameResult = checkWin(currentPlayer == 1 ? 'X' : 'O');
                if (gameResult) {
                    showBoard();
                    printf("Player %d wins!\n", currentPlayer);
                    break;
                }

                if (isBoardFull()) {
                    showBoard();
                    printf("It's a draw!\n");
                    break;
                }

                currentPlayer = (currentPlayer == 1) ? 2 : 1; // Switch players
            }
        } while (1);

        // Update game results and ask if the user wants to play again
        struct GameResult result = loadGameResults();
        if (currentPlayer == 1) {
            result.player1Wins++;
        } else {
            result.player2Wins++;
        }
        if (gameResult == 0) {
            result.draws++;
        }
        saveGameResult(result);

        printf("Do you want to play again? (1 for Yes, 0 for No): ");
        scanf("%d", &restart);
    } while (restart);

    // View game results
    viewGameResults();

    printf("Thank you for playing Tic-Tac-Toe!\n");

    return 0;
}
