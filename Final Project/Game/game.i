#line 1 "game.c"
#line 1 "game.c"

#line 4 "game.c"
 
#line 1 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdio.h"

#line 3 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdio.h"

#line 1 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdarg.h"
 


#line 5 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdarg.h"

typedef void* va_list;
#line 8 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdarg.h"
#line 9 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdarg.h"
#line 10 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdarg.h"
#line 11 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdarg.h"
#line 12 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdarg.h"
#line 4 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdio.h"

#line 1 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stddef.h"
 

#line 4 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stddef.h"

typedef unsigned char wchar_t;


#line 10 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stddef.h"
 
typedef signed short int ptrdiff_t;
typedef signed short int ptrdiffram_t;
typedef signed short long int ptrdiffrom_t;


#line 20 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stddef.h"
 
typedef unsigned short int size_t;
typedef unsigned short int sizeram_t;
typedef unsigned short long int sizerom_t;


#line 34 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stddef.h"
 
#line 36 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stddef.h"


#line 41 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stddef.h"
 
#line 43 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stddef.h"

#line 45 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stddef.h"
#line 5 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdio.h"



#line 9 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdio.h"
 
#line 11 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdio.h"

#line 13 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdio.h"


typedef unsigned char FILE;

 
#line 19 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdio.h"
#line 20 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdio.h"

extern FILE *stderr;
extern FILE *stdout;


int putc (auto char c, auto FILE *f);
int vsprintf (auto char *buf, auto const far  rom char *fmt, auto va_list ap);
int vprintf (auto const far  rom char *fmt, auto va_list ap);
int sprintf (auto char *buf, auto const far  rom char *fmt, ...);
int printf (auto const far  rom char *fmt, ...);
int fprintf (auto FILE *f, auto const far  rom char *fmt, ...);
int vfprintf (auto FILE *f, auto const far  rom char *fmt, auto va_list ap);
int puts (auto const far  rom char *s);
int fputs (auto const far  rom char *s, auto FILE *f);

#line 36 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdio.h"
#line 5 "game.c"

#line 1 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 

#line 4 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"

#line 6 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"

#line 9 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
 

#line 16 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
double atof (const auto char *s);

#line 28 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
signed char atob (const auto char *s);


#line 39 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
int atoi (const auto char *s);

#line 47 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
long atol (const auto char *s);

#line 58 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
unsigned long atoul (const auto char *s);


#line 71 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
char *btoa (auto signed char value, auto char *s);

#line 83 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
char *itoa (auto int value, auto char *s);

#line 95 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
char *ltoa (auto long value, auto char *s);

#line 107 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
char *ultoa (auto unsigned long value, auto char *s);
 


#line 112 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
 

#line 116 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
#line 118 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"


#line 124 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
int rand (void);

#line 136 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
 
void srand (auto unsigned int seed);
 
#line 140 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
#line 149 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"

#line 151 "C:/Program Files (x86)/Microchip/mplabc18/v3.46/bin//..//h/stdlib.h"
#line 6 "game.c"


#line 1 "./game.h"
void setup();


void singlePlayer();
void multiPlayer();
void buildCards();


void findPlayer();
void gameStatus();
int attack(int attackDamage, int targetScore);
int pickMove();


int sendMove(int move);
int recieveMove();
int sendScore();
int recieveScore();


void printSelect(GlobalState* globalData);
void printAttackMenu(GlobalState* globalData, int card);
 void printResults();
 
 typedef struct gamedata {
    int myScore;
	int oppScore;
	short turn;
	int gameOver;
	char* moveName;
}gameData;#line 8 "game.c"

#line 1 "./LCD.h"

#line 6 "./LCD.h"
 


#line 10 "./LCD.h"

#line 14 "./LCD.h"



#line 18 "./LCD.h"
#line 19 "./LCD.h"

#line 21 "./LCD.h"

#line 23 "./LCD.h"
#line 24 "./LCD.h"
#line 25 "./LCD.h"
#line 26 "./LCD.h"

#line 28 "./LCD.h"
#line 29 "./LCD.h"
#line 30 "./LCD.h"
#line 31 "./LCD.h"

#line 33 "./LCD.h"
#line 34 "./LCD.h"
#line 35 "./LCD.h"
#line 36 "./LCD.h"
#line 37 "./LCD.h"
#line 38 "./LCD.h"
#line 39 "./LCD.h"
#line 40 "./LCD.h"

#line 42 "./LCD.h"
#line 43 "./LCD.h"
#line 44 "./LCD.h"

#line 46 "./LCD.h"
#line 47 "./LCD.h"
#line 48 "./LCD.h"
#line 49 "./LCD.h"
#line 50 "./LCD.h"

#line 52 "./LCD.h"
#line 53 "./LCD.h"
#line 54 "./LCD.h"
#line 55 "./LCD.h"
#line 56 "./LCD.h"
#line 57 "./LCD.h"

#line 59 "./LCD.h"
#line 60 "./LCD.h"
#line 61 "./LCD.h"
#line 62 "./LCD.h"

#line 64 "./LCD.h"

#line 66 "./LCD.h"
#line 67 "./LCD.h"


#line 70 "./LCD.h"
#line 71 "./LCD.h"
#line 72 "./LCD.h"
#line 73 "./LCD.h"
#line 74 "./LCD.h"
#line 75 "./LCD.h"
#line 76 "./LCD.h"
#line 77 "./LCD.h"
#line 78 "./LCD.h"


#line 81 "./LCD.h"
#line 82 "./LCD.h"
#line 83 "./LCD.h"

void delay(int x);
void sendcomand(char input);
void senddata(char input);
void SetPix(char x, char y, int color);
void clean(int color);
void initLCD(void);
void ASCII(char x, char y, int color, int background, char letter, char size);

int customColor(int r, int g, int b);
void drawBoxFill(char x, char y,char height, char width, int color);
void drawBox(char x, char y, char height, char width, int border, int color);

void prints(char x, char y, int color, int background, const char message[], char size);
void printrs(char x, char y, int color, int background, char* message, char size);
void integerprint(char x, char y, int color, int background,int integer, char size);

typedef struct globaldata GlobalState;


void nextPage(GlobalState* globalData, int cursorPos);
void printBSOD(void);
void printMainMenu(GlobalState* globalData);
void processDisplay(GlobalState* globalData);


void box(char x, char y, char high, char breth, int color);


#line 115 "./LCD.h"

#line 117 "./LCD.h"

#line 9 "game.c"


gameData game;

void setup() {
	game.myScore = HEALTH;
	game.oppScore = HEALTH;
	game.turn = 0;
	game.gameOver = 0;
	game.moveName = 0 ;
}

void singlePlayer(gameData game) {
	
	setup();
	game.turn = rand() % 2;
	
	
	while(!game.gameOver) {
		
		if(game.turn) {
			game.myMove = pickMove();
			game.oppScore = attack(game.myMove, game.oppScore);
		} else {
			
			game.oppMove = rand() % 50 + 1;
			game.myScore = attack(game.oppMove, game.myScore);
		}
		
		game.gameOver = gameStatus();
		!game.turn;
	}
	
	printResults();
}

 void multiPlayer() {
        int myMove;
        int oppMove = 0;
	int connect = 0;
	
	
	while(!connect) {
		connect = findPlayer();
	}
	
	
	
	
	
	while(!game.gameOver && connect) {
		if(game.turn) {
			
			myMove = pickMove();
			
				sendMove(myMove);
			
				game.oppScore = recieveScore();
		} else {				
			
			game.myScore = attack(recieveMove(), game.myScore);
			
			sendScore(game.myScore);
		}		
		
		game.gameOver = gameStatus();
		!game.turn;
	}
	if (!connect) {
		printBSOD();
	} else {
		printResults();
	}
}




 void buildCard() {
	
	
	
	
	
}


#line 99 "game.c"
 
 

 int findPlayer() {
	return 0;
}


 int gameStatus() {
	return (0 == game.oppScore || 0 == game.myScore);
}


int attack (int attackDamage, int targetScore) {
	if(attackDamage < targetScore) {
		return (targetScore - attackDamage);
	} else {
		return 0;
	}
}



 int selectCard(GlobalState* globalData) {
	globalData->keyStatus = 0;
	printSelect(globalData);
	
	
	globalData->keyStatus = 1;
	while(0 != globalData->keyStatus) {
		
		if(0 == globalData->keyPress || globalData->keypress > 5) {
			globalData->keyStatus = 1;
			printSelectMenu(globalData);
		
		} else if (0 == globalData->cardSelect[globalData->keypress-1]) {
			globalData->keyStatus = 2;
			printSelectMenu(globalData);
		
		} else {
			globalData->keyStatus = 0;
		}
	}
	return (globalData->keypress - 1);
}



 int pickMove(GlobalState* globalData) {
	int card = selectCard(globalData);
	printAttackMenu(globalData, card);
	globalData->keyStatus = 1;
	
	while(0 != globalData->keyStatus) {
		
		if((10 > globalData->keypress) || (globalData->keypress > 12)) {
			globalData->keyStatus = 1;
			printAttackMenu(globalData, card);
		
		} else if (0 == globalData->selectMove[card][(globalData->keypress)-10]) {
			globalData->keyStatus = 2;
			printAttackMenu(globalData, card);
		
		} else {
			globalData->keyStatus = 0;
		}
	}
	
	
	
	switch(globalData->keyPress) {
		case 0x0A:
			return globalData->selectMove[card][0]; 
			break;
		case 0x0B:
			return globalData->selectMove[card][1];
			break;
		case 0xC:
			return globalData->selectMove[card][2];
			break;
	}
}


#line 184 "game.c"
 


int sendMove(int move) {
	return 0;
}


int recieveMove() {
	return 0;
}


int sendScore (int score) {
	return 0;
}


int recieveScore() {
	return 0;
}


#line 208 "game.c"
 
 
 
 
 void printGame(GlobalState* globalData) {
	
    clean(((((255>>3)<<11) | (0>>3) | ((0>>2)<<5)))  );
    drawBoxFill(0, 0, 20, 128  - 1, ((((255>>3)<<11) | (0>>3) | ((0>>2)<<5)))  );
    drawBox(0, 0, 20, 128  - 1, 2, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  );
    
	switch(globalData->keyStatus) {
		case 0:
			prints(35, 7, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((255>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Choose a card by its slot number:", 1);
			break;
		case 1:
			prints(35, 7, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((255>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Invalid input. Please enter a key between 1 to 4:", 1);
			break;
		case 2:
			prints(35, 7, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((255>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "No card found. Please try again:", 1);
			break;
	}
	
	prints(35, 30, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Slot 1", 1);
	prints(35, 60, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Slot 2", 1);
	prints(35, 90, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Slot 3", 1);
	prints(35, 120, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Slot 4", 1);
}


void printSelect(GlobalState* globalData) {
	
    clean(((((255>>3)<<11) | (0>>3) | ((0>>2)<<5)))  );
    drawBoxFill(0, 0, 20, 128  - 1, ((((255>>3)<<11) | (0>>3) | ((0>>2)<<5)))  );
    drawBox(0, 0, 20, 128  - 1, 2, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  );
    
	switch(globalData->keyStatus) {
		case 0:
			prints(35, 7, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((255>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Choose a card by its slot number:", 1);
			break;
		case 1:
			prints(35, 7, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((255>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Invalid input. Please enter a key between 1 to 4:", 1);
			break;
		case 2:
			prints(35, 7, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((255>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "No card found. Please try again:", 1);
			break;
	}
	
	prints(35, 30, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Slot 1", 1);
	prints(35, 60, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Slot 2", 1);
	prints(35, 90, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Slot 3", 1);
	prints(35, 120, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Slot 4", 1);
}


void printAttackMenu(GlobalState* globalData, int card) {
	
    clean(((((0>>3)<<11) | (0>>3) | ((255>>2)<<5)))  );
    drawBoxFill(0, 0, 20, 128  - 1, ((((0>>3)<<11) | (255>>3) | ((255>>2)<<5)))  );
    drawBox(0, 0, 20, 128  - 1, 2, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  );
	
	
	switch(globalData->keyStatus) {
		case 0:
			prints(35, 7, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , "Please select an attack: ", 1);
			break;
		case 1:
			prints(35, 7, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , "Invalid attack input. Please select from the options below: ", 1);
			break;
		case 2:
			prints(35, 7, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , "This will have no effect! Select a better option.", 1);
			break;
    }
	
	
	prints(35, 40, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((255>>2)<<5)))  , "A. Attack with max. damage: ", 1);
	integerprint(60, 40, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((255>>2)<<5)))  , selectMove[card][0]);
	prints(35, 80, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((255>>2)<<5)))  , "B. Attack with damage: ", 1);
	interprint(60, 80, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((255>>2)<<5)))  , selectMove[card][1]);
	prints(35, 120, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((255>>2)<<5)))  , "C. Attack with damage: ", 1);
	integerprint(60, 120, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((255>>2)<<5)))  , selectMove[card][2]);
}


void printResults() {
	prints(0, 15, ((((255>>3)<<11) | (255>>3) | ((0>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "GAME OVER", 1);
	
	if(game.myScore > game.oppScore) {
		prints(0, 15, ((((255>>3)<<11) | (255>>3) | ((0>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "You won!", 1);
		prints(0, 15, ((((255>>3)<<11) | (255>>3) | ((0>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Your Score: ", 1);
		integerprint(0, 15, ((((255>>3)<<11) | (255>>3) | ((0>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , game.myScore);
		prints(((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Opponent Score: ");
		integerprint(((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , game.oppScore);
	} else {
		prints(0, 15, ((((255>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "You lost", 1);
		prints(0, 15, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Your Score: ");
		integerprint(0, 15, ((((255>>3)<<11) | (255>>3) | ((255>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , game.myScore);
		prints(0, 15, ((((255>>3)<<11) | (255>>3) | ((0>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , "Opponent Score: ");
		integerprint(0, 15, ((((255>>3)<<11) | (255>>3) | ((0>>2)<<5)))  , ((((0>>3)<<11) | (0>>3) | ((0>>2)<<5)))  , game.oppScore);
	}
}
