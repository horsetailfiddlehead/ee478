/* 


*/
#include <stdio.h>
#include <stdlib.h>

int myScore;
int oppScore;
int gameOver;

void setup() {
	myScore = 0;
	oppScore = 0;
	gameOver = 0;
}

void singlePlayer() {
	int compMove = 0;
	int myMove = 0;
	int turn = rand() % 2;
	
	// Setup new game
	setup();
	
	// Begin game with computer
	while(!gameOver) {
		// Situation depends on turn
		if(turn) {
			myMove = pickMove();
			oppScore -= myMove;
		} else {
			// Computer randomly picks an attak with damage b/w 0 and 50
			compMove = rand() % 50 + 1;
			myScore -= compMove;
		}
		
		// Check game status
		gameOver = gameStatus(myScore, oppScore);
		!turn;
	}
	// Display results once there is a lost
	
}

 void multiPlayer() {
	// Find other players
	findPlayer();
	
	// Begin game
	while(!gameOver) {
	
		// Check game status
		gameOver = gameStatus(myScore, oppScore);
	}
	// Display results once there is a lost
}

 void buildCard() {
	// Read Card to be built
	
	// Enter data to be stored in card
	
	// Write data to card
}

// Sets up connection with other players
 void findPlayer() {

}

// User selects move based off of available cards
// Returns damage of chosen move
 int pickMove() {
	return myMove;
}

// Displays
 void displayResults() {

}

 int gameStatus(int player1, int player2) {
	if(0 == player1 || 0 == player2) {
		return 1;
	} else {
		return 0;
	}
}