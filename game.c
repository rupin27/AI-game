#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define GRID_ROWS 6   
#define GRID_COLS 6

typedef struct grid {
   int rows;                                    
   int cols;
   char direction[GRID_ROWS * GRID_COLS];                //holds the value for the input direction
   int playerRow;                                    
   int playerCol;
   int desiredRow;                                     //row the player/AI wanted to move towards
   int desiredCol;                                    //row the player/AI wanted to move towards
   int aiRow;
   int aiCol;
   int *playerPoints;                             //pointer to an board storing the values as the octagon is occupied
   int *aiPoints;                                //pointer to an board storing the values as the octagon is occupied
   int playerScore;                          
   int aiScore;                                
   int playerMoves;                           //tracks the location to add Player points
   int aiMoves;                              //tracks the location to add AI Points
   int *board[GRID_ROWS];;                  //main pointer grid holding all values
} makeGrid;
makeGrid gameGrid;                        

void initializeGrid() {                 //initializes the grid and the game values
    gameGrid.rows = GRID_ROWS;        
    gameGrid.cols = GRID_COLS;
    gameGrid.playerRow = -1;
    gameGrid.playerCol = -1;
    
    gameGrid.desiredRow = -1;
    gameGrid.desiredCol = -1;
    gameGrid.playerScore = 0;
    gameGrid.aiScore = 0;
    gameGrid.playerMoves = 0;
    gameGrid.aiMoves = 0;

    gameGrid.playerPoints =  malloc(sizeof(int) * GRID_ROWS * GRID_COLS);            //allocates the requested memory to playerPonts and returns a pointer to it.
    gameGrid.aiPoints = malloc(sizeof(int) * GRID_ROWS * GRID_COLS);                //allocates the requested memory to aiPoints and returns a pointer to it.

    int i, j;

    for (i = 0; i < gameGrid.rows; i++) { 
        gameGrid.board[i] = (int *) malloc (gameGrid.cols * sizeof(int));      // allocates the requested memory to the gameGrid making it 2-dimensional and returns a pointer to it.
    }
                                                                             // NOTE that arr[i][j] is same as *(*(arr+i)+j) 
   for (i = 0; i < gameGrid.rows; i++) {                                    //iterating through the rows of pointer
        for (j = 0; j < gameGrid.cols; j++) {                              //iterating through the cols of pointer board
            if (i == 0 || i == 5) {
            gameGrid.board[i][j] = 1;
            }
            else if (i == 1 || i == 4) {
                if (j == 0 || j == 5) {
                    gameGrid.board[i][j] = 1;
                }
                else {
                    gameGrid.board[i][j] = 2;
                }
            }
            else {
                if (j == 0 || j == 5) {
                    gameGrid.board[i][j] = 1;
                }
                else if(j == 1 || j == 4) {
                    gameGrid.board[i][j] = 2;
                }
                else {
                    gameGrid.board[i][j] = 3;
                }
            }
        }
   }

    if (gameGrid.board == NULL) {             //checking if malloc was declared properly without any bugs 
        printf("Bad malloc error\n");
        exit(1);                            // exit the program and indicate error
    } 
}

void setAILocation() {                  //Allocates the firstMove for AI, hence allowing the AI to start the game
    int x;
    int y;
    do {
        srand(time(NULL));          //returns random values every time a new game is placed
        x = rand() % 6;
        y = rand() % 6;
    } while (gameGrid.board[x][y] != 2 || gameGrid.board[x][y] != 0);
    gameGrid.aiRow = x;
    gameGrid.aiCol = y; 
}

void printGrid() {                                                                             //Displays the grid during gameplay
    printf("   C1    C2   C3   C4   C5   C6  \n");                                            //Column Headers
    for (int rows = 0; rows < gameGrid.rows; rows++) {
        printf("R%i", rows + 1);                                                            //Row Numbers
        for (int cols = 0; cols < gameGrid.cols; cols++) {
            if (gameGrid.playerRow == rows && gameGrid.playerCol == cols) {               //Player spot
                printf("  P  ");
            }
            else if (gameGrid.aiRow == rows && gameGrid.aiCol == cols) {               //AI spot
                printf("  A  ");
            }
            else if (gameGrid.board[rows][cols] == 0) {                             // Taken spots
                printf("  .  ");
            }
            else {                                                              //Free spots
                printf("  %i  ", gameGrid.board[rows][cols]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int min(int val1, int val2) {            
    return val1 < val2 ? val1 : val2;
}

int max(int val1, int val2) {
    return val1 > val2 ? val1 : val2;
}

bool isPositionReachable(int posRow, int posCol, int destRow, int destCol) {
    /*  Calculates the direction to be moved in in order to reach the destniation row and checks the value held by each
        octagon on the way. A blank space or a taken spot would return (fail), the edge of the board will return (fail)
        returns (true) if we reach the destination tile
    */

    int rowDirection = 0;                                       
    if (posRow > destRow) {                                    
        rowDirection = -1;
    }
    if (posRow < destRow) {
        rowDirection = 1;
    } 

    int colDirection = 0;
    if (posCol > destCol) {
        colDirection = -1;
    }
    if (posCol < destCol) {
        colDirection = 1;
    }

    while (posRow != destRow || posCol != destCol) {                           //until we reach the destination octagon
       if (posRow > 5 || posCol > 5 || posRow < 0 || posCol < 0) {           // edge of the board
            return false;
        }      

        posRow += rowDirection;
        posCol += colDirection;      

        if (gameGrid.board[posRow][posCol] == 0) {                      //gap or taken spot in the way
            return false;
        }
    }
    return true;                                                    //no errors then return true
}

void returnFinalGrid(int num) {                                  //allows the program to accept the input correctly
    int i = 0;

    gameGrid.desiredRow = gameGrid.playerRow;
    gameGrid.desiredCol = gameGrid.playerCol;
    
    if (strcmp(gameGrid.direction, "up") == 0) {            //movement when direction is UP
        gameGrid.desiredRow += (-1 * num);
    }

    else if (strcmp(gameGrid.direction, "down") == 0) {      //movement when direction is DOWN
        gameGrid.desiredRow += (1 * num);
    }

    else if (strcmp(gameGrid.direction, "left") == 0) {        //movement when direction is LEFT
        gameGrid.desiredCol += (-1 * num);
    }

    else if (strcmp(gameGrid.direction, "right") == 0) {         //movement when direction is RIGHT   
        gameGrid.desiredCol += (1 * num);
    }

    else if (strcmp(gameGrid.direction, "upLeft") == 0) {          //movement when direction is UPLEFT
        gameGrid.desiredRow += (-1 * num);
        gameGrid.desiredCol += (-1 * num);
    }

    else if (strcmp(gameGrid.direction, "upRight") == 0) {            //movement when direction is UPRIGHT
        gameGrid.desiredRow += (-1 * num);
        gameGrid.desiredCol += (1 * num);
    }

    else if (strcmp(gameGrid.direction, "downLeft") == 0) {              //movement when direction is DOWNLEFT
        gameGrid.desiredRow += (1 * num);
        gameGrid.desiredCol += (-1 * num);
    }

    else if (strcmp(gameGrid.direction, "downRight") == 0) {                //movement when direction is DOWNRIGHT
        gameGrid.desiredRow += (1 * num);
        gameGrid.desiredCol += (1 * num);
    }

    else {
        printf("Invalid direction\n");
    }
}

bool promptPlayerLocation(bool initialPos) {     //Responsible for the movement and information transfer for the player
  
    if (initialPos) {                          //Players first move
        printf("Choose your Initial Starting Position:  ");    //takes the input as two in-bound integers for the first move
       
        while (scanf("%i %i", &gameGrid.playerRow, &gameGrid.playerCol) != 2 || gameGrid.playerRow > GRID_ROWS || gameGrid.playerCol > GRID_COLS) {  //Recieves and checks if the number of inputs we're correct
            printf("Invalid Input: Please type two inbound integers:- (row: 1 - 6)(col: 1 - 6)\n");
           
            char ch;                                                                //Clears the STDIN
            while ((ch = fgetc(stdin) == EOF || ch == '\n'));
        }

        gameGrid.playerRow--;                                                   // Our input range if from 1-6 and our internal information is stored from 0-5          
        gameGrid.playerCol--;              

        if (gameGrid.board[gameGrid.playerRow][gameGrid.playerCol] != 1) {   //checks if the player starts with the octagon holding the fish value 1
            printf("Invalid start location! Starting fish value must be 1, yours was %i\n", gameGrid.board[gameGrid.playerRow][gameGrid.playerCol]);
            return false;                                                  //return false to take input again
        }

        if (gameGrid.playerRow == gameGrid.aiRow && gameGrid.playerCol == gameGrid.aiCol) {     //checks if the desired octagon is taken, doesn't has ice
            printf("AI has chosen that location! Please choose your initial position again\n");
            return false;                                                                     //return false to take input again
        }          
        return true;
    }

    else {                                                                               //Players following moves
        int desiredTimes = -1;
        printf("Next Position: ");

        while (scanf("%s %i", gameGrid.direction, &desiredTimes) != 2 || desiredTimes > GRID_ROWS) {    //takes the input in the style as required and checks if both the inputs we're submitted correctly
            printf("Invalid Input: Please type an inbound move:- (direction and iterations)\n");
    
            char ch;                                                                                 //Clears the STDIN
            while ((ch = fgetc(stdin) == EOF || ch == '\n'));
        }

        returnFinalGrid(desiredTimes);                                                           //sets the values as prompter by the player

        if (!isPositionReachable(gameGrid.playerRow, gameGrid.playerCol, gameGrid.desiredRow, gameGrid.desiredCol)) {    //checks if value in bounds
            printf("The location you specified is (%i %i) is out of reach from your position (%i %i)\n", gameGrid.desiredRow + 1, gameGrid.desiredCol +1, gameGrid.playerRow + 1, gameGrid.playerCol + 1);  
            return promptPlayerLocation(initialPos);
        }
        else if (gameGrid.board[gameGrid.desiredRow][gameGrid.desiredCol] == 0) {                                    //checks if the desired octagon is taken, doesn't has ice      
            printf("The location you specified doesn't have enough ice, you can't go there!\n");
            return promptPlayerLocation(initialPos);            
        }
        else {
            gameGrid.playerRow = gameGrid.desiredRow;                                                           //finalizes the player's row position
            gameGrid.playerCol = gameGrid.desiredCol;                                                          //finalizes the player's column position
        }
        return true;
    }
}

void calculatePoints() {                                                                                   //Adds in the values to the player and AI's score pointers and updates the total score at the end
    if (gameGrid.playerRow >= 0 && gameGrid.board[gameGrid.playerRow][gameGrid.playerCol] > 0) {          //Calculate player's points
        gameGrid.playerPoints[gameGrid.playerMoves] = gameGrid.board[gameGrid.playerRow][gameGrid.playerCol];
        gameGrid.playerScore += gameGrid.board[gameGrid.playerRow][gameGrid.playerCol];
        gameGrid.board[gameGrid.playerRow][gameGrid.playerCol] = 0; 
        gameGrid.playerMoves++;                                                                       //update variable to hold the value of the next octagon
    }

    if (gameGrid.board[gameGrid.aiRow][gameGrid.aiCol] > 0) {                                      //Calculate Ai's points
        gameGrid.aiPoints[gameGrid.aiMoves] = gameGrid.board[gameGrid.aiRow][gameGrid.aiCol];
        gameGrid.aiScore += gameGrid.board[gameGrid.aiRow][gameGrid.aiCol];
        gameGrid.board[gameGrid.aiRow][gameGrid.aiCol] = 0;
        gameGrid.aiMoves++;                                                                    //update variable to hold the value of the next octagon
    }
}

void printScore(bool printWinner) {       
    printf("Player score:");                              //Player's Score
    int i = 0;
    while (gameGrid.playerPoints[i] != 0) {             //print all numbers from the playerPoints pointer 
        printf(" %i ", gameGrid.playerPoints[i]);      // NOTE that arr[i] is same as *(*(arr+i)) 
        if (gameGrid.playerPoints[i+1] != 0) {
            printf("+");                             //print + until we have more numbers ahead
        }
        else {
            printf("= ");                         //if no more numbers, print = followed by the final score
        }
        i++;
    }
    printf("%i in %i moves\n", gameGrid.playerScore, gameGrid.playerMoves);       //final output explaining the game for player

    printf("AI score:");                                                        //AI's Score
    i = 0;
    while (gameGrid.aiPoints[i] != 0) {                                       //print all numbers from the AIPoints pointer 
        printf(" %i ", gameGrid.aiPoints[i]);
        if (gameGrid.aiPoints[i+1] != 0) {
            printf("+");
        }
        else {
            printf("= ");
        }
        i++;
    }
    printf("%i in %i moves\n", gameGrid.aiScore, gameGrid.aiMoves);  //Final output explaining the game for the AI
    
    if (printWinner) {                                            //In case if player score and ai score ties
        if (gameGrid.aiScore == gameGrid.playerScore) {
            printf("It's a tie");
        }
        else {
            printf("%s wins!\n", gameGrid.playerScore > gameGrid.aiScore ? "Player" : "AI");   //Final output concluding the game
        }
    }
}

void moveAI() {                                                                           //configure AI movement
    int largestPointVal = 0;
    int largestRow = -1;
    int largestCol = -1;
    for (int row = 0; row <= 5; row++) {                      //possible rows that can be reached
        for (int col = 0; col <= 5; col++) {              //possible columns that can be reached
            if (gameGrid.board[row][col] > largestPointVal && isPositionReachable(gameGrid.aiRow, gameGrid.aiCol, row, col)) {
                largestRow = row;                                           //assing values 
                largestCol = col; 
                largestPointVal = gameGrid.board[row][col];                                    
            } 
        }
    }

    if (largestRow != -1 && largestCol != -1) {                              //only update if new value is found
        gameGrid.board[gameGrid.aiRow][gameGrid.aiCol] = 0;                 //update the value
        gameGrid.aiRow = largestRow;                                       //chanage aiRow
        gameGrid.aiCol = largestCol;                                      //chnage aiCol
    }
}

bool playerHasValidMoves(int posRow, int posCol) {                                           //Checks if the Player/AI still has open octagons around it
    for (int row = max(0, posRow - 1); row <= min(5, posRow + 1); row++) {
        for (int col = max(0, posCol - 1); col <= min(5, posCol + 1); col++) {
            if(gameGrid.board[row][col] > 0) {
                return true;
            }
        }
    }
    return false;
}

void enterMainGameLoop() {
    while(true) {
        if (!playerHasValidMoves(gameGrid.aiRow, gameGrid.aiCol) && !playerHasValidMoves(gameGrid.playerRow, gameGrid.playerCol)) {  //if plyaer dosen't have any moves left, break the loop
            printf("No more moves to play!\n");
            return;
        }
        if (playerHasValidMoves(gameGrid.playerRow, gameGrid.playerCol)) {                                                        //if player still has moves left, keep asking for input
            printf("Your move...\n");
            printGrid();                                                                                                        //print grid to show player status of gameboard
            promptPlayerLocation(false);                                                                                       //keep asking and updating location until no more moves left
            calculatePoints();                                                                                                //adding in the values to pointer sotring the score and upating totoal score
        }
        else {
            printf("You have no valid moves! Skipping turn...\n");                                                         //when run out of moves, skip players turn and let AI exhaust
        }

        if (playerHasValidMoves(gameGrid.aiRow, gameGrid.aiCol)) {                                                      //keep prompting AI
            printf("AI move...\n");
            printGrid();                                                                                              //print grid to show AI's move and the updated gameboard
            moveAI();
            calculatePoints();                                                                                      //adding in the values to pointer sotring the score and upating totoal score
        }
        else {
            printf("AI has no valid moves! Skipping turn...\n");                                                 //skip AI's turn if no mmoves left
        }        
    }
}
    
int main() {
    printf("**************************************************************************\n\n");
    printf("Starting 'Hey, where's my fish?'\n\nAI Moved first and wished you luck!\n\n");                 //AI moved first
    initializeGrid();                                                                                     
    calculatePoints();                                                                                   //since AI went first, we update the score and mark that location as 0 so that the player can't choose it
    while (!promptPlayerLocation(true)) {};                                                             //get Player starting position until no errors
    calculatePoints();
    printf("Possible directions are: 1. up  2. down  3. left 4. right 5. upLeft  6. upRight 7. downLeft  8.downRight\n");
    enterMainGameLoop();                                                                              //start game

    printf("\nGAME OVER\nFinal Grid:\n");                                                           //Print out the final status
    printGrid();
    printf("FINAL SCORES: \n");
    printScore(true);
    printf("**************************************************************************\n\n");
    return 0;
}