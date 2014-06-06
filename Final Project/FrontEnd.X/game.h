#include "globals.h"

typedef struct _gameData {
    int myScore;
    int oppScore;
    short turn;
    int gameOver;
    int myMove;
    int oppMove;
    char* moveName;
    char name[5];
} gameData;

void setupGame();

void printKeyboard(GlobalState* globalData, char* name);

// Game Prototypes
void singlePlayer(GlobalState* globalData);
void multiPlayer(GlobalState* globalData);
void buildCards(GlobalState* globalData);

// Helper Prototypes
int findPlayer(void);
int gameStatus(void);
int attack(int attackDamage, int targetScore);
int selectCard(GlobalState* globalData);
int pickMove(GlobalState* globalData);

// Xbee Prototypes
void sendMove();
int recieveMove();
void sendScore();
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
