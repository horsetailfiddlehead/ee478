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
 int gameStatus(int player1, int player2) {
	if(0 == player1 || 0 == player2) {
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
 int pickMove() {
	return myMove;
}

// Send move to opponent
int sendMove(int move) {
}

// Receive move from opponent
int recieveMove(int move) {
}

// Send my score to opponent after taking damage
int sendScore (int score) {
}

// Receive new score from opponent after attacking
int recieveScore(int score) {
}

//
void printGame(int status) {
	switch(status) {		
		case 0:
			prints("Your Move!");
			printMoveMenu();
			break;
		case 1:
			prints("Opponent's turn.");
			break;
		case 2:
			prints("Opponent used:");
			prints(moveName);
			break;
		case 3:
			prints("");
			break;
	}
	prints(0, 10, WHITE, BLACK, "Your Score: ");
	integerprint(0, 25, WHITE, BLACK, myScore);
	prints(0, 10, WHITE, BLACK, "Opponent Score: ");
	integerprint(0, 25, WHITE, BLACK, oppScore);

}

// Displays Results
 void printResults() {
	if(myScore > oppScore) {	
		prints(YELLOW, BLACK, "Your Score: ");
		integerprint(YELLOW, BLACK, myScore);
		prints(WHITE, BLACK, "Opponent Score: ");
		integerprint(WHITE, BLACK, oppScore);
	} else {
		prints(WHITE, BLACK, "Your Score: ");
		integerprint(WHITE, BLACK, myScore);
		prints(YELLOW, BLACK, "Opponent Score: ");
		integerprint(YELLOW, BLACK, oppScore);
	}
}
