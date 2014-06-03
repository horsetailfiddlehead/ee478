/* 


*/
#include <stdio.h>
#include <stdlib.h>
#include "LCD.h"

int myScore;
int oppScore;
int turn;
int gameOver;
char* moveName;

void setup() {
	myScore = 0;
	oppScore = 0;
	gameOver = 0;
	
}

void singlePlayer() {
	turn = rand() % 2;
	
	// Setup new game
	setup();
	
	// Begin game with computer
	while(!gameOver) {
		// Situation depends on turn
		if(turn) {
			myMove = pickMove();
			oppScore = attack(myMove, oppScore);
		} else {
			// Computer randomly picks an attak with damage b/w 0 and 50
			compMove = rand() % 50 + 1;
			myScore = attack(compMove, myScore);
		}
		
		// Check game status
		gameOver = gameStatus(myScore, oppScore);
		!turn;
	}
	// Display results once there is a lost
	
}

 void multiPlayer() {
	int oppMove = 0;
	int connect = 0;
	
	// Find other players
	while(!connect) {
		connect = findPlayer();
	}
	
	// Begin game
	while(!gameOver) {
		while(connect) {
			if(turn) {
				// Pick Move
				myMove = pickMove();
				// Send Move
					sendMove(myMove);
				// Receive new score after opponent takes damage
					oppScore = recieveScore();
			} else {				
				// Receive player move and take damage
				myScore = attack(recieveMove(), myScore);
				// Send new score
				sendScore(myScore);
			}		
			// Check game status
			gameOver = gameStatus(myScore, oppScore);
			!turn;
		}
		// If connection is lost, re-establish connection with other player
		connect = findPlayer();
	}
	
	//
	// Display results once there is a lost
}

// Program data on the card
// Monster ID
// Attack ID
 void buildCard() {
	// Read Card to be built
	
	// Enter data to be stored in card
	
	// Write data to card
}

/* The following functions are helper commands for the main games.
 * Set Up
 * Moves
 * Display
 */
 
// Sets up connection with other players
 int findPlayer() {
	return 0;
}

// Checks if a winner has been found
 int gameStatus() {
	if(0 == oppScore || 0 == myScore) {
		return 1;
	} else {
		return 0;
	}
}

// Returns new score of player after taking damage
int attack (int attackDamage, int targetScore) {
	if(attackDamage < targetScore) {
		return (targetScore - attackDamage);
	} else {
		return 0;
	}
}

// User selects move based off of available cards
// Returns damage of chosen move
 int pickMove(GlobalState* globalData) {
	printAttackMenu(globalData);
	switch(globalData->keyPress) {
		case 0x0A:
			return selectMove[0]; 
			break;
		case 0x0B:
			return selectMove[1];
			break;
		case 0xC:
			return select[2]
			break;
	}
}

// Xbee: Send move to opponent
int sendMove(int move) {
	return 0;
}

// Xbee: Receive move from opponent
int recieveMove() {
	return 0;
}

// Xbee: Send my score to opponent after taking damage
int sendScore (int score) {
	return 0;
}

// Xbee: Receive new score from opponent after attacking
int recieveScore() {
	return 0;
}

void printSelectMonster(GlobalState* globalData) {
	// LCD menu
    clean(RED);
    drawBoxFill(0, 0, 20, V - 1, RED);
    drawBox(0, 0, 20, V - 1, 2, WHITE);
    prints(35, 7, WHITE, RED, "Choose a monster by its slot number:", 1);
	
	// Check if there is a card / monster that has been read
	prints(35, 30, WHITE, BLACK, "Slot 1", 1);
	prints(35, 60, WHITE, BLACK, "Slot 2", 1);
	prints(35, 90, WHITE, BLACK, "Slot 3", 1);
	prints(35, 120, WHITE, BLACK, "Slot 4", 1);
}

void printAttackMenu(GlobalState* globalData) {
    int i = 0;
	
	// LCD menu
    clean(BLUE);
    drawBoxFill(0, 0, 20, V - 1, CYAN);
    drawBox(0, 0, 20, V - 1, 2, WHITE);
    prints(35, 7, WHITE, CYAN, "Please select an attack: ", 1);
    // Show attack options and heir
	prints(35, 40, WHITE, BLUE, "A. Attack with max. damage: ", 1);
	interprint(60, 40, WHITE, BLUE, moveSelect[0]);
	prints(35, 80, WHITE, BLUE, "B. Attack with damage: ", 1);
	interprint(60, 80, WHITE, BLUE, moveSelect[1]);
	prints(35, 120, WHITE, BLUE, "C. Attack with damage: ", 1);
	interprint(60, 120, WHITE, BLUE, moveSelect[2]);
}

// Displays Results
 void printResults() {
	prints(0, 15, YELLOW, BLACK, "GAME OVER", 1);
	
	if(myScore > oppScore) {
		prints(0, 15, YELLOW, BLACK, "You won!", 1);
		prints(0, 15, YELLOW, BLACK, "Your Score: ", 1);
		integerprint(0, 15, YELLOW, BLACK, myScore);
		prints(WHITE, BLACK, "Opponent Score: ");
		integerprint(WHITE, BLACK, oppScore);
	} else {
		prints(0, 15, RED, BLACK, "You lost", 1);
		prints(0, 15, WHITE, BLACK, "Your Score: ");
		integerprint(0, 15, WHITE, BLACK, myScore);
		prints(0, 15, YELLOW, BLACK, "Opponent Score: ");
		integerprint(0, 15, YELLOW, BLACK, oppScore);
	}
}
