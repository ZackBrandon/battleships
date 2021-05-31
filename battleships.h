/**
 * @author Zack Brandon
 * @date April 2021
 *
 * Please type in your name[s] here if you change the code:
 *Zack Brandon
 *
 */

#ifndef JETHRO_H        // Double inclusion protection
#define JETHRO_H

using namespace std;

#include "PlayerV2.h"
#include "Message.h"
#include "defines.h"

// Jethro inherits from/extends PlayerV2

class Jethro: public PlayerV2 {
  public:
    Jethro( int boardSize );
    ~Jethro();
    void newRound();
    Message placeShip(int length);
    Message getMove();
    void killShip(int scanRow, int scanCol, int& shotRow, int& shotCol);
    bool killShipNorth(int scanRow, int scanCol, int& shotRow, int& shotCol);
    bool killShipSouth(int scanRow, int scanCol, int& shotRow, int& shotCol);
    bool killShipEast(int scanRow, int scanCol, int& shotRow, int& shotCol);
    bool ifCanPlaceShip(int row, int col, Direction dir, int length);
    bool killShipWest(int scanRow, int scanCol, int& shotRow, int& shotCol);
    void findShot(int& scanRow, int& scanCol);
    void showLevelizerBoard();
    void displayShotMap();
    void updateOpponentShipBoard();
    void updateKillDisplay();
    void showStatsBoard();
    int currentShipLength = 0;
    void goToNextShot(int& scanRow, int& scanCol);
    void update(Message msg);
    bool canPlaceShip(int row, int col, Direction dir, int length);
    void markShip(int row, int col, Direction dir, int length);
    char ships[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
   // char thisLineNotWorking[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    int opponentShotMap[10][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
    };
    void hitOnBoard(int& scanRow, int& scanCol, int& shotRow, int& shotCol);
    int shotRow = 0;
    int shotCol = 0;
    int scanRow;
    int scanCol;
    int firstShot;
  private:
    void initializeBoard();
    int lastRow;
    int lastCol;
    int numShipsPlaced;
    char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
};

#endif