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
    game.myMove = 0;
    game.oppMove = 0;
    srand(ReadTimer0());
}

void singlePlayer(GlobalState* globalData) {
    // First time playing?
    // Read first time from SRAM
    if (globalData.firstTime == TRUE) {
        printKeyboard(globalData, game.name);
        globalData.firstTime = FALSE;
    }

    // Setup new game
    setupGame();
    game.turn = rand() % 2;
    printGame(globalData);

    // Wait till someone presses A to continue
    keypad(globalData);
    while (globalData->keyPress != 0x0D) {
        keypad(globalData);
    }

    // Begin game with computer
    while (!game.gameOver) {
        // Situation depends on game.turn
        if (game.turn) {
            game.myMove = pickMove(globalData);
            printGame(globalData);
            game.oppScore = attack(game.myMove, game.oppScore);
            prints(0, 10, RED, BLACK, "                    ", 1);
            prints(0, 10, RED, BLACK, "Opponent took damage:", 1);
            prints(0, 21, RED, BLACK, "-", 1);
            integerprint(6, 21, RED, BLACK, game.myMove, 1);
        } else {
            // Computer randomly picks an attack with damage b/w 0 and 50
            game.oppMove = rand() % 50 + 1;
            game.myScore = attack(game.oppMove, game.myScore);
            prints(0, 10, RED, BLACK, "                    ", 1);
            prints(0, 10, RED, BLACK, "Taken damage:", 1);
            prints(0, 21, RED, BLACK, "-", 1);
            integerprint(6, 21, RED, BLACK, game.oppMove, 1);
        }
        prints(0, 45, YELLOW, BLACK, "        ", 1);
        integerprint(0, 45, YELLOW, BLACK, game.myScore, 1);
        prints(0, 75, WHITE, BLACK, "        ", 1);
        integerprint(0, 75, WHITE, BLACK, game.oppScore, 1);

        keypad(globalData);
        while (globalData->keyPress != 0x0D) {
            keypad(globalData);
        };

        // Check game status
        game.gameOver = gameStatus();
        game.turn = !game.turn;
    }
    // Display results once there is a lost
    printResults();
}

void multiPlayer(GlobalState* globalData) {
    int connect = 0;

    setupGame();

    // Find other players
    while (!connect) {
        connect = findPlayer();
    }

    // NOTE: Need to build something to determine who goes first
    // Compare Xbee ID's perhaps?

    // Begin game
    while (!game.gameOver && connect) {
        if (game.turn) {
            // Pick Move
            game.myMove = pickMove(globalData);
            // Send Move
            sendMove();
            // Receive new score after opponent takes damage
            game.oppScore = recieveScore();
            printGame(globalData);
            prints(0, 10, RED, BLACK, "                    ", 1);
            prints(0, 10, RED, BLACK, "Opponent took damage:", 1);
            prints(0, 21, RED, BLACK, "-", 1);
            integerprint(6, 21, RED, BLACK, game.myMove, 1);
        } else {
            // Receive player move and take damage
            game.myScore = attack(recieveMove(), game.myScore);
            prints(0, 10, RED, BLACK, "                    ", 1);
            prints(0, 10, RED, BLACK, "Taken damage:", 1);
            prints(0, 21, RED, BLACK, "-", 1);
            integerprint(6, 21, RED, BLACK, game.oppMove, 1);
            // Send new score
            sendScore();
        }
        prints(0, 45, YELLOW, BLACK, "        ", 1);
        integerprint(0, 45, YELLOW, BLACK, game.myScore, 1);
        prints(0, 75, WHITE, BLACK, "        ", 1);
        integerprint(0, 75, WHITE, BLACK, game.oppScore, 1);
        // Check game status
        game.gameOver = gameStatus();
        game.turn =!game.turn;
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

int findPlayer(GlobalState* globalData) {
    printMultiplayerSetup(globalData);
    return 0;
}

// Checks if a winner has been found

int gameStatus() {
    return (0 == game.oppScore || 0 == game.myScore);
}

// Regame.turns new score of player after taking damage

int attack(int attackDamage, int targetScore) {
    if (attackDamage < targetScore) {
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
    prints(0, 5, WHITE, BLUE, "Please select an attack: ", 1);
    globalData->keyStatus = 1;
    // Checks keypad input and outputs a message to user if incorrect
    while (0 != globalData->keyStatus) {
        keypad(globalData);
        if (globalData->keyPress >= 0) {
            // Keypad input is not valid
            if (((0x0A > globalData->keyPress) || (globalData->keyPress > 0x0D))) {
                globalData->keyStatus = 1;
                prints(0, 5, WHITE, BLUE, "                                                            ", 1);
                prints(0, 5, WHITE, BLUE, "Invalid attack input. Please select from the options below: ", 1);
                // Selected attack does nothing
            } else if (0 == globalData->selectMove[card][(globalData->keyPress) - 10]) {
                globalData->keyStatus = 2;
                prints(0, 5, WHITE, BLUE, "                                                 ", 1);
                prints(0, 5, WHITE, BLUE, "This will have no effect! Select a better option.", 1);
                // Input is valid and passes all checks
            } else {
                globalData->keyStatus = 0;
                prints(0, 5, WHITE, BLUE, "                         ", 1);
                prints(0, 5, WHITE, BLUE, "Please select an attack: ", 1);
            }
        }
    }

    // Thing to consider: User pressing a key multiple times???
    // Regame.turn the amount of damage the move makes
    switch (globalData->keyPress) {
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

void sendMove() {
    game.myMove;
}

// Xbee: Receive move from opponent

void recieveMove() {
    game.oppMove = 0;
}

// Xbee: Send my score to opponent after taking damage

void sendScore() {
    game.myScore;
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
    clean(BLACK);
    prints(0, 0, YELLOW, BLACK, "NAME: ", 1);
    printrs(36, 0, YELLOW, BLACK, game.name, 1);
    if (game.turn) {
        prints(0, 10, YELLOW, BLACK, "Your move!", 1);
    } else {
        prints(0, 10, YELLOW, BLACK, "Opponent's turn.", 1);
    }

    prints(0, 30, YELLOW, BLACK, "Your Score: ", 1);
    integerprint(0, 45, YELLOW, BLACK, game.myScore, 1);
    prints(0, 60, WHITE, BLACK, "Opponent Score: ", 1);
    integerprint(0, 75, WHITE, BLACK, game.oppScore, 1);

    prints(0, H - 8, WHITE, BLACK, "Press D to continue.", 1);
}

void printMultiplayerSetup(GlobalState* globalData) {
    // LCD menu
    clean(GREEN);
    drawBoxFill(0, 0, 20, V - 1, GREEN);
    drawBox(0, 0, 20, V - 1, 2, WHITE);
    prints(35, 7, WHITE, BLACK, "Main Menu", 1);
    prints(35, globalData->mainMenuSpots[0], WHITE, GREEN, "Host Game", 1);
    prints(35, globalData->mainMenuSpots[1], WHITE, GREEN, "Join Game", 1);
    prints(35, globalData->mainMenuSpots[2], WHITE, GREEN, "Nevermind.", 1);
    prints(0, H - 8, WHITE, BLUE, "2-UP,8-DOWN,D-ENTER", 1);
    prints(25, globalData->mainMenuSpots[globalData->cursorPos], WHITE, GREEN, ">", 1);
}

void printKeyboard(GlobalState* globalData, char* name) {
    short pos[] = {0, 0};
    short letters = 0;
    clean(BLUE);
    prints(0, H-32, WHITE, BLUE, "B-DELETE",1);
    prints(0, H-24, WHITE, BLUE, "2-UP,8-DOWN",1);
    prints(0, H-16, WHITE, BLUE, "4-LEFT,6-RIGHT",1);
    prints(0, H-8, WHITE, BLUE, "D-SEL,#-DONE",1);
    prints(0, 0, WHITE, BLUE, "NAME?: _ _ _ _",1);
    drawBox(0, 16, 40, V - 1, 2, WHITE);
    drawBox(0, 19, 34, V - 1, 1, WHITE);
    prints(3, 23, WHITE, BLUE, " A B C D E F G H I J", 1);
    prints(3, 33, WHITE, BLUE, " K L M N O P Q R S T", 1);
    prints(3, 43, WHITE, BLUE, " U V W X Y Z [ \\ ] ^", 1);
    prints(3+12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, ">", 1);
    keypad(globalData);

    while (globalData->keyPress != 0x0F) {
        keypad(globalData);
        if (globalData->keyFlag && !globalData->displayedKey) {
            globalData.keyFlag = FALSE;
            globalData.displayedKey = TRUE;

            switch (globalData->keyPress) {
                case 0x08:
                    prints(3+ 12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, " ", 1);
                    if (pos[1] < 2) {
                        pos[1]++;
                    }
                    prints(3+12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, ">", 1);
                    break;
                case 0x02:
                    prints(3+12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, " ", 1);
                    if (pos[1] > 0) {
                        pos[1]--;
                    }
                    prints(3+12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, ">", 1);
                    break;
                case 0x04:
                    prints(3+12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, " ", 1);
                    if (pos[0] > 0) {
                        pos[0]--;
                    }
                    prints(3+12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, ">", 1);
                    break;
                case 0x06:
                    prints(3+12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, " ", 1);
                    if (pos[0] < 9) {
                        pos[0]++;
                    }
                    prints(3+12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, ">", 1);
                    break;
                case 0x0D:
                    if (letters < 4) {
                        name[letters] = (char) (65 + pos[0] + 10 * pos[1]);
                        name[letters + 1] = '\0';
                        ASCII(42 + 12 * letters, 0, WHITE, BLUE, name[letters], 1);
                        letters = letters + 1;
                        if (letters == 4) {
                            prints(42 + 12 * 5, 0, WHITE, BLUE, "end", 1);
                        }
                    }
                    break;
                case 0x0B:
                    if (letters > 0) {
                        if (letters == 4) {
                            prints(42 + 12 * 5, 0, WHITE, BLUE, "   ", 1);
                        }
                        letters--;
                        ASCII(42 + 12 * letters, 0, WHITE, BLUE, '_', 1);
                        name[letters] = '\0';
                    }
                    break;

            }
        }
    }

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
    drawBoxFill(30, 129, 8, 45, BLACK);
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
    integerprint(0, 48, WHITE, BLUE, globalData->selectMove[card][0], 1);
    prints(25, 48, WHITE, BLUE, "damage", 1);

    prints(0, 80, WHITE, BLUE, "B. Attack with: ", 1);
    integerprint(0, 88, WHITE, BLUE, globalData->selectMove[card][1], 1);
    prints(25, 88, WHITE, BLUE, "damage", 1);

    prints(0, 120, WHITE, BLUE, "C. Attack with: ", 1);
    integerprint(0, 128, WHITE, BLUE, globalData->selectMove[card][2], 1);
    prints(25, 128, WHITE, BLUE, "damage", 1);
}

// Displays Game Results
void printResults() {
    // Beep off
    TRISBbits.RB5 = 1;
    prints(0, 15, YELLOW, BLACK, "GAME OVER", 1);

    if (game.myScore > game.oppScore) {
        clean(BLACK);
        prints(0, 15, YELLOW, BLACK, "You won!", 1);
        prints(0, 30, YELLOW, BLACK, "Your Score: ", 1);
        integerprint(0, 45, YELLOW, BLACK, game.myScore, 1);
        prints(0, 60, WHITE, BLACK, "Opponent Score: ", 1);
        integerprint(0, 75, WHITE, BLACK, game.oppScore, 1);
    } else {
        clean(RED);
        prints(0, 15, BLACK, RED, "You lost", 1);
        prints(0, 30, BLACK, RED, "Your Score: ", 1);
        integerprint(0, 45, WHITE, RED, game.myScore, 1);
        prints(0, 60, YELLOW, RED, "Opponent Score: ", 1);
        integerprint(0, 75, YELLOW, RED, game.oppScore, 1);
    }
}
