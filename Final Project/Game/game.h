#include "globals.h"

typedef struct _gameData {
    int myScore;
    int oppScore;
    short turn;
    int gameOver;
    int myMove;
    int oppMove;
    char* moveName;
} gameData;

void setup();

// Game Prototypes
void singlePlayer(gameData game);
void multiPlayer(gameData game);
void buildCards(gameData game);

// Helper Prototypes
void findPlayer();
void gameStatus();
int attack(int attackDamage, int targetScore);
int selectCard(GlobalState* globalData);
int pickMove(GlobalState* globalData);

// Xbee Prototypes
int sendMove(int move);
int recieveMove();
int sendScore(int score);
int recieveScore();

// Game Display Prototypes
void printGame(GlobalState* globalData);
void printSelect(GlobalState* globalData);
void printAttackMenu(GlobalState* globalData, int card);
void printResults();

/*
typedef struct _gameData {
    int myScore;
    int oppScore;
    short turn;
    int gameOver;
    int myMove;
    int oppMove;
    char* moveName;
} gameData;

*/
