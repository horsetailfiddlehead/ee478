

void moveInit(Move *moveList) {
	moveList->name = NULL;
	moveList->damage = 0;
	moveList->next = NULL;
}

void* createMove(char* name, int damage) {
	Move m = 
	m->name = name;
	m->damage = damage;
	return *m;
}

void addMove(Move *moveList, char* name, int damage) {
	if(moveList == NULL) {
		moveList = createMove(moveList,
	}
		newMove->next = moveList;
		moveList = newMove;
}

void addMove (char* name, int damage) {
	if(moveList == NULL) {
		moveList = createMove(
	}
}