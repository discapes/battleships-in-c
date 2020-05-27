#include "configuration.h"
#include "missile.h"
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int width, height;
int ships[MAX_SHIPS][SHIP_MAXLENGTH][3];
char shipNames[MAX_SHIPS][20];
/* Ship1  Ship2  Ship3
   A,1,0  C,5,1  E,0,0
   A,2,1  D,5,1  E,1,0

   [3] = X,Y,isHit	*/
int numShips;

int colToInt(char letter);
int rowToInt(char digit);

/* Returns:
   -2 = MISS
   -1 = HIT BUT NOT SUNK
 0-12 = NUMBER OF SHIP THAT WAS SUNK */
int server_hit(int attackColumn, int attackRow) {
	for (int ship = 0; ship < numShips; ship++) {
		for (int block = 0; block < SHIP_MAXLENGTH; block++) {
			if ((ships[ship][block][0] == attackColumn) && (ships[ship][block][1] == attackRow) && (ships[ship][block][2] == 0)) {
				/* HIT */
				ships[ship][block][2] = 1;

				/* SUNK? */
				for (int blockInCurrentShip = 0; blockInCurrentShip < SHIP_MAXLENGTH; blockInCurrentShip++) {
					/* IF A BLOCK IN THE SHIP IS NOT YET HIT, THE SHIP DOESN'T SINK */
					if (ships[ship][blockInCurrentShip][2] == 0) return -1; /* HIT BUT NOT SUNK */
				}
				return ship; /* NUMBER OF SHIP THAT WAS SUNK */
			}
		}
	}
	return -2 /* MISS*/ ;
}

bool server_isShip(int x, int y) {
	for (int ship = 0; ship < numShips; ship++) {
		for (int block = 0; block < SHIP_MAXLENGTH; block++) {
			if ((ships[ship][block][0] == x) && (ships[ship][block][1] == y) && (ships[ship][block][2] == 0)) {
				return true;
			}
		}
	}
	return false;
}
/* Initializes the server with everything from the board file. Returns true if errors */
bool server_initializeBoard(char* fileName) {



	FILE* file = fopen(fileName, "r");
	if (file == 0) return true;
	fscanf(file, "%d,%d\n", &width, &height);

	if (!(width >= 1 && width <= MAX_WIDTH)) {
		printf("Invalid width!");
		return true;
	}
	if (!(height >= 1 && height <= MAX_HEIGHT)) {
		printf("Invalid height!");
		return true;
	}

	char head[2];
	char direction;
	int x, y;
	int length;
	char name[20];


	static int freeSlot = 0;
	bool eof = false;
	while(!eof) {
		if ((fscanf(file, "%c%c %c %d %s\n", &(head[0]), &(head[1]), &direction, &length, name)) != EOF) {
			x = colToInt(head[0]);
			y = rowToInt(head[1])-1; /* THE ROWS START AT 0!1!!!11!!!!!! */
			direction = tolower(direction);
			strcpy(shipNames[freeSlot], name);
			switch(direction) {
				case 'n': /* TODO optimize */
					for (int offset = 0; offset < SHIP_MAXLENGTH; offset++) {
						if((!(y+offset < height) && (offset < length))) {
							printf("Invalid board configuration");
							return true;
						} else {
							ships[freeSlot][offset][0] = x;
							ships[freeSlot][offset][1] = y+offset;
							if (offset >= length) {
								ships[freeSlot][offset][2] = 1;
							} else {
								ships[freeSlot][offset][2] = 0;
							}
						}
					}
					break;
				case 's':
					for (int offset = 0; offset < SHIP_MAXLENGTH; offset++) {
						if((y-offset < 0) && (offset < length)) {
							printf("Invalid board configuration");
							return true;
						} else {
							ships[freeSlot][offset][0] = x;
							ships[freeSlot][offset][1] = y-offset;
							if (offset >= length) {
								ships[freeSlot][offset][2] = 1;
							} else {
								ships[freeSlot][offset][2] = 0;
							}
						}
					}
					break;
				case 'w':
					for (int offset = 0; offset < SHIP_MAXLENGTH; offset++) {
						if((!(x+offset < width)) && (offset < length)) {
							printf("Invalid board configuration");
							return true;
						} else {
							ships[freeSlot][offset][1] = y;
							ships[freeSlot][offset][0] = x+offset;
							if (offset >= length) {
								ships[freeSlot][offset][2] = 1;
							} else {
								ships[freeSlot][offset][2] = 0;
							}
						}
					}
					break;
				case 'e':
					for (int offset = 0; offset < SHIP_MAXLENGTH; offset++) {
						if((x-offset < 0) && (offset < length)) {
							printf("Invalid board configuration");
							return true;
						} else {
							ships[freeSlot][offset][1] = x;
							ships[freeSlot][offset][0] = x-offset;
							if (offset >= length) {
								ships[freeSlot][offset][2] = 1;
							} else {
								ships[freeSlot][offset][2] = 0;
							}
						}
					}
					break;
				default:
					printf("Invalid board configuration");
					return true;
			}
			//printf("%c%c %c %d %s\n", head[0], head[1], direction, length, name);
			//printf("\n");
			freeSlot++;

		} else {
			eof = true;
		}
	}


	fclose(file);
	numShips = freeSlot;
	return false;
}


/* Initializes the server with everything from the missiles file. Returns true if errors */
bool server_initializeMissiles(char* fileName) {
//	FILE* file = fopen(fileName, "r");

//	char line[7];
//	while (true) {
//		fgets(line, 7, file);
//		for(int i = 0; i < 6; i++)
//			line[i] = tolower(line[i]);
//		if (strcmp(line, "single\n") == 0) missiles.add(SINGLE); /* TODO */
//		else if (strcmp(line, "splash\n") == 0) missiles.add(SPLASH);
//		else if (strcmp(line, "v-line\n") == 0) missiles.add(VLINE);
//		else if (strcmp(line, "h-line\n") == 0) missiles.add(HLINE);
//		else if (strcmp(line, "") == 0) return false;
//		else {
//			printf("Invalid missile \"%s\"!", line);
//			return true;
//		}
//	}

//	fclose(file);
	return false;
}

char* server_getShipName(int shipId) {
	return shipNames[shipId];
}

int server_getWidth() {
	return width;
}

int server_getHeight() {
	return height;
}
