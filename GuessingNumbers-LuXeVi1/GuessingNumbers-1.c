#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_HISTORY 100

// Structure to store game history
typedef struct {
    char playerName[MAX_NAME_LENGTH];
    int targetNumber;
    int numGuesses;
    /*char result[20]*/;
} GameRecord;

// Function
void playGame();
void addGameToHistory(const char playerName[], int targetNumber, int numGuesses/*, const char result[]*/);
void searchGameHistory(const char playerName[]);
void summarizeGameHistory();
void saveGameHistoryToCSV();
// void loadGameHistoryFromCSV();
void viewGameResultsFromCSV();

GameRecord gameHistory[MAX_HISTORY];
int gameCount = 0;

int main() {
    srand(time(NULL));
    // loadGameHistoryFromCSV(); // Load game history from CSV file if available
    printf("Welcome to Guessing Numbers game!\n");
    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Play a game\n");
        printf("2. Search for game history by player name\n");
        printf("3. Summarize game history\n");
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
                saveGameHistoryToCSV();
                break;
            case 5:
                viewGameResultsFromCSV();
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

// Function to play a game
void playGame() {
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
            // addGameToHistory(playerName, number, numGuesses, "Win");
        }
    } while (guess != number);

    addGameToHistory(playerName, number, numGuesses);
}

// Function to add a game to the history
void addGameToHistory(const char playerName[], int targetNumber, int numGuesses/*, const char result[]*/) {
    if (gameCount < MAX_HISTORY) {
        strcpy(gameHistory[gameCount].playerName, playerName);
        gameHistory[gameCount].targetNumber = targetNumber;
        gameHistory[gameCount].numGuesses = numGuesses;
        // strcpy(gameHistory[gameCount].result, result);
        gameCount++;
    }
}

// Function to search for game history by player name
void searchGameHistory(const char playerName[]) {
    printf("Game History for Player: %s\n", playerName);
    printf("===================================\n");
    printf("Name\tTarget Number\tGuesses\n"); // \tResult\n");
    for (int i = 0; i < gameCount; i++) {
        if (strcmp(playerName, gameHistory[i].playerName) == 0) {
            printf("%s\t%d\t\t%d\n", gameHistory[i].playerName, gameHistory[i].targetNumber, gameHistory[i].numGuesses/*\t%s, gameHistory[i].result*/);
        }
    }
}

// Function to summarize game history
void summarizeGameHistory() {
    printf("Game History Summary\n");
    printf("====================\n");
    printf("Total Games Played: %d\n", gameCount);

    // int totalWins = 0, totalLosses = 0, totalGuesses = 0;
    // for (int i = 0; i < gameCount; i++) {
    //     if (strcmp(gameHistory[i].result, "Win") == 0) {
    //         totalWins++;
    //     } else {
    //         totalLosses++;
    //     }
    //     totalGuesses += gameHistory[i].numGuesses;
    // }

    // printf("Total Wins: %d\n", totalWins);
    // printf("Total Losses: %d\n", totalLosses);
    // printf("Average Guesses per Game: %.2f\n", (float)totalGuesses / gameCount);
}

// Function to save game history to a CSV file
void saveGameHistoryToCSV() {
    FILE *csvFile = fopen("game_history.csv", "w");
    if (csvFile == NULL) {
        printf("Error opening the CSV file for writing.\n");
        return;
    }

    fprintf(csvFile, "Player Name,Target Number,Guesses\n");
    for (int i = 0; i < gameCount; i++) {
        fprintf(csvFile, "%s,%d,%d\n", gameHistory[i].playerName, gameHistory[i].targetNumber, gameHistory[i].numGuesses /*,%s , gameHistory[i].result*/);
    }

    fclose(csvFile);
    printf("Game history saved to game_history.csv\n");
}

// Function to load game history from a CSV file
// void loadGameHistoryFromCSV() {
//     FILE *csvFile = fopen("game_history.csv", "r");
//     if (csvFile == NULL) {
//         return; // File doesn't exist or can't be opened so return
//     }
//     char line[256];
//     int count = 0;
//     while (fgets(line, sizeof(line), csvFile) && count < MAX_HISTORY) {
//         sscanf(line, "%[^,],%d,%d", gameHistory[count].playerName, &gameHistory[count].targetNumber, &gameHistory[count].numGuesses /*,%s , gameHistory[count].result*/);
//         count++;
//     }
//     fclose(csvFile);
//     gameCount = count;
// }

// Function to view game results from a CSV file
void viewGameResultsFromCSV() {
    FILE *csvFile = fopen("game_history.csv", "r");
    if (csvFile == NULL) {
        printf("Game history CSV file not found.\n");
        return;
    }

    char line[256];
    printf("Game Results from CSV File:\n");
    printf("Player Name\tTarget Number\tGuesses\n"); /*\tResult*/
    while (fgets(line, sizeof(line), csvFile)) {
        printf("%s", line);
    }
    fclose(csvFile);
}
