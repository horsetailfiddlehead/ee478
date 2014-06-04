/* 


*/
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "game.h"
#include "LCD.h"

#define HEALTH 100;

gameData game;

void setup() {
	game.myScore = HEALTH;
	game.oppScore = HEALTH;
	game.turn = 0;
	game.gameOver = 0;
	game.moveName = NULL;
        game.myMove =0;
        game.oppMove = 0;
}

void singlePlayer(gameData game) {
	// Setup new game
	setup();
	game.turn = rand() % 2;
	
	// Begin game with computer
	while(!game.gameOver) {
		// Situation depends on game.turn
		if(game.turn) {
			game.myMove = pickMove();
			game.oppScore = attack(game.myMove, game.oppScore);
		} else {
			// Computer randomly picks an attack with damage b/w 0 and 50
			game.oppMove = rand() % 50 + 1;
			game.myScore = attack(game.oppMove, game.myScore);
		}
		// Check game status
		game.gameOver = gameStatus();
		!game.turn;
	}
	// Display results once there is a lost
	printResults();
}

 void multiPlayer() {
        int myMove;
        int oppMove = 0;
	int connect = 0;
	
	// Find other players
	while(!connect) {
		connect = findPlayer();
	}
	
	// NOTE: Need to build something to determine who goes first
	// Compare Xbee ID's perhaps?
	
	// Begin game
	while(!game.gameOver && connect) {
		if(game.turn) {
			// Pick Move
			myMove = pickMove();
			// Send Move
				sendMove(myMove);
			// Receive new score after opponent takes damage
				game.oppScore = recieveScore();
		} else {				
			// Receive player move and take damage
			game.myScore = attack(recieveMove(), game.myScore);
			// Send new score
			sendScore(game.myScore);
		}		
		// Check game status
		game.gameOver = gameStatus();
		!game.turn;
	}
	if (!connect) {
		printBSOD();
	} else {
		printResults();
	}
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
	return (0 == game.oppScore || 0 == game.myScore);
}

// Regame.turns new score of player after taking damage
int attack (int attackDamage, int targetScore) {
	if(attackDamage < targetScore) {
		return (targetScore - attackDamage);
	} else {
		return 0;
	}
}

// User selects move based off of available cards
// Regame.turns damage of chosen move
 int selectCard(GlobalState* globalData) {
	globalData->keyStatus = 0;
	printSelect(globalData);
	
	// Waits till valid keypad input
	globalData->keyStatus = 1;
	while(0 != globalData->keyStatus) {
		// Invalid keypad input
		if(0 == globalData->keyPress || globalData->keypress > 5) {
			globalData->keyStatus = 1;
			printSelectMenu(globalData);
		// No card available
		} else if (0 == globalData->cardSelect[globalData->keypress-1]) {
			globalData->keyStatus = 2;
			printSelectMenu(globalData);
		// Selected card is available
		} else {
			globalData->keyStatus = 0;
		}
	}
	return (globalData->keypress - 1);
}

// User selects move based off of available cards
// Regame.turns damage of chosen move
 int pickMove(GlobalState* globalData) {
	int card = selectCard(globalData);
	printAttackMenu(globalData, card);
	globalData->keyStatus = 1;
	// Checks keypad input and outputs a message to user if incorrect
	while(0 != globalData->keyStatus) {
		// Keypad input is not valid
		if((10 > globalData->keypress) || (globalData->keypress > 12)) {
			globalData->keyStatus = 1;
			printAttackMenu(globalData, card);
		// Selected attack does nothing
		} else if (0 == globalData->selectMove[card][(globalData->keypress)-10]) {
			globalData->keyStatus = 2;
			printAttackMenu(globalData, card);
		// Input is valid and passes all checks
		} else {
			globalData->keyStatus = 0;
		}
	}
	
	// Thing to consider: User pressing a key multiple times???
	// Regame.turn the amount of damage the move makes
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

/* Xbee functions for multiplayer
 *
 */

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

/* Display Functions for the Game
 * These functions are built to display the game screen.
 */
 
 
 
 void printGame(GlobalState* globalData) {
	// LCD menu
    clean(RED);
    drawBoxFill(0, 0, 20, V - 1, RED);
    drawBox(0, 0, 20, V - 1, 2, WHITE);
    // Prints messages to LCD based off of keypad input
	switch(globalData->keyStatus) {
		case 0:
			prints(35, 7, WHITE, RED, "Choose a card by its slot number:", 1);
			break;
		case 1:
			prints(35, 7, WHITE, RED, "Invalid input. Please enter a key between 1 to 4:", 1);
			break;
		case 2:
			prints(35, 7, WHITE, RED, "No card found. Please try again:", 1);
			break;
	}
	// Display commands to select a slot - the LED's should indicate if a card is read
	prints(35, 30, WHITE, BLACK, "Slot 1", 1);
	prints(35, 60, WHITE, BLACK, "Slot 2", 1);
	prints(35, 90, WHITE, BLACK, "Slot 3", 1);
	prints(35, 120, WHITE, BLACK, "Slot 4", 1);
}

// Select a card to play
void printSelect(GlobalState* globalData) {
	// LCD menu
    clean(RED);
    drawBoxFill(0, 0, 20, V - 1, RED);
    drawBox(0, 0, 20, V - 1, 2, WHITE);
    // Prints messages to LCD based off of keypad input
	switch(globalData->keyStatus) {
		case 0:
			prints(35, 7, WHITE, RED, "Choose a card by its slot number:", 1);
			break;
		case 1:
			prints(35, 7, WHITE, RED, "Invalid input. Please enter a key between 1 to 4:", 1);
			break;
		case 2:
			prints(35, 7, WHITE, RED, "No card found. Please try again:", 1);
			break;
	}
	// Display commands to select a slot - the LED's should indicate if a card is read
	prints(35, 30, WHITE, BLACK, "Slot 1", 1);
	prints(35, 60, WHITE, BLACK, "Slot 2", 1);
	prints(35, 90, WHITE, BLACK, "Slot 3", 1);
	prints(35, 120, WHITE, BLACK, "Slot 4", 1);
}

// Select an attack.
void printAttackMenu(GlobalState* globalData, int card) {
	// LCD menu
    clean(BLUE);
    drawBoxFill(0, 0, 20, V - 1, CYAN);
    drawBox(0, 0, 20, V - 1, 2, WHITE);
	
	// Prints messages to LCD based off of keypad input
	switch(globalData->keyStatus) {
		case 0:
			prints(35, 7, WHITE, CYAN, "Please select an attack: ", 1);
			break;
		case 1:
			prints(35, 7, WHITE, CYAN, "Invalid attack input. Please select from the options below: ", 1);
			break;
		case 2:
			prints(35, 7, WHITE, CYAN, "This will have no effect! Select a better option.", 1);
			break;
    }
	
	// Show attack options and their damage
	prints(35, 40, WHITE, BLUE, "A. Attack with max. damage: ", 1);
	integerprint(60, 40, WHITE, BLUE, game.selectMove[card][0]);
	prints(35, 80, WHITE, BLUE, "B. Attack with damage: ", 1);
	interprint(60, 80, WHITE, BLUE, game.selectMove[card][1]);
	prints(35, 120, WHITE, BLUE, "C. Attack with damage: ", 1);
	integerprint(60, 120, WHITE, BLUE, selectMove[card][2]);
}

// Displays Game Results
void printResults() {
	prints(0, 15, YELLOW, BLACK, "GAME OVER", 1);
	
	if(game.myScore > game.oppScore) {
		prints(0, 15, YELLOW, BLACK, "You won!", 1);
		prints(0, 15, YELLOW, BLACK, "Your Score: ", 1);
		integerprint(0, 15, YELLOW, BLACK, game.myScore);
		prints(WHITE, BLACK, "Opponent Score: ");
		integerprint(WHITE, BLACK, game.oppScore);
	} else {
		prints(0, 15, RED, BLACK, "You lost", 1);
		prints(0, 15, WHITE, BLACK, "Your Score: ");
		integerprint(0, 15, WHITE, BLACK, game.myScore);
		prints(0, 15, YELLOW, BLACK, "Opponent Score: ");
		integerprint(0, 15, YELLOW, BLACK, game.oppScore);
	}
}
