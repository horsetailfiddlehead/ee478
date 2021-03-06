/* 


 */
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "game.h"
#include "LCD.h"

#define HEALTH 100


gameData game;
Monster myMonsterList[3];
Monster customMoster;

void setupGame() {
    game.myScore = HEALTH;
    game.oppScore = HEALTH;
    game.turn = 0;
    game.gameOver = 0;
    game.monSel = 0;
    game.moveSel = 0;
    game.myMove = NULL;
    game.oppMove = NULL;
    game.myMonster = NULL;
    game.oppMonster = NULL;
    srand(ReadTimer0());
}

void singlePlayer(GlobalState* globalData) {
    static int tempScore = 0;
    static int transition = 0;
    static int movedone = 0;
    // First time playing?
    // Read first time from SRAM
    if (globalData->firstTime == TRUE) {
        if (globalData->newKeyboard) {
            printKeyboard(globalData, "NAME?", 5);
        } else {
            processKeyboard(globalData, game.name, 5);
        }
    }
    if (globalData->doneKeyboard) {
        globalData->firstTime = FALSE;


        if (globalData->newGame) {
            globalData->newGame = 0;
            // Setup new game
            setupGame();
            tempScore = 0;
            transition = 0;
            game.turn = rand() % 2;
            printGame(globalData);
            getCards(); // Assume cards already read by interrupts on switches (?)
        }


        // Wait till someone presses D to continue
        if (globalData->keyPress == 0x0D) {
            transition = 1;
        }

        // Begin game with computer
        if (transition) {
            if (!game.gameOver) {
                // Situation depends on game.turn
                if (game.turn) {
                    if (game.moveSel == 0) {
                        pickMove(globalData);
                    }
                    if (game.moveSel == 1) {

                        tempScore = game.oppScore;
                        game.oppScore = attack(game.myMove, game.myMonster, game.oppScore);
                        if (game.oppScore == tempScore) {
                            prints(0, 10, RED, BLACK, "                    ", 1);
                            prints(0, 18, RED, BLACK, "                    ", 1);
                            prints(0, 26, RED, BLACK, "                    ", 1);
                            prints(0, 10, RED, BLACK, "Missed!", 1);
                        } else {
                            prints(0, 10, RED, BLACK, "                    ", 1);
                            prints(0, 18, RED, BLACK, "                    ", 1);
                            prints(0, 26, RED, BLACK, "                    ", 1);
                            prints(0, 10, RED, BLACK, "Your", 1);
                            printrs(30, 10, RED, BLACK, game.myMonster->monsterName, 1);
                            prints(84, 10, RED, BLACK, "used", 1);
                            printrs(0, 18, RED, BLACK, game.myMove->moveName, 1);
                            prints(0, 26, RED, BLACK, "-    ", 1);
                            integerprint(6, 26, RED, BLACK, tempScore - game.oppScore, 1);
                        }
                        game.moveSel = 0;
                        movedone = 1;
                    }
                } else {
                    // Computer randomly picks a monster and attack
                    game.oppMonster = &myMonsterList[rand() % 3 ];
                    game.oppMove = &game.oppMonster->movelist[rand() % 3];
                    tempScore = game.myScore;
                    game.myScore = attack(game.oppMove, game.oppMonster, game.myScore);
                    if (game.myScore == tempScore) {
                        prints(0, 10, RED, BLACK, "                    ", 1);
                        prints(0, 18, RED, BLACK, "                    ", 1);
                        prints(0, 26, RED, BLACK, "                    ", 1);
                        prints(0, 10, RED, BLACK, "Enmy Missed!", 1);
                    } else {
                        prints(0, 10, RED, BLACK, "                    ", 1);
                        prints(0, 18, RED, BLACK, "                    ", 1);
                        prints(0, 26, RED, BLACK, "                    ", 1);
                        prints(0, 10, RED, BLACK, "Enmy", 1);
                        printrs(30, 10, RED, BLACK, game.oppMonster->monsterName, 1);
                        prints(84, 10, RED, BLACK, "used", 1);
                        printrs(0, 18, RED, BLACK, game.oppMove->moveName, 1);
                        prints(0, 26, RED, BLACK, "-    ", 1);
                        integerprint(6, 26, RED, BLACK, tempScore - game.myScore, 1);
                    }
                    movedone = 1;
                }

                if (movedone == 1) {
                    prints(0, 40, YELLOW, BLACK, "Your Score: ", 1);
                    prints(0, 55, YELLOW, BLACK, "     ", 1);
                    integerprint(0, 55, YELLOW, BLACK, game.myScore, 1);
                    prints(0, 70, WHITE, BLACK, "Opponent Score: ", 1);
                    prints(0, 85, WHITE, BLACK, "     ", 1);
                    integerprint(0, 85, WHITE, BLACK, game.oppScore, 1);
                    // Check game status
                    game.gameOver = gameStatus();
                    game.turn = !game.turn;
                    transition = 0;
                    movedone = 0;
                }
                //                if (game.gameOver == 1) {
                //                    if (globalData->newGame == 0) {
                //                        printResults();
                //                    }
                //                }
            } else {
                if (globalData->newGame == 0) {
                    printResults();
                }
            }
        }
        // Display results once there is a lost
    }
}

void multiPlayer(GlobalState* globalData) {
    int connect = 0;
    int hostOrFind = 0; // 0 host game, 1 find game
    char* mySelections[2] = {"Host Game", "Find Game"};
    setupXbee();
    printMenu(mySelections, BLACK, GRAY, WHITE, YELLOW, 2);
    prints(25, 7, YELLOW, GRAY, "Multiplayer", 1);
    processPrintCursor(globalData, 2, BLACK, YELLOW);
    switch (hostOrFind) {
        case 0:
            prints(3, 48, YELLOW, GRAY, "Waiting for players...", 1);
            hostGame();
            break;
        case 1:
            prints(3, 88, YELLOW, GRAY, "Looking for games...", 1);
            findGame();
            break;
        case 0xFF:
            return;
            break;
    }



    setupGame();

    //    // Find other players
    //    while (!connect) {
    //        connect = findPlayer();
    //    }
    //
    //    // NOTE: Need to build something to determine who goes first
    //    // Compare Xbee ID's perhaps?
    //
    //    // Begin game
    //    while (!game.gameOver && connect) {
    //        if (game.turn) {
    //            // Pick Move
    //            game.myMove = pickMove(globalData);
    //            // Send Move
    //            sendMove();
    //            // Receive new score after opponent takes damage
    //            game.oppScore = receiveScore();
    //            printGame(globalData);
    //            prints(0, 10, RED, BLACK, "                    ", 1);
    //            prints(0, 10, RED, BLACK, "Opponent took damage:", 1);
    //            prints(0, 21, RED, BLACK, "-", 1);
    //            integerprint(6, 21, RED, BLACK, game.myMove, 1);
    //        } else {
    //            // Receive player move and take damage
    //            game.myScore = attack(receiveMove(), game.myScore);
    //            prints(0, 10, RED, BLACK, "                    ", 1);
    //            prints(0, 10, RED, BLACK, "Taken damage:", 1);
    //            prints(0, 21, RED, BLACK, "-", 1);
    //            integerprint(6, 21, RED, BLACK, game.oppMove, 1);
    //            // Send new score
    //            sendScore();
    //        }
    //        prints(0, 45, YELLOW, BLACK, "        ", 1);
    //        integerprint(0, 45, YELLOW, BLACK, game.myScore, 1);
    //        prints(0, 75, WHITE, BLACK, "        ", 1);
    //        integerprint(0, 75, WHITE, BLACK, game.oppScore, 1);
    //        // Check game status
    //        game.gameOver = gameStatus();
    //        game.turn =!game.turn;
    //    }
    //    if (!connect) {
    //        printBSOD();
    //    } else {
    //        printResults();
    //    }
}


// Program data on the card
// Monster ID
// Attack ID

void buildCard(GlobalState* globalData, int sel) {
    char myCommand[32];

    switch (sel) {
        case 0:
            // firedude
            // Enter data to be stored in card
            myCommand[0] = WRITE_CARD_BLOCK; // Write 32-bit data to card block
            myCommand[1] = 0x00; // slot
            myCommand[2] = 0x00; // block
            strcpypgm2ram(&myCommand[3], "FIRE");
            sendBytes(myCommand, 7);
            Delay1KTCYx(250);

            myCommand[0] = WRITE_CARD_BLOCK; // Write 32-bit data to card block
            myCommand[1] = 0x00; // slot
            myCommand[2] = 0x01; // block
            strcpypgm2ram(&myCommand[3], "DUDE");
            sendBytes(myCommand, 7);
            Delay1KTCYx(250);

            Delay1KTCYx(250);
            myCommand[0] = WRITE_CARD_BLOCK; // Write 32-bit data to card block
            myCommand[1] = 0x00; // slot
            myCommand[2] = 0x02; // block
            myCommand[3] = 0x00; // type
            myCommand[4] = 0x01; // level
            myCommand[5] = 0x10; // mov1mov2 scratch,ember
            myCommand[6] = 0x02; // mov3 blank, hotflame
            sendBytes(myCommand, 7);

            break;
        case 1:
            // earthguy
            // Enter data to be stored in card
            myCommand[0] = WRITE_CARD_BLOCK; // Write 32-bit data to card block
            myCommand[1] = 0x00; // slot
            myCommand[2] = 0x00; // block
            strcpypgm2ram(&myCommand[3], "EART");
            sendBytes(myCommand, 7);
            Delay1KTCYx(250);

            myCommand[0] = WRITE_CARD_BLOCK; // Write 32-bit data to card block
            myCommand[1] = 0x00; // slot
            myCommand[2] = 0x01; // block
            strcpypgm2ram(&myCommand[3], "HGUY");
            sendBytes(myCommand, 7);
            Delay1KTCYx(250);

            Delay1KTCYx(250);
            myCommand[0] = WRITE_CARD_BLOCK; // Write 32-bit data to card block
            myCommand[1] = 0x00; // slot
            myCommand[2] = 0x02; // block
            myCommand[3] = 0x02; // type
            myCommand[4] = 0x01; // level
            myCommand[5] = 0x43; // mov1mov2 pound,pebble
            myCommand[6] = 0x05; // mov3 blank,earthquake
            sendBytes(myCommand, 7);
            break;
        case 2:
            // waterman
            // Enter data to be stored in card
            myCommand[0] = WRITE_CARD_BLOCK; // Write 32-bit data to card block
            myCommand[1] = 0x00; // slot
            myCommand[2] = 0x00; // block
            strcpypgm2ram(&myCommand[3], "WATE");
            sendBytes(myCommand, 7);
            Delay1KTCYx(250);

            myCommand[0] = WRITE_CARD_BLOCK; // Write 32-bit data to card block
            myCommand[1] = 0x00; // slot
            myCommand[2] = 0x01; // block
            strcpypgm2ram(&myCommand[3], "RMAN");
            sendBytes(myCommand, 7);
            Delay1KTCYx(250);

            Delay1KTCYx(250);
            myCommand[0] = WRITE_CARD_BLOCK; // Write 32-bit data to card block
            myCommand[1] = 0x00; // slot
            myCommand[2] = 0x02; // block
            myCommand[3] = 0x01; // type
            myCommand[4] = 0x01; // level
            myCommand[5] = 0x16; // mov1mov2 scratch,squirt
            myCommand[6] = 0x07; // mov3 blank, soak
            sendBytes(myCommand, 7);
            break;
        case 3:
            // peckol
            // Enter data to be stored in card
            myCommand[0] = WRITE_CARD_BLOCK; // Write 32-bit data to card block
            myCommand[1] = 0x00; // slot
            myCommand[2] = 0x00; // block
            strcpypgm2ram(&myCommand[3], "DRPE");
            sendBytes(myCommand, 7);
            Delay1KTCYx(250);

            myCommand[0] = WRITE_CARD_BLOCK; // Write 32-bit data to card block
            myCommand[1] = 0x00; // slot
            myCommand[2] = 0x01; // block
            strcpypgm2ram(&myCommand[3], "CKOL");
            sendBytes(myCommand, 7);
            Delay1KTCYx(250);

            Delay1KTCYx(250);
            myCommand[0] = WRITE_CARD_BLOCK; // Write 32-bit data to card block
            myCommand[1] = 0x00; // slot
            myCommand[2] = 0x02; // block
            myCommand[3] = 0x02; // type
            myCommand[4] = 0x09; // level
            myCommand[5] = 0xFF; // mov1mov2 FAIL,FAIL
            myCommand[6] = 0x0F; // mov3 blank, FAIL
            sendBytes(myCommand, 7);
            Delay1KTCYx(250);

            myCommand[0] = REQUEST_CARD_DATA; // Write 32-bit data to card block
            myCommand[1] = 0x00; // slot
            myCommand[2] = 0x00; // block
            sendBytes(myCommand, 3);
            Delay1KTCYx(250);
            Delay1KTCYx(250);
            break;
    }




    globalData->newDisplay = 1;
    globalData->displayPage = 0;
    // peckol card
    // firedude
    // earthguy
    // waterman
    // combogrl

    // Write data to card

}

/* The following functions are helper commands for the main games.
 * Set Up
 * Moves
 * Display
 */

// Sets up connection with other players

int findPlayer(GlobalState* globalData) {
    //    printMultiplayerSetup(globalData);
    return 0;
}

// Checks if a winner has been found

int gameStatus() {
    return (0 == game.oppScore || 0 == game.myScore);
}

// Regame.turns new score of player after taking damage

int attack(Move* attack, Monster* monster, int targetScore) {
    int hitPCT = (3 * monster->level) + 75;
    int totalDmg = attack->baseDamage + 2 * (monster->level);

    if (hitPCT > (rand() % 101)) {

        if (attack->moveType == monster->monsterType) {
            totalDmg = (totalDmg * 120) / 100;
        } else if (attack->moveType == (Type) (((3 + monster->monsterType) - 1) % 3)) {
            totalDmg = (totalDmg * 80) / 100;
        }
    } else {
        totalDmg = 0;
    }
    if (totalDmg < targetScore) {
        return (targetScore - totalDmg);
    } else {
        return 0;
    }
}

// User selects move based off of available cards
// Regame.turns damage of chosen move

void selectCard(GlobalState* globalData) {
    static int displayed = 0;
    static int gotStuff = 0;

    // Beep off
    TRISBbits.RB5 = 1;
    if (displayed == 0) {
        displayed = 1;
    }

    if (gotStuff == 0) {
        gotStuff = printSelect(globalData);
    } else {

        if ((0 == globalData->keyPress || globalData->keyPress > 5)) {
            globalData->keyStatus = 1;
            prints(0, 5, WHITE, RED, "                                                 ", 1);
            prints(0, 5, WHITE, RED, "Invalid input. Please enter a key between 1 to 4:", 1);
            // No card available
        } else if (NULL == &myMonsterList[globalData->keyPress - 1]) {
            globalData->keyStatus = 2;
            prints(0, 5, WHITE, RED, "                                ", 1);
            prints(0, 5, WHITE, RED, "No card found. Please try again:", 1);
            // Selected card is available
        } else {
            if (globalData->keyPress == 0x04) {
                game.myMonster = &customMoster;
            } else {
                game.myMonster = &myMonsterList[globalData->keyPress - 1];
            }
            game.monSel = 1;
            displayed = 0;
            gotStuff = 0;
        }
    }
}

void getCards() {
    strcpypgm2ram(myMonsterList[0].monsterName, "FIREDUDE");
    myMonsterList[0].monsterID = 0x01;
    myMonsterList[0].monsterType = FIRE;
    myMonsterList[0].level = 2;
    strcpypgm2ram(myMonsterList[0].movelist[0].moveName, "EMBER");
    myMonsterList[0].movelist[0].baseDamage = 10;
    myMonsterList[0].movelist[0].moveType = FIRE;
    myMonsterList[0].movelist[0].uses = 10;
    strcpypgm2ram(myMonsterList[0].movelist[1].moveName, "SCRATCH");
    myMonsterList[0].movelist[1].baseDamage = 6;
    myMonsterList[0].movelist[1].moveType = EARTH;
    myMonsterList[0].movelist[1].uses = 15;
    strcpypgm2ram(myMonsterList[0].movelist[2].moveName, "HOTFLAME");
    myMonsterList[0].movelist[2].baseDamage = 20;
    myMonsterList[0].movelist[2].moveType = FIRE;
    myMonsterList[0].movelist[2].uses = 3;

    strcpypgm2ram(myMonsterList[1].monsterName, "EARTHGUY");
    myMonsterList[1].monsterID = 0x02;
    myMonsterList[1].monsterType = EARTH;
    myMonsterList[1].level = 1;
    strcpypgm2ram(myMonsterList[1].movelist[0].moveName, "PEBBLE");
    myMonsterList[1].movelist[0].baseDamage = 12;
    myMonsterList[1].movelist[0].moveType = EARTH;
    myMonsterList[1].movelist[0].uses = 5;
    strcpypgm2ram(myMonsterList[1].movelist[1].moveName, "POUND");
    myMonsterList[1].movelist[1].baseDamage = 3;
    myMonsterList[1].movelist[1].moveType = EARTH;
    myMonsterList[1].movelist[1].uses = 20;
    strcpypgm2ram(myMonsterList[1].movelist[2].moveName, "ROCKSLDE");
    myMonsterList[1].movelist[2].baseDamage = 30;
    myMonsterList[1].movelist[2].moveType = EARTH;
    myMonsterList[1].movelist[2].uses = 2;


    strcpypgm2ram(myMonsterList[2].monsterName, "WATERMAN");
    myMonsterList[2].monsterID = 0x03;
    myMonsterList[2].monsterType = WATER;
    myMonsterList[2].level = 1;
    strcpypgm2ram(myMonsterList[2].movelist[0].moveName, "SQUIRT");
    myMonsterList[2].movelist[0].baseDamage = 12;
    myMonsterList[2].movelist[0].moveType = WATER;
    myMonsterList[2].movelist[0].uses = 7;
    strcpypgm2ram(myMonsterList[2].movelist[1].moveName, "SCRATCH");
    myMonsterList[2].movelist[1].baseDamage = 6;
    myMonsterList[2].movelist[1].moveType = EARTH;
    myMonsterList[2].movelist[1].uses = 15;
    strcpypgm2ram(myMonsterList[2].movelist[2].moveName, "SOAK");
    myMonsterList[2].movelist[2].baseDamage = 15;
    myMonsterList[2].movelist[2].moveType = WATER;
    myMonsterList[2].movelist[2].uses = 5;

    //    strcpypgm2ram(myMonsterList[3].monsterName, "COMBOGRL");
    //    myMonsterList[3].monsterID = 0x04;
    //    myMonsterList[3].monsterType = EARTH;
    //    myMonsterList[3].level = 3;
    //    strcpypgm2ram(myMonsterList[3].movelist[0].moveName, "SOAK");
    //    myMonsterList[3].movelist[0].baseDamage = 15;
    //    myMonsterList[3].movelist[0].moveType = WATER;
    //    myMonsterList[3].movelist[0].uses = 5;
    //    strcpypgm2ram(myMonsterList[3].movelist[1].moveName, "PEBBLE");
    //    myMonsterList[3].movelist[1].baseDamage = 12;
    //    myMonsterList[3].movelist[1].moveType = EARTH;
    //    myMonsterList[3].movelist[1].uses = 5;
    //    strcpypgm2ram(myMonsterList[3].movelist[2].moveName, "EMBER");
    //    myMonsterList[3].movelist[2].baseDamage = 10;
    //    myMonsterList[3].movelist[2].moveType = FIRE;
    //    myMonsterList[3].movelist[2].uses = 10;



}

// User selects move based off of available cards
// Regame.turns damage of chosen move

void pickMove(GlobalState* globalData) {
    static int displaymove = 0;
    if (game.monSel == 0) {
        selectCard(globalData);
    } else {
        if (displaymove == 0) {
            printAttackMenu(globalData, game.myMonster);
            prints(8, 5, WHITE, BLUE, "Please select an attack: ", 1);
            displaymove = 1;
        } else {
            if (((0x0A > globalData->keyPress) || (globalData->keyPress > 0x0C))) {
                globalData->keyStatus = 1;
                prints(8, 5, WHITE, BLUE, "                                                            ", 1);
                prints(8, 5, WHITE, BLUE, "Invalid attack input. Please select from the options below: ", 1);
            } else {
                displaymove = 0;
                globalData->keyStatus = 0;
                switch (globalData->keyPress) {
                    case 0x0A:
                        game.myMove = &game.myMonster->movelist[0];
                        break;
                    case 0x0B:
                        game.myMove = &game.myMonster->movelist[1];
                        break;
                    case 0xC:
                        game.myMove = &game.myMonster->movelist[2];
                        break;
                }
                game.monSel = 0;
                game.moveSel = 1;
                game.myMove->uses--;
                printGame(globalData);
            }
        }
    }
}

// Thing to consider: User pressing a key multiple times???
// Regame.turn the amount of damage the move makes

/* Xbee functions for multiplayer
 *
 */

// Xbee: Send move to opponent

void sendMove() {
    game.myMove;
}

// Xbee: Receive move from opponent

void receiveMove() {
    game.oppMove = 0;
}

// Xbee: Send my score to opponent after taking damage

void sendScore() {
    game.myScore;
}

// Xbee: Receive new score from opponent after attacking

int receiveScore() {
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
        prints(0, 18, YELLOW, BLACK, "Your move!", 1);
    } else {
        prints(0, 18, YELLOW, BLACK, "Opponent's turn.", 1);
    }

    prints(0, 40, YELLOW, BLACK, "Your Score: ", 1);
    integerprint(0, 55, YELLOW, BLACK, game.myScore, 1);
    prints(0, 70, WHITE, BLACK, "Opponent Score: ", 1);
    integerprint(0, 85, WHITE, BLACK, game.oppScore, 1);

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

void printKeyboard(GlobalState* globalData, char* inputType, int size) {
    int i = 0;
    globalData->newKeyboard = 0;
    clean(BLUE);
    prints(0, H - 32, WHITE, BLUE, "B-DELETE", 1);
    prints(0, H - 24, WHITE, BLUE, "2-UP,8-DOWN", 1);
    prints(0, H - 16, WHITE, BLUE, "4-LEFT,6-RIGHT", 1);
    prints(0, H - 8, WHITE, BLUE, "D-SEL,#-DONE", 1);
    prints(0, 0, WHITE, BLUE, inputType, 1);
    for (i = 0; i < size - 1; i++) {
        prints(12 * i, 8, WHITE, BLUE, " _", 1);
    }
    drawBox(0, 16, 40, V - 1, 2, WHITE);
    drawBox(0, 19, 34, V - 1, 1, WHITE);
    prints(3, 23, WHITE, BLUE, " A B C D E F G H I J", 1);
    prints(3, 33, WHITE, BLUE, " K L M N O P Q R S T", 1);
    prints(3, 43, WHITE, BLUE, " U V W X Y Z [ \\ ] ^", 1);
    prints(3, 23, WHITE, BLUE, ">", 1);

    //    keypad(globalData);
    //
    //    while (globalData->keyPress != 0x0F) {
    //        keypad(globalData);
    //        if (globalData->keyFlag && !globalData->displayedKey) {
    //            globalData->keyFlag = FALSE;
    //            globalData->displayedKey = TRUE;
}

void processKeyboard(GlobalState* globalData, char* name, int size) {
    static short pos[] = {0, 0};
    static short letters = 0;

    if (globalData->newKeyboard == 1) {
        globalData->newKeyboard = 0;
        pos[0] = 0;
        pos[1] = 0;
        letters = 0;
    } else {

        switch (globalData->keyPress) {
            case 0x08:
                prints(3 + 12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, " ", 1);
                if (pos[1] < 2) {
                    pos[1]++;
                }
                prints(3 + 12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, ">", 1);
                break;
            case 0x02:
                prints(3 + 12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, " ", 1);
                if (pos[1] > 0) {
                    pos[1]--;
                }
                prints(3 + 12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, ">", 1);
                break;
            case 0x04:
                prints(3 + 12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, " ", 1);
                if (pos[0] > 0) {
                    pos[0]--;
                }
                prints(3 + 12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, ">", 1);
                break;
            case 0x06:
                prints(3 + 12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, " ", 1);
                if (pos[0] < 9) {
                    pos[0]++;
                }
                prints(3 + 12 * pos[0], 10 * pos[1] + 23, WHITE, BLUE, ">", 1);
                break;
            case 0x0D:
                if (letters < size - 1) {
                    name[letters] = (char) (65 + pos[0] + 10 * pos[1]);
                    name[letters + 1] = '\0';
                    ASCII(6 + 12 * letters, 8, WHITE, BLUE, name[letters], 1);
                    letters = letters + 1;
                    if (letters == size - 1) {
                        prints(42 + 12 * 5, 0, WHITE, BLUE, "end", 1);
                    }
                }
                break;
            case 0x0B:
                if (letters > 0) {
                    if (letters == size - 1) {
                        prints(42 + 12 * 5, 0, WHITE, BLUE, "   ", 1);
                    }
                    letters--;
                    ASCII(6 + 12 * letters, 8, WHITE, BLUE, '_', 1);
                    name[letters] = '\0';
                }
                break;
            case 0x0F:
                globalData->doneKeyboard = 1;
                globalData->newKeyboard = 1;
                break;
        }

    }
}




// Select a card to play

int printSelect(GlobalState* globalData) {
    char myCommand[16];
    static int gotDefaults = 0;
    int i = 0;
    // Beep off
    TRISBbits.RB5 = 1;


    // Display commands to select a slot - the LED's should indicate if a card is read
    //    drawBoxFill(15, 29, 26, 85, BLACK)
    if (gotDefaults == 0) {
                // LCD menu
        clean(RED);
        prints(0, 5, WHITE, RED, "Choose a card by its slot number:", 1);
        globalData->myRequestStatus = 0;
        for (i = 0; i < 3; i++) {
            drawBoxFill(15, 29 + 35 * i, 26, 85, BLACK);
            printrs(20, 38 + 35 * i, WHITE, BLACK, myMonsterList[i].monsterName, 1);
            prints(20, 38 + 8 + 35 * i, WHITE, BLACK, "Lvl:", 1);
            integerprint(38, 38 + 8 + 35 * i, WHITE, BLACK, myMonsterList[i].level, 1);
            switch (myMonsterList[i].monsterType) {
                case FIRE:
                    drawBoxFill(61, 38 + 8 + 35 * i - 1, 8, 30, RED);
                    prints(62, 38 + 8 + 35 * i, WHITE, RED, "FIRE", 1);
                    break;
                case WATER:
                    drawBoxFill(61, 38 + 8 + 35 * i - 1, 8, 35, CYAN);
                    prints(62, 38 + 8 + 35 * i, WHITE, CYAN, "WATER", 1);
                    break;
                case EARTH:
                    prints(62, 38 + 8 + 35 * i, WHITE, BLACK, "EARTH", 1);
                    break;
            }
        }
        gotDefaults = 1;
    }

    if (globalData->myRequestStatus == 0) {
        myCommand[0] = REQUEST_CARD_DATA; // Write 32-bit data to card block
        myCommand[1] = 0x00; // slot
        myCommand[2] = 0x00; // block
        sendBytes(myCommand, 3);
        Delay1KTCYx(250);
    } else if (globalData->myRequestStatus == 1) {
        customMoster.monsterName[0] = (char) (((globalData->dataBlock[0] - '0') << 4) | ((globalData->dataBlock[1] - '0')&0x0F));
        customMoster.monsterName[1] = (char) (((globalData->dataBlock[2] - '0') << 4) | ((globalData->dataBlock[3] - '0')&0x0F));
        customMoster.monsterName[2] = (char) (((globalData->dataBlock[4] - '0') << 4) | ((globalData->dataBlock[5] - '0')&0x0F));
        customMoster.monsterName[3] = (char) (((globalData->dataBlock[6] - '0') << 4) | ((globalData->dataBlock[7] - '0')&0x0F));

        myCommand[0] = REQUEST_CARD_DATA; // Write 32-bit data to card block
        myCommand[1] = 0x00; // slot
        myCommand[2] = 0x01; // block
        sendBytes(myCommand, 3);
        Delay1KTCYx(250);
    } else if (globalData->myRequestStatus == 2) {
        customMoster.monsterName[4] = (char) (((globalData->dataBlock[0] - '0') << 4) | ((globalData->dataBlock[1] - '0')&0x0F));
        customMoster.monsterName[5] = (char) (((globalData->dataBlock[2] - '0') << 4) | ((globalData->dataBlock[3] - '0')&0x0F));
        customMoster.monsterName[6] = (char) (((globalData->dataBlock[4] - '0') << 4) | ((globalData->dataBlock[5] - '0')&0x0F));
        customMoster.monsterName[7] = (char) (((globalData->dataBlock[6] - '0') << 4) | ((globalData->dataBlock[7] - '0')&0x0F));
        customMoster.monsterName[8] = '\0';

        myCommand[0] = REQUEST_CARD_DATA; // Write 32-bit data to card block
        myCommand[1] = 0x00; // slot
        myCommand[2] = 0x02; // block
        sendBytes(myCommand, 3);
        Delay1KTCYx(250);
    } else if (globalData->myRequestStatus == 3) {
        customMoster.monsterType = (Type) (globalData->dataBlock[0] - '0');
        customMoster.level = (globalData->dataBlock[3] - '0');
        customMoster.movelist[0] = getMoveFromList(globalData->dataBlock[4]);
        customMoster.movelist[1] = getMoveFromList(globalData->dataBlock[4]);
        customMoster.movelist[2] = getMoveFromList(globalData->dataBlock[4]);

        drawBoxFill(15, 29 + 35 * 3, 26, 85, BLACK);
        printrs(20, 38 + 35 * 3, WHITE, BLACK, customMoster.monsterName, 1);
        prints(20, 38 + 8 + 35 * 3, WHITE, BLACK, "Lvl:", 1);
        integerprint(38, 38 + 8 + 35 * 3, WHITE, BLACK, customMoster.level, 1);
        switch (customMoster.monsterType) {
            case FIRE:
                drawBoxFill(61, 38 + 8 + 35 * 3 - 1, 8, 30, RED);
                prints(62, 38 + 8 + 35 * 3, WHITE, RED, "FIRE", 1);
                break;
            case WATER:
                drawBoxFill(61, 38 + 8 + 35 * 3 - 1, 8, 35, CYAN);
                prints(62, 38 + 8 + 35 * 3, WHITE, CYAN, "WATER", 1);
                break;
            case EARTH:
                prints(62, 38 + 8 + 35 * 3, WHITE, BLACK, "EARTH", 1);
                break;
        }
        globalData->myRequestStatus = 0;
        gotDefaults = 0;
        prints(20, 30, WHITE, BLACK, "Slot 1", 1);
        prints(20, 65, WHITE, BLACK, "Slot 2", 1);
        prints(20, 100, WHITE, BLACK, "Slot 3", 1);
        prints(20, 135, WHITE, BLACK, "Slot 4", 1);
        return 1;
    }
    return 0;
}


    //            0 0       0 1     1 0       02
    //
    //            30 30    30 31    31 30    30 32
    //
    //            Delay1KTCYx(250);
    //            myCommand[0] = WRITE_CARD_BLOCK; // Write 32-bit data to card block
    //            myCommand[1] = 0x00; // slot
    //            myCommand[2] = 0x02; // block
    //
    //            myCommand[3] = 0x00; // type
    //            myCommand[4] = 0x01; // level
    //            myCommand[5] = 0x10; // mov1mov2 scratch,ember
    //            myCommand[6] = 0x02; // mov3 blank, hotflame
    //            sendBytes(myCommand, 7);




Move getMoveFromList(char id) {
    Move newMove;
    switch (id) {
        case 'F':
            strcpypgm2ram(newMove.moveName, "FAIL");
            newMove.baseDamage = 100;
            newMove.moveType = EARTH;
            newMove.uses = 10;
            break;
        default:
            strcpypgm2ram(newMove.moveName, "SCRATCH");
            newMove.baseDamage = 6;
            newMove.moveType = EARTH;
            newMove.uses = 15;
            break;
    }
    return newMove;

}

// Select an attack.

void printAttackMenu(GlobalState* globalData, Monster* card) {
    int i = 0;
    // Beep off
    TRISBbits.RB5 = 1;

    // LCD menu
    clean(BLUE);

    switch (card->monsterType) {
        case FIRE:
            drawBoxFill(0, 31, 8, 48, RED);
            printrs(0, 32, WHITE, RED, card->monsterName, 1);
            break;
        case WATER:
            drawBoxFill(0, 31, 8, 48, CYAN);
            printrs(0, 32, WHITE, CYAN, card->monsterName, 1);
            break;
        case EARTH:
            drawBoxFill(0, 31, 8, 48, BLACK);
            printrs(0, 32, WHITE, BLACK, card->monsterName, 1);
            break;
    }
    prints(54, 32, WHITE, BLUE, "Lvl:", 1);
    integerprint(84, 32, WHITE, BLUE, card->level, 1);

    prints(0, 40, YELLOW, BLUE, "A. Attack with:", 1);
    prints(0, 80, YELLOW, BLUE, "B. Attack with: ", 1);
    prints(0, 120, YELLOW, BLUE, "C. Attack with: ", 1);

    for (i = 0; i < 3; i++) {
        switch (card->movelist[i].moveType) {
            case FIRE:
                drawBoxFill(0, 40 + 40 * i + 8 - 1, 8, 83, RED);
                printrs(0, 40 + 40 * i + 8, WHITE, RED, card->movelist[i].moveName, 1);
                prints(54, 40 + 40 * i + 8, WHITE, RED, "FIRE", 1);
                break;
            case WATER:
                drawBoxFill(0, 40 + 40 * i + 8 - 1, 8, 83, CYAN);
                printrs(0, 40 + 40 * i + 8, WHITE, CYAN, card->movelist[i].moveName, 1);
                prints(54, 40 + 40 * i + 8, WHITE, CYAN, "WATER", 1);
                break;
            case EARTH:
                drawBoxFill(0, 40 + 40 * i + 8 - 1, 8, 83, BLACK);
                printrs(0, 40 + 40 * i + 8, WHITE, BLACK, card->movelist[i].moveName, 1);
                prints(54, 40 + 40 * i + 8, WHITE, BLACK, "EARTH", 1);
                break;
        }
        prints(0, 40 + 40 * i + 16, WHITE, BLUE, "Min damage:", 1);
        integerprint(60, 40 + 40 * i + 16, YELLOW, BLUE, card->movelist[i].baseDamage, 1);
        prints(0, 40 + 40 * i + 24, WHITE, BLUE, "Uses left:", 1);
        integerprint(72, 40 + 40 * i + 24, YELLOW, BLUE, card->movelist[i].uses, 1);
    }
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
    if (globalData.keyPress == 0x0B) {
        globalData.displayPage = 0;
        globalData.newGame = 1;
    }
}
