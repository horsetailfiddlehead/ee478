typedef struct Move
{
	char *name;			// Name of move
	int damage;			// Amount of damage
	struct Move next;	// Next attack on the list
} Move;



