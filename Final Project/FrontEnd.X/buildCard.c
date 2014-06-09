void printBuildMenu(GlobalState* globalData) {
	int i = 0;
	
	prints(7, 10, BLACK, RED, "Available Cards:", 1);
	processPrintCursor(globalData, size, RED, BLACK);
}

void buildCardMenu(GlobalState* globalData) {
	char* buildOptions[3] = {"Add Name", "Add Move", "Remove Move"};
	int build;
	printMenu(buildOptions, PINK, WHITE, BLACK, BLACK, 3};
	build = processPrintCursor(globalData, 3, BLACK, PINK);
	switch(build) {
		// Add Name
		case 0:
			addName();
			break;
		// Add Move
		case 1:
			addMove();
			break;
		// Remove Move
		case 2:
			removeMove();
			break;
		// Go Back
		case 0xFF:
			globalData->goBack = TRUE;
			break;
	}
}

void printAddNameMenu(GlobalState* globalData) {
	clean(RED);
	prints(35, 7, RED, BLACK, "Add the following name?", 1);
	
	// Draw Box with name inside
    drawBoxFill(0, 20, 20, V - 1, WHITE);
    drawBoxBorder(0, 20, 20, V - 1, 2, BLACK, 8);
	printrs(35, 25, BLACK, WHITE, name, 1);
	prints(35, 40, RED, BLACK, "1. Yes", 1);
	prints(35, 60, RED, BLACK, "2. No", 1);
	prints(0, H - 8, RED, BLACK, "Press B to return.", 1);
}

void addName(GlobalState* globalData) {
	char* name[9] = "";
	printKeyboard(globalData, name, "NAME?", 9);
	
}


/*         //Doing an inventory command from the Build card menu
        if (globalData.getInventory == TRUE) {
            // get the inventory of cards
            inventoryRFID();

            // Print out each on to the LCD
            for (i = 0; i < readerData.availableUIDs; i++) {
                if (readerData.readUID[i][0] != ',') {
                    // Get rid of commas
                    processUID(readerData.readUID[i]);
                    printrs(0, 24 + 8 * i, BLACK, RED, readerData.readUID[i], 1); // print first UID
                }
            }
            // Tell UID to be quiet - Works but needs to have at least one uid in this state
            // quietRFID(readerData.readUID[0]);

            if (readerData.availableUIDs > 0) {

                // block 0 high bits being 0x0000 indicates factory card, not custom
                // block 0 high bits being 0x0001 indicates custom card
                // block 0 low bits indicate the game the card is for. 0x0001 is the monster game
                writeRFID(readerData.readUID[0], 0x00, 0x0000, 0x0001); // 7654 3210
                writeRFID(readerData.readUID[0], 0x01, 0x0010, 0x0001); // hex 7-5 are for level 0x001 is level 1
                // hex 4 is for type 0 1 2 3
                // 0x0 fire, 0x1 water, 0x3 earth
                // last 4 are monster ID

                //writes 8 chars in 2 addresses of memory (0x02 and 0x03 here)
                char8RFID(readerData.readUID[0], 0x02, "FIREDUDE");
                writeRFID(readerData.readUID[0], 0x04, 0x0003, 0x0201); // Move list by id, has moves 03, 02, and 01
                readRFID(readerData.readUID[0], 0x00);
                printrs(0, 32, BLACK, RED, readerData.readData, 1); // print 1st block
                readRFID(readerData.readUID[0], 0x01);
                printrs(0, 40, BLACK, RED, readerData.readData, 1); // print 2nd block
                readRFID(readerData.readUID[0], 0x02);
                printrs(0, 48, BLACK, RED, readerData.readData, 1); // print 3rd block
                readRFID(readerData.readUID[0], 0x03);
                printrs(0, 56, BLACK, RED, readerData.readData, 1); // print 4th block
                readRFID(readerData.readUID[0], 0x04);
                printrs(0, 64, BLACK, RED, readerData.readData, 1); // print 5th block
            }


            readRFID(readerData.readUID[0], 0x01);
            // Print out block on to the LCD
            for (j = 0; j < readerData.availableUIDs; j++) {
                if (readerData.readUID[j][0] != ',') {
                    // Get rid of commas
                    printrs(0, 24 + 8 * i + 8 * j, BLACK, RED, readerData.readUID[j], 1); // print first UID
                }
            }

            prints(0, H - 8, BLACK, RED, "Press B to go back.", 1);
            // Turn off inventory flag

            globalData.getInventory = FALSE;
        } */