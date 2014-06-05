/* 


*/
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "game.h"
#include "LCD.h"

#define HEALTH 100

gameData game;

void setupGame() {
	game.myScore = HEALTH;
	game.oppScore = HEALTH;
	game.turn = 0;
	game.gameOver = 0;
	game.moveName = NULL;
        game.myMove =0;
        game.oppMove = 0;
}

void singlePlayer(GlobalState* globalData) {
	// Setup new game
	setupGame();
	game.turn = rand() % 2;
	
	// Begin game with computer
	while(!game.gameOver) {
		// Situation depends on game.turn
		if(game.turn) {
			game.myMove = pickMove(globalData);
			game.oppScore = attack(game.myMove, game.oppScore);
		} else {
			// Computer randomly picks an attack with damage b/w 0 and 50
			game.oppMove = rand() % 50 + 1;
			game.myScore = attack(game.oppMove, game.myScore);
		}
		// Check game status
		game.gameOver = gameStatus();
		game.turn = !game.turn;
	}
	// Display results once there is a lost
	printResults();
}

 void multiPlayer(GlobalState* globalData) {
        int myMove;
        int oppMove = 0;
	int connect = 0;
	setupGame();
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
			myMove = pickMove(globalData);
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
 void buildCard(GlobalState* globalData) {
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
    printSelect(globalData);
    prints(0, 5, WHITE, RED, "Choose a card by its slot number:", 1);
    // Waits till valid keypad input
    globalData->keyStatus = 1;
    while (0 != globalData->keyStatus) {
        keypad(globalData);
        // Invalid keypad input
        if (globalData->keyPress >= 0) {
            // Beep off
            TRISBbits.RB5 = 1;
            if ((0 == globalData->keyPress || globalData->keyPress > 5)) {
                globalData->keyStatus = 1;
                prints(0, 5, WHITE, RED, "                                                 ", 1);
                prints(0, 5, WHITE, RED, "Invalid input. Please enter a key between 1 to 4:", 1);
                // No card available
            } else if (0 == globalData->cardSelect[globalData->keyPress - 1]) {
                globalData->keyStatus = 2;
                prints(0, 5, WHITE, RED, "                                ", 1);
                prints(0, 5, WHITE, RED, "No card found. Please try again:", 1);
                // Selected card is available
            } else {
                globalData->keyStatus = 0;
            }
        }
    }
    return (globalData->keyPress - 1);
}

// User selects move based off of available cards
// Regame.turns damage of chosen move
 int pickMove(GlobalState* globalData) {
	int card = selectCard(globalData);
	printAttackMenu(globalData, card);
        prints(0, 5, WHITE, CYAN, "Please select an attack: ", 1);
	globalData->keyStatus = 1;
	// Checks keypad input and outputs a message to user if incorrect
	while(0 != globalData->keyStatus) {
            keypad(globalData);
            if(globalData->keyPress >= 0) {
                // Keypad input is not valid
                if(((0x0A > globalData->keyPress) || (globalData->keyPress > 0x0D))) {
                        globalData->keyStatus = 1;
                        prints(0, 5, WHITE, CYAN, "Invalid attack input. Please select from the options below: ", 1);
                // Selected attack does nothing
                } else if (0 == globalData->selectMove[card][(globalData->keyPress)-10]) {
                        globalData->keyStatus = 2;
                        prints(0, 5, WHITE, CYAN, "This will have no effect! Select a better option.", 1);
                // Input is valid and passes all checks
                } else {
                        globalData->keyStatus = 0;
                         prints(0, 5, WHITE, CYAN, "Please select an attack: ", 1);
                }
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
         // Beep off
    TRISBbits.RB5 = 1;

    clean(RED);
    // Prints messages to LCD based off of keypad input
	// Display commands to select a slot - the LED's should indicate if a card is read
	prints(35, 30, WHITE, BLACK, "Slot 1", 1);
	prints(35, 60, WHITE, BLACK, "Slot 2", 1);
	prints(35, 90, WHITE, BLACK, "Slot 3", 1);
	prints(35, 120, WHITE, BLACK, "Slot 4", 1);
}

// Select a card to play
void printSelect(GlobalState* globalData) {
        // Beep off
    TRISBbits.RB5 = 1;

	// LCD menu
        clean(RED);
	// Display commands to select a slot - the LED's should indicate if a card is read
        drawBoxFill(30, 39, 8, 45, BLACK);
        drawBoxFill(30, 69, 8, 45, BLACK);
        drawBoxFill(30, 99, 8, 45, BLACK);
        drawBoxFill(30, 129, 8,45, BLACK);
	prints(35, 40, WHITE, BLACK, "Slot 1", 1);
	prints(35, 70, WHITE, BLACK, "Slot 2", 1);
	prints(35, 100, WHITE, BLACK, "Slot 3", 1);
	prints(35, 130, WHITE, BLACK, "Slot 4", 1);
}

// Select an attack.
void printAttackMenu(GlobalState* globalData, int card) {
     // Beep off
    TRISBbits.RB5 = 1;

    // LCD menu
    clean(BLUE);
	
    // Show attack options and their damage
    prints(0, 40, WHITE, BLUE, "A. Attack with:", 1);
    integerprint(0, 48, WHITE, BLUE, globalData->selectMove[card][0],1);
    prints(25, 48, WHITE, BLUE, "damage", 1);

    prints(0, 80, WHITE, BLUE, "B. Attack with: ", 1);
    integerprint(0, 88, WHITE, BLUE, globalData->selectMove[card][1],1);
    prints(25, 88, WHITE, BLUE, "damage", 1);
    
    prints(0, 120, WHITE, BLUE, "C. Attack with: ", 1);
    integerprint(0, 128, WHITE, BLUE, globalData->selectMove[card][2],1);
    prints(25, 128, WHITE, BLUE, "damage", 1);
}

// Displays Game Results
void printResults() {
        // Beep off
    TRISBbits.RB5 = 1;
        clean(RED);
	prints(0, 15, YELLOW, BLACK, "GAME OVER", 1);
	
	if(game.myScore > game.oppScore) {
		prints(0, 15, YELLOW, BLACK, "You won!", 1);
		prints(0, 30, YELLOW, BLACK, "Your Score: ", 1);
		integerprint(0, 45, YELLOW, BLACK, game.myScore,1);
		prints(0,60, WHITE, BLACK, "Opponent Score: ",1);
		integerprint(0, 75, WHITE, BLACK, game.oppScore,1);
	} else {
		prints(0, 15, RED, BLACK, "You lost", 1);
		prints(0, 30, WHITE, BLACK, "Your Score: ",1);
		integerprint(0, 45, WHITE, BLACK, game.myScore,1);
		prints(0, 60, YELLOW, BLACK, "Opponent Score: ",1);
		integerprint(0, 75, YELLOW, BLACK, game.oppScore,1);
	}
}
