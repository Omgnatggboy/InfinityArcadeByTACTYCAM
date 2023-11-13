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

// Function prototypes
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
void readGameHistoryFromCSV(); // Function to read game results from CSV
void computerMove(char player);
bool isValidMove(int position);
int getComputerMove(char player);
bool isBoardFull();
char* getCurrentDateTime();

int main() {
    srand(time(NULL));
    printf("Welcome to Tic Tac Toe!\n");
    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Play against a friend\n");
        printf("2. Play against the computer\n");
        printf("3. View game results\n");
        printf("4. Save game history to CSV\n");
        printf("5. View game results from CSV\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                playAgainstComputer();
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

// Initialize the Tic Tac Toe board
void initializeBoard() {
    int position = 1;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = position + '0'; // Fill the board with numbers 1 to 9
            position++;
        }
    }
}

// Print the Tic Tac Toe board with numbered positions
void printBoard() {
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf(" %c ", board[i][j]);
            if (j < BOARD_SIZE - 1) {
                printf("|");
            }
        }
        printf("\n");
        if (i < BOARD_SIZE - 1) {
            printf("---|---|---\n");
        }
    }
    printf("\n");
}

// Make a move on the Tic Tac Toe board
bool makeMove(char player, int position) {
    int row, col;
    position--; // Adjust position to array index

    if (position < 0 || position >= BOARD_SIZE * BOARD_SIZE) {
        return false; // Invalid position
    }

    row = position / BOARD_SIZE;
    col = position % BOARD_SIZE;

    if (board[row][col] >= '1' && board[row][col] <= '9') {
        board[row][col] = player;
        return true;
    }
    return false;
}

// Check if a player has won
bool checkWin(char player) {
    // Check rows and columns
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true; // Row win
        }
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
            return true; // Column win
        }
    }

    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true; // Diagonal win (top-left to bottom-right)
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true; // Diagonal win (top-right to bottom-left)
    }

    return false;
}

// Check if the game is a draw
bool checkDraw() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] >= '1' && board[i][j] <= '9') {
                return false; // There are still empty positions
            }
        }
    }
    return true;
}

// Play a game of Tic Tac Toe against a friend
void playGame() {
    char player1[MAX_NAME_LENGTH], player2[MAX_NAME_LENGTH];
    char currentPlayer = 'X';
    int position;
    int roundsPlayed = 0;

    printf("Enter the name of Player 1 (X): ");
    scanf("%s", player1);
    printf("Enter the name of Player 2 (O): ");
    scanf("%s", player2);

    initializeBoard();

    printf("Let's start the game!\n");

    while (true) {
        printBoard();
        printf("%s's turn (%c):\n", (currentPlayer == 'X') ? player1 : player2, currentPlayer);
        printf("Enter the position (1-9): ");
        scanf("%d", &position);

        if (makeMove(currentPlayer, position)) {
            if (checkWin(currentPlayer)) {
                printBoard();
                printf("%s wins!\n", (currentPlayer == 'X') ? player1 : player2);
                addGameToHistory((currentPlayer == 'X') ? player1 : player2, (currentPlayer == 'X') ? player1 : player2, roundsPlayed + 1, getCurrentDateTime());
                break;
            } else if (checkDraw()) {
                printBoard();
                printf("It's a draw!\n");
                addGameToHistory(player1, "Draw", roundsPlayed + 1, getCurrentDateTime());
                addGameToHistory(player2, "Draw", roundsPlayed + 1, getCurrentDateTime());
                break;
            } else {
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                roundsPlayed++;
            }
        } else {
            printf("Invalid move. Try again.\n");
        }
    }
}

// Play a game of Tic Tac Toe against the computer
void playAgainstComputer() {
    char player1[MAX_NAME_LENGTH];
    char currentPlayer = 'X';
    int position;
    int roundsPlayed = 0;

    printf("Enter your name (X): ");
    scanf("%s", player1);

    initializeBoard();

    printf("Let's start the game against the computer!\n");

    while (true) {
        if (currentPlayer == 'X') {
            printBoard();
            printf("%s's turn (%c):\n", player1, currentPlayer);
            printf("Enter the position (1-9): ");
            scanf("%d", &position);

            if (makeMove(currentPlayer, position)) {
                if (checkWin(currentPlayer)) {
                    printBoard();
                    printf("%s wins!\n", player1);
                    addGameToHistory(player1, player1, roundsPlayed + 1, getCurrentDateTime());
                    break;
                } else if (checkDraw()) {
                    printBoard();
                    printf("It's a draw!\n");
                    addGameToHistory(player1, "Draw", roundsPlayed + 1, getCurrentDateTime());
                    break;
                } else {
                    currentPlayer = 'O';
                    roundsPlayed++;
                }
            } else {
                printf("Invalid move. Try again.\n");
            }
        } else {
            printf("Computer's turn (O):\n");
            computerMove('O'); // Computer (O) makes a move

            if (checkWin('O')) {
                printBoard();
                printf("Computer wins!\n");
                addGameToHistory("Computer", "Computer", roundsPlayed + 1, getCurrentDateTime());
                break;
            } else if (checkDraw()) {
                printBoard();
                printf("It's a draw!\n");
                addGameToHistory(player1, "Draw", roundsPlayed + 1, getCurrentDateTime());
                break;
            } else {
                currentPlayer = 'X';
                roundsPlayed++;
            }
        }
    }
}

// Computer makes a move (intelligent move)
void computerMove(char player) {
    int position = getComputerMove(player);
    makeMove(player, position);
}

// Check if the board is full
bool isBoardFull() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] >= '1' && board[i][j] <= '9') {
                return false; // There are still empty positions
            }
        }
    }
    return true;
}

// Get the computer's move
int getComputerMove(char player) {
    // Check if the computer can win in the next move
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] >= '1' && board[i][j] <= '9') {
                char originalValue = board[i][j];
                board[i][j] = player;
                if (checkWin(player)) {
                    board[i][j] = originalValue;
                    return (i * BOARD_SIZE + j + 1); // Winning move
                }
                board[i][j] = originalValue;
            }
        }
    }

    // Check if the player can win in the next move and block them
    char opponent = (player == 'X') ? 'O' : 'X';
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] >= '1' && board[i][j] <= '9') {
                char originalValue = board[i][j];
                board[i][j] = opponent;
                if (checkWin(opponent)) {
                    board[i][j] = player;
                    return (i * BOARD_SIZE + j + 1); // Blocking opponent's winning move
                }
                board[i][j] = originalValue;
            }
        }
    }

    // If no winning or blocking move, make a strategic move
    if (board[1][1] >= '1' && board[1][1] <= '9') {
        return 5; // Center is the best move
    }

    // Choose a corner if available
    if (board[0][0] >= '1' && board[0][0] <= '9') {
        return 1;
    }
    if (board[0][2] >= '1' && board[0][2] <= '9') {
        return 3;
    }
    if (board[2][0] >= '1' && board[2][0] <= '9') {
        return 7;
    }
    if (board[2][2] >= '1' && board[2][2] <= '9') {
        return 9;
    }

    // Choose an edge if no corner is available
    if (board[0][1] >= '1' && board[0][1] <= '9') {
        return 2;
    }
    if (board[1][0] >= '1' && board[1][0] <= '9') {
        return 4;
    }
    if (board[1][2] >= '1' && board[1][2] <= '9') {
        return 6;
    }
    if (board[2][1] >= '1' && board[2][1] <= '9') {
        return 8;
    }

    return -1; // No available moves
}

// Check if a move is valid (within bounds and the cell is empty)
bool isValidMove(int position) {
    position--; // Adjust position to array index
    int row = position / BOARD_SIZE;
    int col = position % BOARD_SIZE;

    return position >= 0 && position < BOARD_SIZE * BOARD_SIZE && board[row][col] >= '1' && board[row][col] <= '9';
}

// Add a game result to the history
void addGameToHistory(const char playerName[], const char winner[], int roundsPlayed, const char dateTime[]) {
    if (gameCount < MAX_HISTORY) {
        strcpy(gameHistory[gameCount].playerName, playerName);
        strcpy(gameHistory[gameCount].winner, winner);
        gameHistory[gameCount].roundsPlayed = roundsPlayed;
        strcpy(gameHistory[gameCount].dateTime, dateTime);
        gameCount++;
    }
}

// View game results
void viewGameResults() {
    printf("Game Results:\n");
    printf("Player Name\tWinner\tRounds Played\tDate and Time\n");
    for (int i = 0; i < gameCount; i++) {
        printf("%s\t%s\t%d\t%s\n", gameHistory[i].playerName, gameHistory[i].winner, gameHistory[i].roundsPlayed, gameHistory[i].dateTime);
    }
}

// Save game history to a CSV file
void saveGameHistoryToCSV() {
    FILE *csvFile = fopen("game_history.csv", "w");
    if (csvFile == NULL) {
        printf("Error opening the CSV file for writing.\n");
        return;
    }

    fprintf(csvFile, "Player Name,Winner,Rounds Played,Date and Time\n");
    for (int i = 0; i < gameCount; i++) {
        fprintf(csvFile, "%s,%s,%d,%s\n", gameHistory[i].playerName, gameHistory[i].winner, gameHistory[i].roundsPlayed, gameHistory[i].dateTime);
    }

    fclose(csvFile);
    printf("Game history saved to game_history.csv\n");
}


char* getCurrentDateTime() {
    time_t rawtime;
    struct tm* timeinfo;
    static char dateTime[20];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", timeinfo);

    return dateTime;
}

void readGameHistoryFromCSV() {
    FILE *csvFile = fopen("game_history.csv", "r");
    if (csvFile == NULL) {
        printf("Game history CSV file not found.\n");
        return;
    }

    char line[256];
    printf("Game Results from CSV File:\n");
    printf("Player Name\tWinner\tRounds Played\tDate/Time\n");
    while (fgets(line, sizeof(line), csvFile)) {
        char playerName[MAX_NAME_LENGTH];
        char winner[MAX_NAME_LENGTH];
        int roundsPlayed;
        char dateTime[20];

        if (sscanf(line, "%[^,],%[^,],%d,%19[^\n]", playerName, winner, &roundsPlayed, dateTime) == 4) {
            printf("%s\t\t%s\t%d\t\t%s\n", playerName, winner, roundsPlayed, dateTime);
        } else {
            printf("Error parsing line: %s", line);
        }
    }
    fclose(csvFile);
}