void setup();

// Game Prototypes
void singlePlayer();
void multiPlayer();
void buildCards();

// Helper Prototypes
void findPlayer();
void gameStatus();
int attack(int attackDamage, int targetScore);
int pickMove();

// Xbee Prototypes
int sendMove(int move);
int recieveMove();
int sendScore();
int recieveScore();

// Game Display Prototypes
void printSelect(GlobalState* globalData);
void printAttackMenu(GlobalState* globalData, int card);
 void printResults();
 
 typedef struct gamedata {
    int myScore;
	int oppScore;
	short turn;
	int gameOver;
        int myMove;
        int oppMove;
	char* moveName;
}gameData;