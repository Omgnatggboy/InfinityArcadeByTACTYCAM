#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define BOARD_SIZE 3
#define MAX_NAME_LENGTH 100
#define MAX_HISTORY 100

// Structure to store game history
typedef struct {
    char playerName[MAX_NAME_LENGTH];
    char result[20];
} GameRecord;

// Function prototypes
void playTicTacToe();
void addGameToHistory(const char playerName[], const char result[]);
void searchGameHistory(const char playerName[]);
void summarizeGameHistory();
void viewGameResultsFromCSV();

// Global variables
GameRecord gameHistory[MAX_HISTORY];
int gameCount = 0;

int main() {
    printf("Welcome to Tic Tac Toe!\n");
    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Play Tic Tac Toe\n");
        printf("2. Search for game history by player name\n");
        printf("3. Summarize game history\n");
        printf("4. View game results from CSV\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                playTicTacToe();
                break;
            case 2:
                {
                    char playerName[MAX_NAME_LENGTH];
                    printf("Enter player name to search: ");
                    scanf("%s", playerName);
                    searchGameHistory(playerName);
                }
                break;
            case 3:
                summarizeGameHistory();
                break;
            case 4:
                viewGameResultsFromCSV();
                break;
            case 5:
                printf("End of program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

// Function to play Tic Tac Toe
void playTicTacToe() {
    char playerName[MAX_NAME_LENGTH];
    char board[BOARD_SIZE][BOARD_SIZE];
    bool playerX = true; // true for player X, false for player O

    // Initialize the board
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = ' ';
        }
    }

    printf("Enter your name: ");
    scanf("%s", playerName);

    printf("Welcome, %s! Let's play Tic Tac Toe.\n", playerName);

    // Main game loop
    int moves = 0;
    bool gameEnded = false;

    while (moves < BOARD_SIZE * BOARD_SIZE && !gameEnded) {
        // Display the current board
        printf("\nCurrent board:\n");
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                printf(" %c ", board[i][j]);
                if (j < BOARD_SIZE - 1) {
                    printf("|");
                }
            }
            printf("\n");
            if (i < BOARD_SIZE - 1) {
                printf("-----------\n");
            }
        }

        // Determine the current player
        char currentPlayer = (playerX) ? 'X' : 'O';
        printf("Player %c's turn. Enter row and column (e.g., 1 2): ", currentPlayer);

        // Get the player's move
        int row, col;
        scanf("%d %d", &row, &col);

        // Check if the move is valid
        if (row < 1 || row > BOARD_SIZE || col < 1 || col > BOARD_SIZE || board[row - 1][col - 1] != ' ') {
            printf("Invalid move. Try again.\n");
            continue;
        }

        // Update the board
        board[row - 1][col - 1] = currentPlayer;
        moves++;

        // Check for a win
        bool win = false;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i][0] == currentPlayer && board[i][1] == currentPlayer && board[i][2] == currentPlayer) {
                win = true;
                break;
            }
            if (board[0][i] == currentPlayer && board[1][i] == currentPlayer && board[2][i] == currentPlayer) {
                win = true;
                break;
            }
        }
        if (board[0][0] == currentPlayer && board[1][1] == currentPlayer && board[2][2] == currentPlayer) {
            win = true;
        }
        if (board[0][2] == currentPlayer && board[1][1] == currentPlayer && board[2][0] == currentPlayer) {
            win = true;
        }

        if (win) {
            printf("\nPlayer %c wins!\n", currentPlayer);
            addGameToHistory(playerName, "Win");
            gameEnded = true;
        }

        // Switch players
        playerX = !playerX;
    }

    // If the game is a draw
    if (!gameEnded) {
        printf("\nThe game is a draw!\n");
        addGameToHistory(playerName, "Draw");
    }
}


// Function to add a game to the history
void addGameToHistory(const char playerName[], const char result[]) {
    if (gameCount < MAX_HISTORY) {
        strcpy(gameHistory[gameCount].playerName, playerName);
        strcpy(gameHistory[gameCount].result, result);
        gameCount++;
    }
}

// Function to search for game history by player name
void searchGameHistory(const char playerName[]) {
    printf("Game History for Player: %s\n", playerName);
    printf("===================================\n");
    printf("Name\tResult\n");
    for (int i = 0; i < gameCount; i++) {
        if (strcmp(playerName, gameHistory[i].playerName) == 0) {
            printf("%s\t%s\n", gameHistory[i].playerName, gameHistory[i].result);
        }
    }
}

// Function to summarize game history
void summarizeGameHistory() {
    printf("Game History Summary\n");
    printf("====================\n");
    printf("Total Games Played: %d\n", gameCount);

    int totalWins = 0, totalDraws = 0;
    for (int i = 0; i < gameCount; i++) {
        if (strcmp(gameHistory[i].result, "Win") == 0) {
            totalWins++;
        } else if (strcmp(gameHistory[i].result, "Draw") == 0) {
            totalDraws++;
        }
    }

    printf("Total Wins: %d\n", totalWins);
    printf("Total Draws: %d\n", totalDraws);
}

// Function to view game results from a CSV file
void viewGameResultsFromCSV() {
    printf("This feature is not implemented for Tic Tac Toe.\n");
}
