// Including headers
# include <iostream>

// Variables
const int numPadLookup[9] = {           // When the input is a valid number
    6, 7, 8,
    3, 4, 5,
    0, 1, 2
};

const int keyBoardLookup[9][3] = {      // When the input is a valid letter
    {81, 113, 0},
    {87, 119, 1},
    {69, 101, 2},
    {65, 97 , 3},
    {83, 115, 4},
    {68, 100, 5},
    {90, 122, 6},
    {88, 120, 7},
    {67, 99 , 8}
};

const char displayLookup[4][2] = {      // The char to be displayed and it's color
    {' ', 7},
    {'-', 1},
    {'|', 4},
    {'T', 5}
};

const char horizontalGridLine[13] = {10, -60, -60, -60, -59, -60, -60, -60, -59, -60, -60, -60, 0};
const char verticalGridLine[4]    = {32, -77, 32, 0};

char board[3][3] = {0};          // 0 - empty space, 1 - horizontal, 2 - vertical, 3 - fully occupied
char currentTurn  = 1;           // 1 is player red/horizontal, 2 is player blue/vertical
int  currentTurns = 0;           // Turns elapsed

// Function declaration
void printCharWithColor(char ch, int color);
void printIntro(void);
int getPosition(char ch);
void drawBoard(void);
int checkForWin(void);

/*----------------Function code----------------*/

// Main function
int main (void) {
    printIntro();

    int row, col,
        prevRow = -1, prevCol = -1;
    char in_;

    drawBoard();

    // Game loop
    while (currentTurns < 18) {
        // Print out current turn
        if (currentTurn == 1) std::printf("\e[0;31m<Red's/horizontal's turn (turn: %d)>\e[0m\n", currentTurns+1);
        else std::printf("\e[0;34m<Blue's/vertical's turn (turn: %d)>\e[0m\n", currentTurns+1);

        // Print prompt and take user input
        std::cout << "\e[0;32mEnter position> \e[0m";
        std::fflush(stdin);
        in_ = getPosition( std::getchar() );

        // Re-loop if given an invalid input
        if (in_ < 0) {
            std::cout << "Invalid position\n\n";
            continue;
        }

        // Get the row and column of the position selected
        row = in_ / 3;
        col = in_ % 3;

        // So that player 2 doesn't always win
        if (prevRow == row && prevCol == col) {
            std::cout << "Cannot mark a line in the position that was just occupied in the previous turn\n\n";
            continue;
        }
        
        // Setting new values for the previous position picked
        prevRow = row;
        prevCol = col;

        // If space if fully occupied or occupied by the current player then re-loop
        if (board[row][col] == 3 || board[row][col] == currentTurn) {
            std::cout << "Place already occupied by you\n\n";
            continue;
        }

        // If the position entered passes all these checks then place the character there
        else board[row][col] += currentTurn;

        drawBoard();

        // If the game has ended then the player who played last is the winner
        if (checkForWin()) {
            if (currentTurn == 1) {
                std::cout << "\n\e[0;31mRED WON!\e[0m\n";
            }
            else if (currentTurn == 2) {
                std::cout << "\n\e[0;34mBLUE WON!\e[0m\n";
            }
            
            break;
        }

        currentTurns++;
        currentTurn = currentTurns % 2 + 1;
    }

    std::system("echo Press any key to exit&&pause > nul");

    return 0;
}

void printCharWithColor(char ch, int color) {
    std::printf("\e[0;3%dm%c\e[0m", color, ch);
}

void printIntro(void) {
    std::cout << "\e[1;97mWelcome to Tick oaT Two\e[0m\n";
    std::cout << "Game creation and rules: \e[4;37mhttps://www.youtube.com/watch?v=ePxrVU4M9uA&ab_channel=OatsJenkins\e[0m\n\n";

    std::cout << "The spaces on the board work as the following:\n";
    std::cout << "Q W E \t\t 7 8 9\n";
    std::cout << "A S D \t\t 4 5 6\n";
    std::cout << "Z X C \t\t 1 2 3\n\n";

    std::cout << "Any one method or both can be used at the same time\n\n";

    std::cout << "\e[0;36m[START GAME]\e[0m\n\n";
}

int getPosition(char ch) {
    // If using the numpad
    if (ch > 47 && ch < 58) {
        return numPadLookup[ch - 49];
    }

    // If using the keyboard
    for (int i = 0; i < 9; i++) {
        if (ch == keyBoardLookup[i][0] || ch == keyBoardLookup[i][1]) {
            return keyBoardLookup[i][2];
        }
    }

    // If useless
    return -1;
}

void drawBoard(void) {
    std::putchar('\n');
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (j == 0) std::putchar(' ');

            printCharWithColor( displayLookup[ board[i][j] ][0], displayLookup[ board[i][j] ][1] );

            if (j < 2) std::printf(verticalGridLine);
        }
        if (i < 2) std::printf(horizontalGridLine);
        std::putchar('\n');
    }
    std::putchar('\n');
}

int checkForWin(void) {
    // Horizontal
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == 3 && board[row][1] == 3 && board[row][2] == 3) {
            return currentTurn;
        }
    }

    // Verical
    for (int col = 0; col < 3; col++) {
        if (board[0][col] == 3 && board[1][col] == 3 && board[2][col] == 3) {
            return currentTurn;
        }
    }

    // Diagnols
    if (board[0][0] == 3 && board[1][1] == 3 && board[2][2] == 3) return currentTurn;
    if (board[2][0] == 3 && board[1][1] == 3 && board[0][2] == 3) return currentTurn;
    return 0;
}