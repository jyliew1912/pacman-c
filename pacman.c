#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define STORE_SIZE 3
#define DATA_SIZE 4
#define GAME_POINT 0
#define GAME_SIZE 1
#define GAME_MOVE 2
#define GAME_SCORE 3

void intro();
int input(const char *question);
char** allocate(int length, int width);
void initialiseMap(char **map, int length, int width, int *point, int level);
void displayMap(char **map, int length, int width, int live, int point);
void gameLoop(char **map, int length, int width, int *live, int *point, int *coins, 
 int *times, int *level, int *count, int movePoint, int arr[][DATA_SIZE], bool *end);

char characterHit(char userMove, char **map, int length, int width, int *point, int *movePoint, int *coins);
void run(char hit, char **map, int length, int width, int *live, int *game, int *point, int level);
void scramble(char **map, int length, int width, int *game, int *live);
bool positionCheck(char **map, int length, int width);
void arrayStoring(int *times, int point, int size, int moves, int arr[][DATA_SIZE], int *score, int level);
void winOutput(int arr[][DATA_SIZE], int times, int *recentScore);
bool gameOver(int *coins, int *game);
bool playAgain(int *times, int *level, int *count);
void exitReturn();
void deallocate(char **map, int length);

void randomLocation(char **map, int length, int width, char func);
int moveRow(int row, int column, int length, char userMove);
int moveColumn(int row, int column, int width, char userMove);
void lessMonster(char **map, int length, int width);
int count(char **map, int length, int width, char function);
bool check(char **map, int row, int column, int length, int width, char func);
bool win(char **map, int length, int width);
void ranking(int arr[][DATA_SIZE]);
void swap(int arr[][DATA_SIZE], int a, int b);

int main()
{
    // an array which can store top 3
    int arr[STORE_SIZE][DATA_SIZE] = {0};
    int times, level, count, coins;
    times = coins = 0;
    level = count = 1;
    intro();

    bool endWhile = false;
    while(1)
    {
        // user's input
        printf("\nLEVEL %d (%d / %d)\n", level, count, level);
        int length = input("Enter the X dimension of the map (minimum 5): ");
        int width = input("Enter the Y dimension of the map (minimum 5): ");

        // allocate memory
        char **map = allocate(length, width);

        // create the map
        int live = 1;
        int movePoint, point;
        movePoint = point = 0;

        srand(time(0));
        initialiseMap(map, length, width, &movePoint, level);
        displayMap(map, length, width, live, point);

        // on going game loop
        gameLoop(map, length, width, &live, &point, &coins, &times, &level, &count, movePoint, arr, &endWhile);
        if(endWhile) break;
        deallocate(map, length);
    }
    return 0;
}

void intro()
{
    // ASCII ART: https://www.asciiart.eu/text-to-ascii-art
    printf(" ____   _    ____      __  __    _    _   _ \n");
    printf("|  _ \\ / \\  / ___|    |  \\/  |  / \\  | \\ | |\n");
    printf("| |_) / _ \\| |   _____| |\\/| | / _ \\ |  \\| |\n");
    printf("|  __/ ___ \\ |__|_____| |  | |/ ___ \\| |\\  |\n");
    printf("|_| /_/   \\_\\____|    |_|  |_/_/   \\_\\_| \\_|\n\n");

    printf("Objective:\n   Eat all 'S' (Score) to WIN!\n");
    printf("Game Elements:\n");
    printf("   'P' is Pac-Man\n");
    printf("   'S' represents delicious SCORE - eat them all!\n");
    printf("   'B' is the BOOSTER - chances with both GOOD AND BAD EFFECT!\n");
    printf("   'M' is the MONSTER - avoid them at all costs!\n");
    printf("   '$' is the COIN - 2 coins will exchange one live!\n");
    printf("   'X' is the BLOCK - find another place to go!\n");
    printf("Controls:\n   Utilize 'w', 's', 'a', 'd' keys to move Pac-Man Up, Down, Left, Right respectively.\n");
}

int input(const char *question)
{
    int output;
    do
    {
        printf("%s", question);
        scanf("%d", &output);
    } 
    while(output < 5);
    return output;
}

char** allocate(int length, int width) {
    char **map = (char**)malloc(length * sizeof(char*));
    if (!map) {
        perror("Failed to allocate map rows");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < length; i++) {
        map[i] = (char*)malloc(width * sizeof(char));
        if (!map[i]) {
            perror("Failed to allocate map column");
            exit(EXIT_FAILURE);
        }
    }
    return map;
}

void initialiseMap(char **map, int length, int width, int *point, int level)
{
    // let array first all in '.'
    for(int i = 0; i < length; i++)
    {
        for(int j = 0 ; j < width; j++) map[i][j] = '.';
    }
    randomLocation(map, length, width, 'P'); // only one 'P'
    randomLocation(map, length, width, '$'); // only one '$'
    randomLocation(map, length, width, 'S'); // at least one 'S'

    // set other functions in map
    char ch[] = {'S', 'B'};
    int size = length * width;
    int numCh;

    if(level == 1) numCh = size / 4; // 0.25
    if(level == 2) numCh = size / 3; // 0.33
    if(level == 3) numCh = size / 4; 

    // randomly set 'S' and 'B'
    for(int i = 0; i < numCh; i++)
    {
        int random = rand() % 2;
        randomLocation(map, length, width, ch[random]);
    }
    // set 'M'
    for(int i = 0; i < numCh / 3; i++) randomLocation(map, length, width, 'M');

    // if level 3, set 'X' block
    if(level == 3)
    {
        for(int i = 0; i < (size / 15); i++) randomLocation(map, length, width, 'X');
    }

    // add how many points for one 'S'
    float temp = 20 / count(map, length, width, 'S');
    *point = round(temp);
}

void randomLocation(char **map, int length, int width, char func)
{
    int randomI = rand() % length;
    int randomJ = rand() % width;

    if(map[randomI][randomJ] == '.')
    {
        map[randomI][randomJ] = func;
        return;
    }
    else
    {
        randomLocation(map, length, width, func);
    }
}

void displayMap(char **map, int length, int width, int live, int point)
{
    printf("\nlive: ");
    for(int i = 0; i < live; i++)
    {
        // unicode for a heart emoji: https://unicode.org/emoji/charts/full-emoji-list.html#1f970
        printf("\U00002764  ");	
    }
    printf("\npoint: %d\n", point);

    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j< width; j++) printf("%c ", map[i][j]);
        printf("\n");
    }
    printf("\n");
}

void gameLoop(char **map, int length, int width, int *live, int *point, int *coins, 
 int *times, int *level, int *count, int movePoint, int arr[][DATA_SIZE], bool *end)
{
    int game = 1;
    int moves, recentScore;
    moves = recentScore = 0;
    while (game > 0)
    {
        // User's move input
        char userMove;
        do
        {
            printf("Your move ('w'/'a'/'s'/'d'): ");
            scanf(" %c", &userMove);
        }
        while (userMove != 'w' && userMove != 'a' && userMove != 's' && userMove != 'd');
        moves++;

        if(game == 1)
        {
            // Find the character that player hit
            char hit = characterHit(userMove, map, length, width, point, &movePoint, coins);

            // Run the function of the map!
            run(hit, map, length, width, live, &game, point, *level);

            // Scramble the functions in the map
            scramble(map, length, width, &game, live);
            displayMap(map, length, width, *live, *point);
            bool position = positionCheck(map, length, width); // 4 positions surrounded by 'M' / 'X'
            if(!position) game = 0;
        } 
        else 
        {
            displayMap(map, length, width, *live, *point);
            game--;
        }

        if(win(map, length, width))
        {
            // store result to ranking array
            arrayStoring(times, *point, length * width, moves, arr, &recentScore, *level);
            winOutput(arr, *times, &recentScore);
            (*times)++;
            game = -1; // jump out the loop
        }
    }

    if(game == 0)
    {
        if(gameOver(coins, &game))
        {
            deallocate(map, length);
            *end = true;
            return;
        }
    }
    else
    {
        if(!playAgain(times, level, count))
        {
            deallocate(map, length);
            *end = true;
            return;
        }
    }
}

char characterHit(char userMove, char **map, int length, int width, int *point, int *movePoint, int *coins)
{
    int row, column;
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(map[i][j] == 'P')
            {
                // find player's location then update player's old location to '.'
                row = i;
                column = j;
                map[i][j] = '.';
                break;
            }
        }
    }
    int tempRow = row;
    int tempCol = column;

    // find the user's move
    row = moveRow(row, column, length, userMove);
    column = moveColumn(row, column, width, userMove);

    // find which charater does the player hit
    char hit = map[row][column];
    if(hit == 'S') *point += *movePoint;
    if(hit == '$') 
    {
        printf("coins +1!\n");
        *coins += 1;
    }
    if(hit == 'X')
    {
        // reverse back, do not change position
        row = tempRow;
        column = tempCol;
    }

    // update the player's new location
    map[row][column] = 'P';

    return hit;
}

int moveRow(int row, int column, int length, char userMove)
{
    switch(userMove)
    {
        case'w':
            row = (row == 0) ? length - 1 : row - 1;
            break;
        case's':
            row = (row == (length - 1)) ? 0 : row + 1;
            break;
    }
    return row;
}

int moveColumn(int row, int column, int width, char userMove)
{
    switch(userMove)
    {
        case'a':
            column = (column == 0) ? width - 1 : column - 1;
            break;
        case'd':
            column = (column == (width - 1)) ? 0 : column + 1;
            break;
    }
    return column;
}

void run(char hit, char **map, int length, int width, int *live, int *game, int *point, int level)
{
    switch(hit)
    {
        case'M':
            *game = (*live == 1) ? 0 : (*live -= 1);
            break;
        case'B':
            int random = rand() % 5;
            switch(random)
            {
                case 0:
                    printf("A FREEZE BOOSTER!\n");
                    *game = 3;
                    break;
                case 1:
                    printf("LESS MONSTER BOOSTER! (+3 points)\n");
                    *point += 3;
                    lessMonster(map, length, width);
                    break;
                case 2:
                    printf("EXTRA LIVE BOOSTER! (+3 points)\n");
                    *live += 1;
                    *point += 3;
                    break;
                case 3:
                    if(level > 2)
                    {
                        printf("BLOCK ADDING BOOSTER! (-3 points)\n");
                        *point -= 3;
                        randomLocation(map, length, width, 'X');
                    }
                    else
                    {
                        printf("MONSTER ADDING BOOSTER! (-3 points)\n");
                        *point -= 3;
                        randomLocation(map, length, width, 'M');
                    }
                    break;
                case 4:
                    printf("POINT BOOSTER! (+5 points)\n");
                    *point += 5;
                    break;
            }
            break;
    }
}

void lessMonster(char **map, int length, int width)
{
    if(count(map, length, width, 'M') == 0) return;

    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(map[i][j] == 'M')
            {
                map[i][j] = '.';
                return;
            }
        }
    }
}

int count(char **map, int length, int width, char function)
{
    int count = 0;
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(map[i][j] == function) count++;
        }
    }
    return count;
}

void scramble(char **map, int length, int width, int *game, int *live)
{
    int numberM = count(map, length, width, 'M');
    if(numberM == 0) return;

    // store position of M
    int arr[numberM][2];
    int num = 0;
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(map[i][j] == 'M')
            {
                arr[num][0] = i;
                arr[num][1] = j;
                num++;
            }
        }
    }

    int row, column, random;
    for(int i = 0; i < numberM; i++)
    {
        // on going do-while loop while surrounding have '.' / 'B'
        if(check(map, arr[i][0], arr[i][1], length, width, '.') || check(map, arr[i][0], arr[i][1], length, width, 'B'))
        {
            do
            {
                row = arr[i][0];
                column = arr[i][1];
                random = rand() % 4;
                switch(random)
                {
                    case 0:
                        row = moveRow(row, column, length, 'w');
                        break;
                    case 1:
                        row = moveRow(row, column, length, 's');
                        break;
                    case 2:
                        column = moveColumn(row, column, width, 'a');
                        break;
                    case 3:
                        column = moveColumn(row, column, width, 'd');
                        break;
                }
            } 
            while((map[row][column] == 'S') || (map[row][column] == 'X')); // repeat if new location hit 'S' / 'X'

            if(map[row][column] == 'P')
            {
                if(*live == 1)
                {
                    printf("YOU'VE GOT EATEN BY A MONSTER!\n");
                    map[arr[i][0]][arr[i][1]] = '.';
                    map[row][column] = 'M';
                    *game = 0;
                    return;
                }
                *live -= 1;
                map[arr[i][0]][arr[i][1]] = '.';
                map[row][column] = 'P';
                return;
            }

            // interchange monster with map[row][column]
            map[arr[i][0]][arr[i][1]] = map[row][column];
            map[row][column] = 'M';
        }
    }
}

bool check(char **map, int row, int column, int length, int width, char func)
{
    int up = moveRow(row, column, length, 'w');
    int down = moveRow(row, column, length, 's');
    int left = moveColumn(row, column, width, 'a');
    int right = moveColumn(row, column, width, 'd');

    char upCell = map[up][column];
    char downCell = map[down][column];
    char leftCell = map[row][left];
    char rightCell = map[row][right];

    if(upCell == func || downCell == func || leftCell == func || rightCell == func) return true; // do have position which is the func

    return false; // surrounding does not have func
}

bool positionCheck(char **map, int length, int width)
{
    int row, column;
    row = column = 0;
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(map[i][j] == 'P')
            {
                row = i;
                column = j;
                break;
            }
        }
    }
    bool checkEmp = check(map, row, column, length, width, '.');
    bool checkS = check(map, row, column, length, width, 'S');
    bool checkB = check(map, row, column, length, width, 'B');

    if(!checkEmp && !checkS && !checkB) return false; // no position to go! (surrounding are only 'M' / 'X')

    return true; // still having place to go
}

bool win(char **map, int length, int width)
{
    if(count(map, length, width, 'S') == 0 && count(map, length, width, 'P') == 1) return true; // win  
    return false; // do not win / not yet win
}

void arrayStoring(int *times, int point, int size, int moves, int arr[][DATA_SIZE], int *score, int level)
{
    if(*times == 3) *times = 2;

    // calculation by point weight = 3, size weight = 2, moves weight = -1
    *score = point * 3 + size * 2 + level - moves;
    if(*score >= arr[*times][GAME_SCORE])
    {
        arr[*times][GAME_POINT] = point;
        arr[*times][GAME_SIZE] = size;
        arr[*times][GAME_MOVE] = moves;
        arr[*times][GAME_SCORE] = *score;
    }
}

void winOutput(int arr[][DATA_SIZE], int times, int *recentScore)
{
    printf(".-..-..----..-..-. .-.-.-..-..-..-.\n");
    printf(" >  / | || || || | | | | || || .` |\n");
    printf(" `-'  `----'`----' `-----'`-'`-'`-'\n");
    printf("Your Score : %d\n\n", *recentScore);

    // arrange the ranking
    ranking(arr);

    printf("ranking\tpoints\tsize\tmoves\tscore\n");
    for(int i = 0; i < STORE_SIZE; i++)
    {
        printf("%d\t", i + 1);
        for(int j = 0; j < DATA_SIZE; j++)
        {
            printf("%d\t", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void ranking(int arr[][DATA_SIZE])
{
    // swap score from largest to smallest
    for(int i = 0; i < STORE_SIZE - 1; i++) // at most two times as only 3 scores
    {
        int maxIndex = i;
        for(int j = i + 1; j < STORE_SIZE; j++)
        {
            if(arr[j][3] > arr[maxIndex][3]) maxIndex = j;
        }
        swap(arr, i, maxIndex);
    }
}

void swap(int arr[][DATA_SIZE], int a, int b)
{
    for(int i = 0; i < DATA_SIZE; i++)
    {
        int temp = arr[a][i];
        arr[a][i] = arr[b][i];
        arr[b][i] = temp;
    }
}

bool gameOver(int *coins, int *game)
{
    printf("GAME OVER!\n");
    printf("Your coins: %d / 2\n", *coins);
    if(*coins > 1)
    {
        char restart;
        do
        {
            printf("Do you want to pay 2 coins and get extra live? (y / n): ");
            scanf(" %c", &restart);
        } 
        while(restart != 'y' && restart != 'n');

        if(restart == 'n') 
        {
            exitReturn();
            return true;
        }
        *coins -= 2;
        *game = 1;
    }
    else
    {
        exitReturn();
        return true;
    }
    return false;
}

bool playAgain(int *times, int *level, int *count)
{
    char restart;
    do
    {
        printf("Restart? (y / n): ");
        scanf(" %c", &restart);
    } 
    while(restart != 'y' && restart != 'n');

    if(restart == 'n') 
    {
        exitReturn();
        return false;
    }

    (*times)++;
    if(*count == *level)
    {
        *level = (*level == 3) ? 1 : *level + 1;
        *count = 0;
    }
    *count += 1;

    return true;
}

void exitReturn()
{
    printf("Enter to EXIT\n");
    int exit;
    while((exit = getchar()) != '\n' && exit != EOF);
    getchar(); // wait for user to press on enter
}

void deallocate(char **map, int length)
{
    for(int i = 0; i < length; i++) free(map[i]);
    free(map);
}
