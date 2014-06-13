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
    int monSel;
    int moveSel;
    Monster* myMonster;
    Monster* oppMonster;
    Move* myMove;
    Move* oppMove;
    char name[5];
} gameData;

void setupGame();

void processKeyboard(GlobalState* globalData, char* name, int size);
void printKeyboard(GlobalState* globalData, char* inputType, int size);

// Game Prototypes
void singlePlayer(GlobalState* globalData);
void multiPlayer(GlobalState* globalData);
void buildCards(GlobalState* globalData);
void getCards(void);
void buildCard(GlobalState* globalData, int sel);

// Helper Prototypes
int findPlayer(GlobalState* globalData);
int gameStatus(void);
int attack(Move* attack, Monster* monster, int targetScore);
void selectCard(GlobalState* globalData);
void pickMove(GlobalState* globalData);

// Xbee Prototypes
void sendMove(void);
void receiveMove(void);
void sendScore(void);
int receiveScore(void);

// Game Display Prototypes
void printGame(GlobalState* globalData);
int printSelect(GlobalState* globalData);
void printAttackMenu(GlobalState* globalData, Monster* card);
void printResults(void);
Move getMoveFromList(char id);
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
