#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORDS 20
#define BOARD_SIZE 3

typedef struct {
    char name[50];
    int gamesPlayed;
    int humanWins;
    int aiWins;
    int draws;
    int currentStreak;
    int bestStreak;
    int totalScore;
} Player;

Player player;
int difficulty = 2;

void clearInput()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pauseScreen()
{
    printf("\nPress ENTER to continue...");
    getchar();
}

void saveData()
{
    FILE *file = fopen("player.dat", "wb");

    if (file == NULL)
        return;

    fwrite(&player, sizeof(Player), 1, file);
    fwrite(&difficulty, sizeof(int), 1, file);

    fclose(file);
}

void loadData()
{
    FILE *file = fopen("player.dat", "rb");

    if (file == NULL) {
        strcpy(player.name, "Player");
        player.gamesPlayed = 0;
        player.humanWins = 0;
        player.aiWins = 0;
        player.draws = 0;
        player.currentStreak = 0;
        player.bestStreak = 0;
        player.totalScore = 0;
        difficulty = 2;
        return;
    }

    fread(&player, sizeof(Player), 1, file);
    fread(&difficulty, sizeof(int), 1, file);

    fclose(file);
}

void win()
{
    player.gamesPlayed++;
    player.humanWins++;
    player.currentStreak++;
    player.totalScore += 10;

    if (player.currentStreak > player.bestStreak)
        player.bestStreak = player.currentStreak;

    saveData();
}

void loss()
{
    player.gamesPlayed++;
    player.aiWins++;
    player.currentStreak = 0;

    saveData();
}

void draw()
{
    player.gamesPlayed++;
    player.draws++;
    player.totalScore += 2;

    saveData();
}

void header(char title[])
{
    system("cls");

    printf("\n============================================================\n");
    printf("                     %s\n", title);
    printf("============================================================\n");
}

void playerProfile()
{
    header("PLAYER PROFILE");

    printf("\nName           : %s\n", player.name);
    printf("Games Played   : %d\n", player.gamesPlayed);
    printf("Human Wins     : %d\n", player.humanWins);
    printf("AI Wins        : %d\n", player.aiWins);
    printf("Draws          : %d\n", player.draws);
    printf("Current Streak : %d\n", player.currentStreak);
    printf("Best Streak    : %d\n", player.bestStreak);
    printf("Total Score    : %d\n", player.totalScore);

    pauseScreen();
}

void statistics()
{
    float humanRate = 0;
    float aiRate = 0;

    if (player.gamesPlayed > 0) {
        humanRate =
            ((float)player.humanWins / player.gamesPlayed) * 100;

        aiRate =
            ((float)player.aiWins / player.gamesPlayed) * 100;
    }

    header("AI STATISTICS");

    printf("\nGames Played     : %d\n", player.gamesPlayed);
    printf("Human Wins       : %d\n", player.humanWins);
    printf("AI Wins          : %d\n", player.aiWins);
    printf("Draws            : %d\n", player.draws);

    printf("\nHuman Win Rate   : %.2f%%\n", humanRate);
    printf("AI Win Rate      : %.2f%%\n", aiRate);

    printf("\nCurrent Streak   : %d\n", player.currentStreak);
    printf("Best Streak      : %d\n", player.bestStreak);
    printf("Total Score      : %d\n", player.totalScore);

    pauseScreen();
}

void changeName()
{
    header("PLAYER SETTINGS");

    printf("\nCurrent Name: %s\n", player.name);

    printf("\nEnter new name: ");

    fgets(player.name, 50, stdin);

    player.name[strcspn(player.name, "\n")] = '\0';

    saveData();

    printf("\nName updated successfully.\n");

    pauseScreen();
}

void changeDifficulty()
{
    int choice;

    header("DIFFICULTY");

    printf("\n1. Easy\n");
    printf("2. Medium\n");
    printf("3. Hard\n");

    printf("\nCurrent Difficulty: ");

    if (difficulty == 1)
        printf("Easy\n");
    else if (difficulty == 2)
        printf("Medium\n");
    else
        printf("Hard\n");

    printf("\nSelect difficulty: ");
    scanf("%d", &choice);
    clearInput();

    if (choice >= 1 && choice <= 3) {
        difficulty = choice;
        saveData();

        printf("\nDifficulty updated.\n");
    }
    else {
        printf("\nInvalid choice.\n");
    }

    pauseScreen();
}

void numberGuessing()
{
    int secret;
    int guess;
    int attempts = 0;
    int maxAttempts;

    header("NUMBER GUESSING");

    if (difficulty == 1)
        maxAttempts = 10;
    else if (difficulty == 2)
        maxAttempts = 7;
    else
        maxAttempts = 5;

    secret = rand() % 100 + 1;

    printf("\nThe AI has selected a number between 1 and 100.\n");
    printf("You have %d attempts.\n", maxAttempts);

    while (attempts < maxAttempts) {

        printf("\nAttempt %d: ", attempts + 1);
        scanf("%d", &guess);

        attempts++;

        if (guess == secret) {

            printf("\nYou defeated the AI!\n");
            printf("The number was %d.\n", secret);

            win();
            pauseScreen();
            return;
        }

        if (guess < secret)
            printf("AI: Too low.\n");
        else
            printf("AI: Too high.\n");
    }

    printf("\nAI wins!\n");
    printf("The number was %d.\n", secret);

    loss();

    pauseScreen();
}

int getRPSWinner(int human, int ai)
{
    if (human == ai)
        return 0;

    if ((human == 1 && ai == 3) ||
        (human == 2 && ai == 1) ||
        (human == 3 && ai == 2))
        return 1;

    return -1;
}

void rockPaperScissors()
{
    int human;
    int ai;
    int history[4] = {0};
    int rounds = 5;

    header("ROCK PAPER SCISSORS");

    printf("\n1. Rock\n");
    printf("2. Paper\n");
    printf("3. Scissors\n");

    for (int round = 1; round <= rounds; round++) {

        printf("\nRound %d\n", round);
        printf("Choose: ");
        scanf("%d", &human);

        if (human < 1 || human > 3) {
            printf("Invalid choice.\n");
            round--;
            continue;
        }

        if (round <= 2 || difficulty == 1) {
            ai = rand() % 3 + 1;
        }
        else {
            if (history[1] >= history[2] &&
                history[1] >= history[3])
                ai = 2;
            else if (history[2] >= history[1] &&
                     history[2] >= history[3])
                ai = 3;
            else
                ai = 1;
        }

        history[human]++;

        printf("You: ");

        if (human == 1)
            printf("Rock\n");
        else if (human == 2)
            printf("Paper\n");
        else
            printf("Scissors\n");

        printf("AI: ");

        if (ai == 1)
            printf("Rock\n");
        else if (ai == 2)
            printf("Paper\n");
        else
            printf("Scissors\n");

        int result = getRPSWinner(human, ai);

        if (result == 1) {
            printf("You win this round!\n");
        }
        else if (result == -1) {
            printf("AI wins this round!\n");
        }
        else {
            printf("Draw!\n");
        }
    }

    printf("\nGame completed.\n");
    printf("Your choices were analyzed by the AI.\n");

    pauseScreen();
}

void printBoard(char board[3][3])
{
    printf("\n");

    for (int i = 0; i < 3; i++) {

        printf(" %c | %c | %c\n",
               board[i][0],
               board[i][1],
               board[i][2]);

        if (i < 2)
            printf("---+---+---\n");
    }
}

int checkWinner(char board[3][3])
{
    for (int i = 0; i < 3; i++) {

        if (board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
            return board[i][0];

        if (board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
            return board[0][i];
    }

    if (board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
        return board[0][0];

    if (board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
        return board[0][2];

    return 0;
}

int emptyCells(char board[3][3])
{
    int count = 0;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ')
                count++;

    return count;
}

int findWinningMove(char board[3][3], char symbol)
{
    for (int i = 0; i < 3; i++) {

        for (int j = 0; j < 3; j++) {

            if (board[i][j] == ' ') {

                board[i][j] = symbol;

                if (checkWinner(board) == symbol) {
                    board[i][j] = ' ';
                    return i * 3 + j;
                }

                board[i][j] = ' ';
            }
        }
    }

    return -1;
}

int aiMove(char board[3][3])
{
    int move;

    if (difficulty >= 2) {

        move = findWinningMove(board, 'O');

        if (move != -1)
            return move;

        move = findWinningMove(board, 'X');

        if (move != -1)
            return move;
    }

    if (board[1][1] == ' ')
        return 4;

    int corners[] = {0, 2, 6, 8};

    for (int i = 0; i < 4; i++) {

        if (board[corners[i] / 3][corners[i] % 3] == ' ')
            return corners[i];
    }

    do {
        move = rand() % 9;
    } while (board[move / 3][move % 3] != ' ');

    return move;
}

void ticTacToe()
{
    char board[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    int move;
    int result;

    header("TIC TAC TOE");

    while (emptyCells(board) > 0) {

        printBoard(board);

        printf("\nChoose position 1-9: ");
        scanf("%d", &move);

        if (move < 1 || move > 9) {
            printf("Invalid position.\n");
            continue;
        }

        move--;

        if (board[move / 3][move % 3] != ' ') {
            printf("Position already occupied.\n");
            continue;
        }

        board[move / 3][move % 3] = 'X';

        result = checkWinner(board);

        if (result == 'X') {
            printBoard(board);
            printf("\nYou defeated the AI!\n");
            win();
            pauseScreen();
            return;
        }

        if (emptyCells(board) == 0)
            break;

        move = aiMove(board);

        board[move / 3][move % 3] = 'O';

        result = checkWinner(board);

        if (result == 'O') {
            printBoard(board);
            printf("\nAI wins!\n");
            loss();
            pauseScreen();
            return;
        }
    }

    printBoard(board);

    printf("\nThe game is a draw.\n");

    draw();

    pauseScreen();
}

void memoryChallenge()
{
    int sequence[10];
    int length = 3;
    int answer;
    int correct;

    header("MEMORY CHALLENGE");

    if (difficulty == 1)
        length = 3;
    else if (difficulty == 2)
        length = 5;
    else
        length = 7;

    printf("\nRemember this sequence:\n\n");

    for (int i = 0; i < length; i++) {

        sequence[i] = rand() % 10;

        printf("%d ", sequence[i]);
    }

    printf("\n\n");

    printf("Memorize it...\n");

    for (volatile long i = 0; i < 500000000; i++);

    system("cls");

    header("MEMORY CHALLENGE");

    printf("\nEnter the sequence:\n");

    correct = 1;

    for (int i = 0; i < length; i++) {

        scanf("%d", &answer);

        if (answer != sequence[i])
            correct = 0;
    }

    if (correct) {

        printf("\nExcellent memory! You defeated the AI.\n");

        win();
    }
    else {

        printf("\nAI wins this challenge.\n");

        printf("Correct sequence: ");

        for (int i = 0; i < length; i++)
            printf("%d ", sequence[i]);

        printf("\n");

        loss();
    }

    pauseScreen();
}

int checkPrime(int n)
{
    if (n < 2)
        return 0;

    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return 0;

    return 1;
}

void numberMaster()
{
    int type;
    int answer;
    int correctAnswer;

    header("NUMBER MASTER");

    printf("\nAI will generate a mathematical challenge.\n");

    type = rand() % 3;

    if (type == 0) {

        int start = rand() % 10 + 1;

        printf("\nSequence:\n");
        printf("%d %d %d %d ?\n",
               start,
               start + 2,
               start + 4,
               start + 6);

        correctAnswer = start + 8;
    }

    else if (type == 1) {

        int start = rand() % 5 + 2;

        printf("\nSequence:\n");
        printf("%d %d %d %d ?\n",
               start,
               start * 2,
               start * 4,
               start * 8);

        correctAnswer = start * 16;
    }

    else {

        int start = rand() % 10 + 5;

        printf("\nSequence:\n");
        printf("%d %d %d ?\n",
               start,
               start + 5,
               start + 10);

        correctAnswer = start + 15;
    }

    printf("\nYour answer: ");
    scanf("%d", &answer);

    if (answer == correctAnswer) {

        printf("\nCorrect! Human wins.\n");

        win();
    }
    else {

        printf("\nWrong!\n");
        printf("Correct answer: %d\n", correctAnswer);

        loss();
    }

    pauseScreen();
}

char words[MAX_WORDS][20] = {
    "computer",
    "software",
    "programming",
    "algorithm",
    "database",
    "network",
    "compiler",
    "keyboard",
    "internet",
    "security",
    "developer",
    "variable",
    "function",
    "pointer",
    "structure",
    "terminal",
    "processor",
    "memory",
    "digital",
    "technology"
};

void wordBattle()
{
    char word[20];
    char guess[20];

    int index;
    int score = 0;
    int attempts = 3;

    header("WORD BATTLE");

    index = rand() % MAX_WORDS;

    strcpy(word, words[index]);

    printf("\nThe AI selected a hidden technology word.\n");
    printf("You have %d attempts.\n", attempts);

    printf("\nHint: The word has %d letters.\n",
           (int)strlen(word));

    while (attempts > 0) {

        printf("\nGuess: ");

        scanf("%s", guess);

        if (strcmp(guess, word) == 0) {

            score += attempts * 5;

            printf("\nCorrect!\n");
            printf("You defeated the AI.\n");
            printf("Score earned: %d\n", score);

            win();

            pauseScreen();
            return;
        }

        attempts--;

        printf("Wrong guess.\n");

        if (attempts > 0)
            printf("Attempts remaining: %d\n", attempts);
    }

    printf("\nAI wins!\n");
    printf("The word was: %s\n", word);

    loss();

    pauseScreen();
}

void survivalChallenge()
{
    int health = 100;
    int food = 50;
    int day = 1;
    int choice;

    header("SURVIVAL CHALLENGE");

    printf("\nSurvive for 5 days.\n");

    while (day <= 5 && health > 0) {

        printf("\n====================================\n");
        printf("DAY %d\n", day);
        printf("====================================\n");

        printf("Health : %d\n", health);
        printf("Food   : %d\n", food);

        printf("\n1. Search for food\n");
        printf("2. Rest\n");
        printf("3. Explore\n");

        printf("\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) {

            int found = rand() % 30 + 10;

            food += found;

            printf("\nYou found %d food.\n", found);
        }

        else if (choice == 2) {

            health += 10;

            if (health > 100)
                health = 100;

            printf("\nYou rested and recovered health.\n");
        }

        else if (choice == 3) {

            int danger = rand() % 3;

            if (danger == 0) {

                printf("\nYou found useful supplies!\n");

                food += 20;
            }
            else if (danger == 1) {

                printf("\nYou encountered danger!\n");

                health -= 20;
            }
            else {

                printf("\nNothing happened.\n");
            }
        }

        else {

            printf("\nInvalid choice.\n");
            continue;
        }

        food -= 10;

        if (food < 0) {
            health -= 20;
            food = 0;
        }

        day++;
    }

    if (health > 0) {

        printf("\n====================================\n");
        printf("        SURVIVAL SUCCESSFUL\n");
        printf("====================================\n");

        printf("\nYou survived all 5 days!\n");

        win();
    }
    else {

        printf("\n====================================\n");
        printf("             GAME OVER\n");
        printf("====================================\n");

        printf("\nThe AI predicted your survival strategy.\n");

        loss();
    }

    pauseScreen();
}

void gameMenu()
{
    int choice;

    do {

        header("GAME ARENA");

        printf("\n1. Number Guessing\n");
        printf("2. Rock Paper Scissors\n");
        printf("3. Tic Tac Toe\n");
        printf("4. Memory Challenge\n");
        printf("5. Number Master\n");
        printf("6. Word Battle\n");
        printf("7. Survival Challenge\n");
        printf("0. Back\n");

        printf("\nSelect game: ");
        scanf("%d", &choice);
        clearInput();

        switch (choice) {

            case 1:
                numberGuessing();
                break;

            case 2:
                rockPaperScissors();
                break;

            case 3:
                ticTacToe();
                break;

            case 4:
                memoryChallenge();
                break;

            case 5:
                numberMaster();
                break;

            case 6:
                wordBattle();
                break;

            case 7:
                survivalChallenge();
                break;

            case 0:
                break;

            default:
                printf("\nInvalid choice.\n");
                pauseScreen();
        }

    } while (choice != 0);
}

void instructions()
{
    header("INSTRUCTIONS");

    printf("\nAI vs Human is a collection of games where\n");
    printf("you compete against different computer strategies.\n");

    printf("\nYour performance is tracked through:\n");

    printf("- Human wins\n");
    printf("- AI wins\n");
    printf("- Draws\n");
    printf("- Total score\n");
    printf("- Current streak\n");
    printf("- Best streak\n");

    printf("\nDifficulty levels:\n");

    printf("\n1. Easy");
    printf("\n2. Medium");
    printf("\n3. Hard");

    printf("\n\nTry different games and discover which\n");
    printf("AI you can defeat.\n");

    pauseScreen();
}

void mainMenu()
{
    int choice;

    do {

        header("MAIN MENU");

        printf("\nWelcome, %s!\n", player.name);

        printf("\nAI STATUS: ONLINE");

        printf("\n\n------------------------------------------------------------\n");

        printf("1. Start Game\n");
        printf("2. AI Statistics\n");
        printf("3. Player Profile\n");
        printf("4. Change Player Name\n");
        printf("5. Difficulty\n");
        printf("6. Instructions\n");
        printf("0. Shutdown\n");

        printf("------------------------------------------------------------\n");

        printf("\nSelect option: ");
        scanf("%d", &choice);
        clearInput();

        switch (choice) {

            case 1:
                gameMenu();
                break;

            case 2:
                statistics();
                break;

            case 3:
                playerProfile();
                break;

            case 4:
                changeName();
                break;

            case 5:
                changeDifficulty();
                break;

            case 6:
                instructions();
                break;

            case 0:

                saveData();

                printf("\nSaving player data...\n");
                printf("AI vs Human shutting down...\n");
                printf("Goodbye, %s!\n", player.name);

                break;

            default:

                printf("\nInvalid option.\n");
                pauseScreen();
        }

    } while (choice != 0);
}

int main()
{
    srand((unsigned int)time(NULL));

    loadData();

    system("cls");

    printf("\n============================================================\n");
    printf("                     AI vs HUMAN\n");
    printf("============================================================\n");

    printf("\nInitializing AI...\n");
    printf("Loading Game Arena...\n");
    printf("Loading Player Data...\n");

    printf("\nAI STATUS: ONLINE\n");

    pauseScreen();

    mainMenu();

    return 0;
}