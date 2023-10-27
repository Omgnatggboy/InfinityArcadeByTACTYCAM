#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 3
#define MAX_NAME_LENGTH 50
#define MAX_HISTORY 100

// Structure to store game history
typedef struct {
    char playerName[MAX_NAME_LENGTH];
    char winner[MAX_NAME_LENGTH];
    int roundsPlayed;
    char dateTime[20];
} GameRecord;

char board[BOARD_SIZE][BOARD_SIZE];
GameRecord gameHistory[MAX_HISTORY];
int gameCount = 0;

// Function of TicTacTor
void initializeBoard();
void printBoard();
bool makeMove(char player, int position);
bool checkWin(char player);
bool checkDraw();
void playGame();
void playAgainstComputer();
void addGameToHistory(const char playerName[], const char winner[], int roundsPlayed, const char dateTime[]);
void viewGameResults();
void saveGameHistoryToCSV();
void readGameHistoryFromCSV();
void computerMove(char player);
bool isValidMove(int position);
int getComputerMove(char player);
bool isBoardFull();
char* getCurrentDateTime();

// Function to add a game result to the history
void addGameToHistory(const char playerName[], const char winner[], int roundsPlayed, const char dateTime[]) {
    if (gameCount < MAX_HISTORY) {
        strcpy(gameHistory[gameCount].playerName, playerName);
        strcpy(gameHistory[gameCount].winner, winner);
        gameHistory[gameCount].roundsPlayed = roundsPlayed;
        strcpy(gameHistory[gameCount].dateTime, dateTime);
        gameCount++;
    }
}

// Function to play a game
void playTicTacTorGame() {
    // ... (The rest of your first program's playGame function)
}

// ... (The rest of your first program)

// Second program functions

// Function to play a number guessing game
void playNumberGame() {
    char playerName[MAX_NAME_LENGTH];
    int number, guess, numGuesses = 0;
    int low = 1, high = 100;

    printf("Enter your name: ");
    scanf("%s", playerName);

    number = rand() % 100 + 1;

    printf("Welcome, %s! Guess the number between 1 and 100\n", playerName);

    do {
        printf("Enter your guess: ");
        scanf("%d", &guess);
        numGuesses++;

        if (guess > number) {
            printf("Too high\n");
            high = guess - 1;
        } else if (guess < number) {
            printf("Too low\n");
            low = guess + 1;
        } else {
            printf("Congratulations, %s! You guessed the number in %d guesses!\n", playerName, numGuesses);
            addGameToHistory(playerName, playerName, numGuesses, getCurrentDateTime());
        }
    } while (guess != number);
}

// ... (The rest of your second program functions)

int main() {
    srand(time(NULL));
    printf("Welcome to Infinity Game! by TACTYCAM\n");
    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Play Tic Tac Toe\n");
        printf("2. Play Number Guessing Game\n");
        printf("3. View game results\n");
        printf("4. Save game history to CSV\n");
        printf("5. View game results from CSV\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                // Play Tic Tac Toe
                playTicTacTorGame();
                break;
            case 2:
                // Play Number Guessing Game
                playNumberGame();
                break;
            case 3:
                viewGameResults();
                break;
            case 4:
                saveGameHistoryToCSV();
                break;
            case 5:
                readGameHistoryFromCSV();
                break;
            case 6:
                printf("End of program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}

// ... (The rest of your first program's functions)
