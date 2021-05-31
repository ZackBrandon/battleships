/**
 * @brief Jethro AI for battleships
 * @file Jethro.cpp
 * @author Zack Brandon
 * @date May 3, 2021
 */
//Make a showBoard function to compartmentalize the code into
//Remove the unessesary stuff (clean it up)
//After the show function has been compartmentalized, break it down into more than just four sections and see how much of an improvement that is on performace against the other people
#include <iostream>
#include <cstdio>
#include "conio.h"
#include "Jethro.h"
using namespace conio;
int roundNumber = 0;
int statsBoard[10][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
};
int averages[10][10] = {
    {3,3,3,3,0,0,3,3,3,3},
    {3,0,0,0,0,0,0,0,0,3},
    {3,0,0,0,0,0,0,0,0,3},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,3},
    {0,0,0,0,3,0,0,0,0,3},
    {3,3,3,3,3,0,0,3,3,3}
};
float opponentShipPlacements[10][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
};


float levelizer[10][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
};
int shipsLeft[6] = {3,4};
bool DISPLAY_BOARD = false;

//NOTES FOR TOMMORROW:
// (1) Include CONIO so that I can see the heatmap being made in real time. 
// (2) Figure out what to do with the heat map and probabilities
/**
 * @brief Constructor that initializes any inter-round data structures.
 * @param boardSize Indication of the size of the board that is in use.
 *
 * The constructor runs when the AI is instantiated (the object gets created)
 * and is responsible for initializing everything that needs to be initialized
 * before any of the rounds happen. The constructor does not get called 
 * before rounds; newRound() gets called before every round.
 */
Jethro::Jethro( int boardSize )
    :PlayerV2(boardSize)
{
    // Could do any initialization of inter-round data structures here.
}

/**
 * @brief Destructor placeholder.
 * If your code does anything that requires cleanup when the object is
 * destroyed, do it here in the destructor.
 */
Jethro::~Jethro( ) {}

/*
 * Private internal function that initializes a MAX_BOARD_SIZE 2D array of char to water.
 */
void Jethro::initializeBoard() {
    for(int row=0; row<boardSize; row++) {
        for(int col=0; col<boardSize; col++) {
            this->board[row][col] = WATER;
        }
    }
}


/**
 * @brief Specifies the AI's shot choice and returns the information to the caller.
 * @return Message The most important parts of the returned message are 
 * the row and column values. 
 *
 * See the Message class documentation for more information on the 
 * Message constructor.
 */
Message Jethro::getMove() {
    //shotRow = The row you are going to shoot THIS turn 
    //ShotCol = The col you are going to shoot THIS turn
    //lastRow = The row you shot LAST turn
    //lastCol = The col you shot LAST turn
    //scanRow = initially set to shotRow, saves the shotRow value while in functions
    //scanCol = initially set to shotCol, saves the shotCol value while in functions
    //Is this the first shot of the round?
    //If it is the first shot of the round,

    if (firstShot) {
        //Then set everything to zero.
        shotRow = 5;
        shotCol = 5;
        scanRow = shotRow;
        scanCol = shotCol;
        firstShot = false;
        //If it is not the first shot,
    } else {
        if (board[shotRow][shotCol] == HIT || board[scanRow][scanCol] == KILL) {
            if (board[shotRow][shotCol] == KILL) {
                currentShipLength += 1;
                //remove it from the list of ships (by setting it to zero);
                for (int i = 0; i < 6; i++) {
                    if (shipsLeft[i] == currentShipLength) {
                        shipsLeft[i] = 0;
                        break;
                    }
                }
                currentShipLength = 0;
                if (DISPLAY_BOARD) {
                    updateKillDisplay();

                }
            }
            if (board[shotRow][shotCol] == HIT) {
                currentShipLength += 1;
            }
            opponentShipPlacements[shotRow][shotCol] += 1;
            updateOpponentShipBoard();
        }
        //If you got a hit at scanRow, scanCol the last time:
        if (board[scanRow][scanCol] == HIT) {
            //Use the killShip() function to set shotRow and shotCol to the next part of the  ship!
            killShip(scanRow, scanCol, shotRow, shotCol);
            //^^ scanRow and scanCol by value, shotRow and shotCol by reference
            //findShot(scanRow, scanCol);
            //shotRow = scanRow;
            //shotCol = scanCol;
            //If you did not get a hit,
        } else {
            //Find a shot / continue scanning
            findShot(scanRow, scanCol);
            //^^ By reference
            //Update our shot variables to shoot
            shotRow = scanRow;
            shotCol = scanCol;

            hitOnBoard(scanRow, scanCol, shotRow, shotCol);
        }
    }
    lastRow = shotRow;
    lastCol = shotCol;
    Message result(SHOT, shotRow, shotCol, "Bang", None, 1);
    return result;
}

void Jethro::updateKillDisplay() {
    cout << gotoRowCol(5,60) << "Three Segment: " << shipsLeft[0];
    cout << gotoRowCol(6,60) << "Three Segment: " << shipsLeft[1];
    cout << gotoRowCol(7,60) << "Three Segment: " << shipsLeft[2];
    cout << gotoRowCol(8,60) << "Four Segment: " << shipsLeft[3];
    cout << gotoRowCol(9,60) << "Four Segment: " << shipsLeft[4];
    cout << gotoRowCol(10,60) << "Five Segment: " << shipsLeft[5];

}

void Jethro::updateOpponentShipBoard() {
    int min = 10000;
    int lowerMid;
    int middle;
    int upperMid;
    int max = 0;
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (opponentShipPlacements[row][col] > max) {
                max = opponentShipPlacements[row][col];
            }
            if (opponentShipPlacements[row][col] < min) {
                min = opponentShipPlacements[row][col];
            }
        }
    }
    middle = (min + max) / 2;
    lowerMid = (middle + min) /2;
    upperMid = (middle + max) /2;
    //Display the results
    if (DISPLAY_BOARD) {
        cout << gotoRowCol(5,90);
        cout << "Min: " << min;
        cout << gotoRowCol(6, 90);
        cout << "lowerMid: " << lowerMid;
        cout << gotoRowCol(7,90);
        cout << "Middle: " << middle;
        cout << gotoRowCol(8,90);
        cout << "upperMid: " << upperMid;
        cout << gotoRowCol(9, 90);
        cout << "Max: " << max;
    } 
    //Update the color
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (opponentShipPlacements[row][col] <= lowerMid) {
                if (DISPLAY_BOARD) cout << bgColor(LIGHT_RED);
                //averages[row][col] = 0;
            } else if (opponentShipPlacements[row][col] > lowerMid && opponentShipPlacements[row][col] <= middle) {
                if (DISPLAY_BOARD) cout << bgColor(LIGHT_YELLOW);
                //averages[row][col] = 1;
            } else if (opponentShipPlacements[row][col] > middle && opponentShipPlacements[row][col] <= upperMid) {
                if (DISPLAY_BOARD) cout << bgColor(GREEN);
                //averages[row][col] = 2;
            } else if (opponentShipPlacements[row][col] > upperMid && opponentShipPlacements[row][col] <= max) {
                if (DISPLAY_BOARD) cout << bgColor(LIGHT_GREEN);
                //averages[row][col] = 3;
            }
            if (DISPLAY_BOARD) cout << gotoRowCol(row+5,col+80) << opponentShipPlacements[row][col];
        }
    }
    cout << resetAll() << flush;

}
void Jethro::hitOnBoard(int& scanRow, int& scanCol, int& shotRow, int& shotCol) {
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (board[row][col] == HIT) {
                scanRow = row;
                scanCol = col;
                killShip(scanRow, scanCol, shotRow, shotCol);
            }
        }
    }
}

void Jethro::showStatsBoard() {
    //Find the lowest and the highest number in our shotMap
    int min = 10000;
    int lowerMid;
    int middle;
    int upperMid;
    int max = 0;
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (statsBoard[row][col] > max) {
                max = statsBoard[row][col];
            }
            if (statsBoard[row][col] < min) {
                min = statsBoard[row][col];
            }
        }
    }
    middle = (min + max) / 2;
    lowerMid = (middle + min) /2;
    upperMid = (middle + max) /2;
    //Display the results
    if (DISPLAY_BOARD) {
        cout << gotoRowCol(5,110);
        cout << "Min: " << min;
        cout << gotoRowCol(6, 110);
        cout << "lowerMid: " << lowerMid;
        cout << gotoRowCol(7,110);
        cout << "Middle: " << middle;
        cout << gotoRowCol(8,110);
        cout << "upperMid: " << upperMid;
        cout << gotoRowCol(9, 110);
        cout << "Max: " << max;
    } 
    //Update the color
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (statsBoard[row][col] <= lowerMid) {
                if (DISPLAY_BOARD) cout << bgColor(LIGHT_RED);
                //averages[row][col] = 0;
            } else if (statsBoard[row][col] > lowerMid && statsBoard[row][col] <= middle) {
                if (DISPLAY_BOARD) cout << bgColor(LIGHT_YELLOW);
                //averages[row][col] = 1;
            } else if (statsBoard[row][col] > middle && statsBoard[row][col] <= upperMid) {
                if (DISPLAY_BOARD) cout << bgColor(GREEN);
                //averages[row][col] = 2;
            } else if (statsBoard[row][col] > upperMid && statsBoard[row][col] <= max) {
                if (DISPLAY_BOARD) cout << bgColor(LIGHT_GREEN);
                //averages[row][col] = 3;
            } else if (statsBoard[row][col] == max) {
                //if (DISPLAY_BOARD) cout << bgColor(WHITE);
            }
            if (DISPLAY_BOARD) cout << gotoRowCol(row+5,col+100) << " ";
        }
    }
    cout << resetAll() << flush;
}

int loop = false;
void Jethro::goToNextShot(int& scanRow, int& scanCol) {
    int bestValue = 0;
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            statsBoard[row][col] = 0;
            if (opponentShipPlacements[row][col] > bestValue) {
                bestValue = opponentShipPlacements[row][col];
            }
        }
    }
    if (DISPLAY_BOARD) {
        cout << gotoRowCol(20, 90) << bestValue << " << most ships placed in one location";
        cout << gotoRowCol(21, 90) << roundNumber << " << Round Number";
    }
    int cutoffValue = 100;
    if (roundNumber == cutoffValue && bestValue == cutoffValue) {
        loop = false;
    }
    int bestRow = 0;
    int bestCol = 0;
    if (loop) { 
        //if the above is true, that means that wherever in opponentShipPlacements the value equals the
        //above number, the player is ALWAYS placing a ship there, and I should skyrocket
        //the values in statsBoard at all of those locations
        cout << gotoRowCol(22,90) << "Yes!";
        for (int row = 0; row < boardSize; row++) {
            for (int col = 0; col < boardSize; col++) {
                if (opponentShipPlacements[row][col] == cutoffValue) {
                    //   statsBoard[row][col] = 100000;
                }
            }
        }
        
        int currentBestShot = 0;
        for(static bool first = true;first;first=false){
            for (int row = 0; row < boardSize; row++) {
                for (int col = 0; col < boardSize; col++) {
                    int random = rand()%10000;
                    opponentShipPlacements[row][col] += random;
                }
            }      
        }
        for (int row = 0; row < boardSize; row++) {
            for (int col = 0; col < boardSize; col++) {
                if (opponentShipPlacements[row][col] > currentBestShot) {
                    currentBestShot = opponentShipPlacements[row][col];
                    bestRow = row;
                    bestCol = col;
                }
            }
        }
        cout << gotoRowCol(22, 90) << "Yes!";
    } else {
        Direction directions[] = {Horizontal, Vertical};
        //for every row in the board:
        for (int row = 0; row < boardSize; row++) {
            //for every col in the board:
            for (int col = 0; col < boardSize; col++) {
                //for every direction in Directions:
                for (Direction dir : directions) {
                    //for every ship left to hit:
                    for (int length : shipsLeft) {
                        //if I can place a ship there:
                        //I need to write a NEW can placeship function!
                        if (ifCanPlaceShip(row, col, dir, length)) {
                            //update statsBoard with the location that the ship can be placed.
                            if (dir == Horizontal) {
                                for (int c = col; c < col+length; c++) {
                                    statsBoard[row][c] += 1;
                                }
                            } else {
                                for (int r = row; r < row+length; r++) {
                                    statsBoard[r][col] += 1;
                                }
                            }   
                        }
                    }
                }
            }
        }
        int currentBestShot = 0;

        for (int row = 0; row < boardSize; row++) {
            for (int col = 0; col < boardSize; col++) {
                if (statsBoard[row][col] > currentBestShot) {
                    currentBestShot = statsBoard[row][col];
                    bestRow = row;
                    bestCol = col;
                }
            }
        }
    }


    updateOpponentShipBoard();
    showStatsBoard();
    //showLevelizerBoard();
    // cout << bgColor(WHITE);
    //cout << gotoRowCol(bestRow + 5, bestCol + 100) << " ";
    //cout << bgColor(RESET);
    if (scanRow == bestRow && scanCol == bestCol) {
        scanRow = rand()%boardSize;
        scanCol = rand()%boardSize;
        return;
    }
    scanRow = bestRow;
    scanCol = bestCol;
    if (scanRow == 0 && scanCol == 0) {
        scanRow = rand()%boardSize;
        scanCol = rand()%boardSize;
    }
}

void Jethro::showLevelizerBoard() {
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            levelizer[row][col] = (statsBoard[row][col] + opponentShipPlacements[row][col]) * 0.1;
        }
    }
    int min = levelizer[0][0];
    int lowerMid;
    int middle;
    int upperMid;
    int max = levelizer[0][0];
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (levelizer[row][col] > max) {
                max = levelizer[row][col];
            }
            if (levelizer[row][col] < min) {
                min = levelizer[row][col];
            }
        }
    }
    middle = (min + max) / 2;
    lowerMid = (middle + min) /2;
    upperMid = (middle + max) /2;
    //Display the results
    if (DISPLAY_BOARD) {
        cout << gotoRowCol(5,30);
        cout << "Min: " << min;
        cout << gotoRowCol(6, 30);
        cout << "lowerMid: " << lowerMid;
        cout << gotoRowCol(7,30);
        cout << "Middle: " << middle;
        cout << gotoRowCol(8,30);
        cout << "upperMid: " << upperMid;
        cout << gotoRowCol(9, 30);
        cout << "Max: " << max;
    } 
    //Update the color
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (levelizer[row][col] <= lowerMid) {
                if (DISPLAY_BOARD) cout << bgColor(LIGHT_RED);
                //averages[row][col] = 0;
            } else if (levelizer[row][col] > lowerMid && levelizer[row][col] <= middle) {
                if (DISPLAY_BOARD) cout << bgColor(LIGHT_YELLOW);
                //averages[row][col] = 1;
            } else if (levelizer[row][col] > middle && levelizer[row][col] <= upperMid) {
                if (DISPLAY_BOARD) cout << bgColor(GREEN);
                //averages[row][col] = 2;
            } else if (levelizer[row][col] > upperMid && levelizer[row][col] <= max) {
                if (DISPLAY_BOARD) cout << bgColor(LIGHT_GREEN);
                //averages[row][col] = 3;
            }
            if (DISPLAY_BOARD) cout << gotoRowCol(row+20,col+90) << " ";
        }
    }
    cout << resetAll() << flush;
}
void Jethro::findShot(int& scanRow, int& scanCol) {
    //goToNextShot(scanRow, scanCol);
    if (board[scanRow][scanCol] == MISS) {
        //Increments scanRow and scanCol to the next place in the algorithm (defined in goToNextShot())
        goToNextShot(scanRow, scanCol);
        findShot(scanRow, scanCol);
    } else if (board[scanRow][scanCol] == KILL) {
        goToNextShot(scanRow, scanCol);
        findShot(scanRow, scanCol);
    } else if (board[scanRow][scanCol] == HIT) {
        goToNextShot(scanRow, scanCol);
        findShot(scanRow, scanCol);
        return;
    } else if (board[scanRow][scanCol] == WATER) {
        return;
    }
}

bool Jethro::killShipNorth(int scanRow, int scanCol, int& shotRow, int& shotCol) {
    for (int r = scanRow - 1; r >= 0; r--) {
        switch (board[r][scanCol]) {
            case WATER:
                shotRow = r;
                shotCol = scanCol;
                return true;
            case HIT:
                break;
            case KILL:
                return false;
            case MISS:
                return false;
            default:
                return false;
        }
    }
    return false;
}


bool Jethro::killShipSouth(int scanRow, int scanCol, int& shotRow, int& shotCol) {
    for (int r = scanRow + 1; r <= boardSize - 1; r++) {
        switch (board[r][scanCol]) {
            case WATER:
                shotRow = r;
                shotCol = scanCol;
                return true;
            case HIT:
                break;
            case KILL:
                return false;
            case MISS:
                return false;
            default:
                return false;
        }
    }
    return false;
}

bool Jethro::killShipEast(int scanRow, int scanCol, int& shotRow, int& shotCol) {

    for (int c = scanCol + 1; c <= boardSize - 1; c++) {
        switch (board[scanRow][c]) {
            case WATER:
                shotRow = scanRow;
                shotCol = c;
                return true;
            case HIT:
                break;
            case KILL:
                return false;
            case MISS:
                return false;
            default:
                return false;
        }
    }
    return false;
}


bool Jethro::killShipWest(int scanRow, int scanCol, int& shotRow, int& shotCol) {

    for (int c = scanCol - 1; c >= 0; c--) {
        switch (board[scanRow][c]) {
            case WATER:
                shotRow = scanRow;
                shotCol = c;
                return true;
            case HIT:
                break;
            case KILL:
                return false;
            case MISS:
                return false;
            default:
                return false;
        }
    }
    return false;
}

void Jethro::killShip(int scanRow, int scanCol, int& shotRow, int& shotCol) {
    if (killShipEast(scanRow, scanCol, shotRow, shotCol)) {
        return;
    } else if (killShipNorth(scanRow, scanCol, shotRow, shotCol)) {
        return;
    } else if (killShipSouth(scanRow, scanCol, shotRow, shotCol)) {
        return;
    } else if (killShipWest(scanRow, scanCol, shotRow, shotCol)) {
        return;
    }

    /*
    //Loop through all of the places between the current scanRow (where there was a hit) and the edge of the board (in the negative row direction)
    for (int r = scanRow - 1; r >= 0; r--) {
    switch (board[r][scanCol]) {
    case WATER:
    shotRow = r;
    shotCol = scanCol;
    return;
    break;
    case HIT:
    break;
    case KILL:

    case MISS:
    }

    }
     */
}
/**
 * @brief Tells the AI that a new round is beginning.
 * The AI show reinitialize any intra-round data structures.
 */
void Jethro::newRound() {
    /* Jethro is too simple to do any inter-round learning. Smarter players 
     * reinitialize any round-specific data structures here.
     */
    roundNumber++;
    this->firstShot = true;
    shipsLeft[0] = 3;
    shipsLeft[1] = 3;
    shipsLeft[2] = 3;
    shipsLeft[3] = 4;
    shipsLeft[4] = 4;
    shipsLeft[5] = 5;
    this->lastRow = 0;
    this->lastCol = 0;
    this->numShipsPlaced = 0;
    for (int row = 0; row < MAX_BOARD_SIZE; row++) {
        for (int col = 0; col < MAX_BOARD_SIZE; col++) {
            ships[row][col] = WATER;
        }
    }
    this->initializeBoard();
}


/**
 * @brief Gets the AI's ship placement choice. This is then returned to the caller.
 * @param length The length of the ship to be placed.
 * @return Message The most important parts of the returned message are 
 * the direction, row, and column values. 
 *
 * The parameters returned via the message are:
 * 1. the operation: must be PLACE_SHIP 
 * 2. ship top row value
 * 3. ship top col value
 * 4. a string for the ship name
 * 5. direction Horizontal/Vertical (see defines.h)
 * 6. ship length (should match the length passed to placeShip)
 */


bool Jethro::ifCanPlaceShip(int row, int col, Direction dir, int length) {
    if (row < 0 || col < 0 || row > boardSize || col > boardSize) {
        return false;
    }
    //if the direction is horizontal
    if (dir == Horizontal) {
        if (col > boardSize-length) {
            return false;
        }
        for (int c = col; c < col+length; c++) {
            if (board[row][c] == MISS || board[row][c] == KILL) {
                return false;
            }
        }
    } else if (dir == Vertical) {
        if (row > boardSize - length) {
            return false;
        }
        for (int r = row; r < row+length; r++) {
            if (board[r][col] == MISS || board[r][col] == KILL) {
                return false;
            }
        }
    }
    return true;
}

bool Jethro::canPlaceShip(int row, int col, Direction dir, int length) {
    if (row < 0 || col < 0 || row > boardSize || col > boardSize) {
        return false;
    }
    //If the direction is horizontal
    if (dir == Horizontal) {
        //If the row is outside board or if it is impossible to place a ship:
        if (col > boardSize-length) {
            return false;
        }
        for (int c = col; c < col+length; c++) {
            if (ships[row][c] == SHIP) {
                return false;
            }
        }
    } else if (dir == Vertical) {
        if (row > boardSize-length) {
            return false;
        }
        for (int r = row; r < row+length; r++) {
            if (ships[r][col] == SHIP) {
                return false;
            }
        }
    }

    return true;
}

void Jethro::markShip(int row, int col, Direction dir, int length) {
    if (dir == Horizontal) {
        for (int c = col; c < col+length; c++) {
            ships[row][c] = SHIP;

        }
    } else {
        for (int r = row; r < row+length; r++) {
            ships[r][col] = SHIP;
        }
    }
}


Message Jethro::placeShip(int length) {
    /*
    //Ship 1 (length 3)
    if (length == 3 && ship1NotPlaced) {
    ship1NotPlaced = false;
    Message ship(PLACE_SHIP, boardSize, 0, "", Horizontal, 3);
    return ship;
    }
    //Ship 2 (length 3)
    if (length == 3 && ship2NotPlaced) {
    ship2NotPlaced = false;
    Message ship(PLACE_SHIP, boardSize, boardSize - 3, "", Horizontal, 3);
    return ship;
    }
    //Ship 3 (length 3)
    if (length == 3 && ship3NotPlaced) {
    ship3NotPlaced = false;
    Message ship(PLACE_SHIP, 4, 3, "", Vertical, 3);
    return ship;
    }
    //Ship 4 (length 4)
    if (length == 4 && ship4NotPlaced) {
    ship4NotPlaced = false;
    Message ship(PLACE_SHIP, boardSize, boardSize - 6, "", Vertical, 4);
    return ship;
    }
    //Ship 5 (length 4)
    if (length == 4 && ship5NotPlaced) {
    ship5NotPlaced = false;
    Message ship(PLACE_SHIP, 0, 0, "", Vertical, 4);
    return ship;
    }
    //Ship 6
    if (length == 5 && ship6NotPlaced) {
    ship6NotPlaced = false; 
    Message ship(PLACE_SHIP, 0, 5, "", Horizontal, 6);
    return ship;
    }
     */
    // /*
    /*
       bool isFirst;
       if (length == 3 && isFirst) {
       if (canPlaceShip(boardSize, 0, Horizontal, 3)) {
       isFirst = false;
       markShip(boardSize, 0, Horizontal, 3);
       Message ship(PLACE_SHIP, boardSize, 0, "", Horizontal, 3);
       return ship;
       }
       }
     */
    int bestRow = 0;
    int bestCol = 0;
    float lowestLocationScore = 100000;
    Direction bestDir = Horizontal;
    for (int j = 0; j < 10; j++) {

    }
    for (int i = 0; i < 1000; i++) {
        int row = rand()%boardSize;
        int col = rand()%boardSize;
        Direction dir = Direction(rand()%2+1);
        if (canPlaceShip(row, col, dir, length)) {
            //Find the average of the ships in the area.
            float locationScore = 0;
            if (dir == Horizontal) {
                for (int c = col; c < col+length; c++) {
                    //locationScore += averages[row][c];
                    locationScore += opponentShotMap[row][c];
                }
            } else {
                for (int r = row; r < row+length; r++) {
                    //locationScore += averages[r][col];
                    locationScore += opponentShotMap[r][col];
                }
            }
            locationScore = locationScore / length;
            if (locationScore < lowestLocationScore) {
                lowestLocationScore = locationScore;
                bestRow = row;
                bestCol = col;
                bestDir = dir;
            }
        }
    }

    //We want to mark the ship after I choose a row and a col
    markShip(bestRow, bestCol, bestDir, length);
    Message ship(PLACE_SHIP, bestRow, bestCol, "", bestDir, length);
    return ship;
    /*
       while(true) {
       int row = rand()%boardSize;
       int col = rand()%boardSize;
       Direction dir = Direction(rand()%2+1);
       if (canPlaceShip(row, col, dir, length)) {
       markShip(row, col, dir, length);
       Message ship(PLACE_SHIP, row, col, "", dir, length);
       return ship;
       }
       }
     */
    /*
    // */
    //OLD CODE BELOW
    /*
       char shipName[10];
    // Create ship names each time called: Ship0, Ship1, Ship2, ...
    snprintf(shipName, sizeof shipName, "Ship%d", numShipsPlaced);

    // parameters = mesg type (PLACE_SHIP), row, col, a string, direction (Horizontal/Vertical)
    Message response( PLACE_SHIP, numShipsPlaced, 0, shipName, Horizontal, length );
    numShipsPlaced++;

    return response;
     */
}

void Jethro::displayShotMap() {
    //Find the lowest and the highest number in our shotMap
    int min = opponentShotMap[0][0];
    int lowerMid;
    int middle;
    int upperMid;
    int max = opponentShotMap[0][0];
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (opponentShotMap[row][col] > max) {
                max = opponentShotMap[row][col];
            }
            if (opponentShotMap[row][col] < min) {
                min = opponentShotMap[row][col];
            }
        }
    }
    middle = (min + max) / 2;
    lowerMid = (middle + min) /2;
    upperMid = (middle + max) /2;
    //Display the results
    if (DISPLAY_BOARD) {
        cout << gotoRowCol(5,30);
        cout << "Min: " << min;
        cout << gotoRowCol(6, 30);
        cout << "lowerMid: " << lowerMid;
        cout << gotoRowCol(7,30);
        cout << "Middle: " << middle;
        cout << gotoRowCol(8,30);
        cout << "upperMid: " << upperMid;
        cout << gotoRowCol(9, 30);
        cout << "Max: " << max;
    } 
    //Update the color
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (opponentShotMap[row][col] <= lowerMid) {
                if (DISPLAY_BOARD) cout << bgColor(LIGHT_RED);
                averages[row][col] = 0;
            } else if (opponentShotMap[row][col] > lowerMid && opponentShotMap[row][col] <= middle) {
                if (DISPLAY_BOARD) cout << bgColor(LIGHT_YELLOW);
                averages[row][col] = 1;
            } else if (opponentShotMap[row][col] > middle && opponentShotMap[row][col] <= upperMid) {
                if (DISPLAY_BOARD) cout << bgColor(GREEN);
                averages[row][col] = 2;
            } else if (opponentShotMap[row][col] > upperMid && opponentShotMap[row][col] <= max) {
                if (DISPLAY_BOARD) cout << bgColor(LIGHT_GREEN);
                averages[row][col] = 3;
            }
            if (DISPLAY_BOARD) cout << gotoRowCol(row+5,col+20) << averages[row][col];
        }
    }
    //cout << gotoRowCol(4, 17) << "Shot Map:";
    cout << resetAll() << flush;

}
/**
 * @brief Updates the AI with the results of its shots and where the opponent is shooting.
 * @param msg Message specifying what happened + row/col as appropriate.
 */
void Jethro::update(Message msg) {
    switch(msg.getMessageType()) {
        case HIT:
        case KILL:
        case MISS:
            board[msg.getRow()][msg.getCol()] = msg.getMessageType();
            break;

        case WIN:
            break;
        case LOSE:
            break;
        case TIE:
            break;
        case OPPONENT_SHOT:
            // TODO: replace in your AI with code that does something useful with the information about where the opponent is shooting.
            int row = msg.getRow();
            int col = msg.getCol();
            opponentShotMap[row][col] += 1;
            displayShotMap();
            break;
    }
}

