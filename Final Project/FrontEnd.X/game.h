#include "globals.h"

typedef enum _myTypes {
    FIRE = 0, WATER = 1, EARTH = 2
}Type;

typedef struct _move {
    char moveName[9];
    Type moveType;
    int baseDamage;
    int uses;
} Move;

typedef struct _monster {
    char monsterName[9];
    int monsterID;
    int level;
    Type monsterType;
    Move movelist[3];
} Monster;

typedef struct _gameData {
    int myScore;
    int oppScore;
    short turn;
    int gameOver;
    Monster* myMonster;
    Monster* oppMonster;
    Move* myMove;
    Move* oppMove;
    char name[5];
} gameData;

void setupGame();

void printKeyboard(GlobalState* globalData, char* name);

// Game Prototypes
void singlePlayer(GlobalState* globalData);
void multiPlayer(GlobalState* globalData);
void buildCards(GlobalState* globalData);
void getCards(void);

// Helper Prototypes
int findPlayer(GlobalState* globalData);
int gameStatus(void);
int attack(Move* attack, Monster* monster, int targetScore);
Monster* selectCard(GlobalState* globalData);
Move* pickMove(GlobalState* globalData);

// Xbee Prototypes
void sendMove(void);
void receiveMove(void);
void sendScore(void);
int receiveScore(void);

// Game Display Prototypes
void printGame(GlobalState* globalData);
void printSelect(GlobalState* globalData);
void printAttackMenu(GlobalState* globalData, int card);
void printResults(void);

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
