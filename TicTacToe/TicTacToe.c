#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define BOARD_SIZE 3
typedef unsigned long long bitboard;

// Bitmasks for each winning combination
const bitboard WIN_MASKS[] = {
    0x000000000000007, // Row 1
    0x000000000000038, // Row 2
    0x0000000000001C0, // Row 3
    0x000000000000049, // Column 1
    0x000000000000092, // Column 2
    0x000000000000124, // Column 3
    0x000000000000111, // Diagonal 1
    0x000000000000054  // Diagonal 2
};

// Function to print the board
void printBoard(const bitboard board) {
    printf("     1   2   3\n");
    printf("   +---+---+---+\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf(" %d ", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++) {
            int index = i * BOARD_SIZE + j;
            char c = (board & (1ULL << index)) ? 'X' : ((board & (1ULL << (index + 9))) ? 'O' : ' ');
            printf("| %c ", c);
        }
        printf("|\n");
        printf("   +---+---+---+\n");
    }
}

// Function to check if a player has won the game
bool checkWin(const bitboard board, const bitboard mask) { return ((board & mask) == mask); }

// Function to check if the game has ended in a draw
bool checkDraw(const bitboard board) { return ((board & 0x1FFULL) == 0x1FFULL); }

// Function to make a move on the board
bool makeMove(bitboard *board, const int row, const int col, const int player) {
    int index = (row - 1) * BOARD_SIZE + (col - 1);
    if ((*board & (1ULL << index)) || (*board & (1ULL << (index + 9)))) {
        return false; // Invalid move
    }
    *board |= (1ULL << (index + (player == 1 ? 0 : 9)));
    return true; // Valid move
}

// Function to save the game state to a CSV file
void saveGame(const bitboard board) {
    FILE *file = fopen("tictactoe.csv", "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int index = i * BOARD_SIZE + j;
            char c = (board & (1ULL << index)) ? 'X' : ((board & (1ULL << (index + 9))) ? 'O' : ' ');
            fprintf(file, "%c,", c);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Game saved to 'tictactoe.csv'.\n");
}

// Function to load the game state from a CSV file
void loadGame(bitboard *board) {
    FILE *file = fopen("tictactoe.csv", "r");
    if (file == NULL) {
        printf("No saved game found.\n");
        return;
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            char c;
            fscanf(file, " %c,", &c);
            int index = i * BOARD_SIZE + j;
            if (c == 'X') {
                *board |= (1ULL << index);
            } else if (c == 'O') {
                *board |= (1ULL << (index + 9));
            }
        }
    }

    fclose(file);
    printf("Game loaded from 'tictactoe.csv'.\n");
}

// Function to ask players if they want to continue playing or end the game
bool askToContinue() {
    char input[10];
    printf("Do you want to continue playing? (y/n): ");
    scanf("%s", input);
    return (input[0] == 'y' || input[0] == 'Y');
}

// int main() {
    bitboard board = 0;
    int currentPlayer = 1;
    int row, col;
    bool gameRunning = true;

    printf("Welcome to Tic-Tac-Toe!\n");

    while (gameRunning) {
        printf("\nPlayer %d's turn\n", currentPlayer);
        printBoard(board);

        // Input row and column from the current player
        printf("Enter row (1-3) and column (1-3): ");
        scanf("%d %d", &row, &col);

        if (makeMove(&board, row, col, currentPlayer)) {
            for (int i = 0; i < 8; i++) {
                if (checkWin(board, WIN_MASKS[i])) {
                    printBoard(board);
                    printf("Player %d wins!\n", currentPlayer);
                    gameRunning = askToContinue();
                    break;
                }
            }
            if (checkDraw(board)) {
                printBoard(board);
                printf("It's a draw!\n");
                gameRunning = askToContinue();
            } else {
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
            }
        } else {
            printf("Invalid move! Try again.\n");
        }

        // Ask the user if they want to start a new game or continue the game from the beginning
        char input[10];
        printf("Do you want to start a new game or continue from the beginning? (n/c): ");
        scanf("%s", input);
        if (input[0] == 'n' || input[0] == 'N') {
            board = 0;
            currentPlayer = 1;
        } else {
            loadGame(&board);
            currentPlayer = 2;
        }
    }
    saveGame(board); // Save the final game state
    printf("Press Enter to exit...");
    getchar(); // Wait for user input before closing window
    return 0;
}