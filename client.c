#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define MAX_SHIPS 10

#include "server.h"
#include "missiles.h"

FILE* boardFile;
FILE* missileFile;
int width, height;
char** board;

// ran out of time so I couldn't do the missiles or colors

enum menuOption { EXIT, PLAY, MISSILES, INVALID};

// forward declarations (I like to use header files only for the parts that are actually used by otherfiles,
// as to not expose the private implementation (like Java I guess). I don't really need a header file for the client...
void client_playGame();
enum menuOption mainMenu();
int colToInt(char letter);
int rowToInt(char digit);

/* Closes everything up */
void client_quit() {
	for (int col = 0; col < width; col++)
		free(board[col]);
	free(board);
	exit(0);
}

/* does what it says on the tin */
enum missileType getNextMissileFromFile() {
	static char line[7];
	fgets(line, 7, missileFile);
	for(int i = 0; i < 6; i++)
		line[i] = tolower(line[i]);
	if (strcmp(line, "single\n") == 0) return SINGLE;
	else if (strcmp(line, "splash\n") == 0) return SPLASH;
	else if (strcmp(line, "v-line\n") == 0) return VLINE;
	else if (strcmp(line, "h-line\n") == 0) return HLINE;
	else if (strcmp(line, "") == 0) return MISSILE_EOF;
	else {
		printf("Invalid missile configuration file!");
		client_quit();
	}
}

// gets the ships from the boardFile and fills the ships array. What ships array you say? Well, TODO organize the functions so that they go in a clean order, as shown in Robert C. Martins book Clean Code

int fillShips(int* ships[MAX_SHIPS]) { // I can't remember if output parameters are bad...
	//TODO

}
// getNextMissileFromFile() and fillShips() should probably be in a different file called configreader.c or something. If this was C++ everything would also be an object...


/* I have chosen to use a "client-server" approach to this. It would be easy to expand to multiplayer, and it also keeps
   the actual logic of hit-checking and other stuff separate from the terminal user interface. I am more used to programming
   in OO languages, but I think this is still a good way to go about the game */
int main(/*TODO*/) {
	//{ // initializes the server from the configuration files
	//	{ // read board config
	//		boardFile = fopen(arg0, 'r');
	//		width = getBoardWidth();
	//		height = getBoardHeight();
			width = 10;
			height = 10;
	//		if (!(width >= 1 && width <= 12)) { printf("Invalid board configuration file!"; client_quit(); }
	//		if (!(height >= 1 && height <= 12)) { printf("Invalid board configuration file!"; client_quit(); }


			int ships[MAX_SHIPS][12][3];
			/*array of ships with the x and y of each block of the ship also if its hit (TODO use a byte long int).
			  I though of using max(width, height) instead of 12, but I would have to malloc() it, and that would be way
			  too much trouble.*/
			int shipNames[MAX_SHIPS]; //TODO these two should be just pointers, and fillShips() would determine their length and return it.

			int numOfShips = fillShips(ships, shipNames);
	//		fclose(boardFile);
	//	}
	//	{ // read missile file
	//		missileFile = fopen(arg1, 'r');
	//		missileType currentMissile;
	//		// linked list missileType missiles;
	//		while((missile = getNextMissileFromFile()) != MISSILE_EOF) {
	//			missiles.push();
	//		}
	//		fclose(missileFile);
	//	}

		server_initialize(nullptr, ships, numOfShips, width, height);
	//}


	// I guess we have to set up some parts of the client, too
	board = (char **)malloc(width * sizeof(char *));
	for (int col = 0; col < width; col++)
		board[col] = (char *)malloc(height * sizeof(char));
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			board[i][j] = '#';
		}
	}

	while(true) { /* I know some people consider while(true) loops bad, because while(true) means that the loop shouldn't
			 break. However, this loop doesn't really ever break, the program just exit():s when I call quit from
			 here. quit() is also called from other places, so the while(true) loop seems appropriate here.
			 Also I think having only the server initialization block, and then a really short and simple main menu
			 block should be considered clean code */
		switch(mainMenu()) {
			case EXIT:
				client_quit();
				break;
			case PLAY:
				client_playGame(); //TODO separate main and client
				break;
			case MISSILES:
				//printMissiles(missiles);
				break;
			case INVALID:
				break;
		}
	}
}

char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                 'Y', 'Z'};

void client_playGame() {
	static int attackColumn = -1;
	static int attackRow = -1;

	while(true) {

		printf("\033[2J\n");
		if ((attackRow > -1) && (attackRow < height) && (attackColumn > -1) && (attackColumn < width)) {
			static int result;
			result = server_isHit(attackColumn, attackRow);
			if (result == -2) {
				board[attackColumn][attackRow] = '0';
			} if (result >= 0) {
				board[attackColumn][attackRow] = 'S'; // S for sunk
				printf(" SHIP %s HAS BEEN SUNK", shipNames[result];
			} else {
				board[attackColumn][attackRow] = 'X';
			}
		}

		{ // print the board
			printf(":) ||");
			for (int i = 0; i < width; i++) {
				printf(" %c |", alphabet[i]);
			} printf("\n");
			printf("---++");
			for (int i = 0; i < width; i++) {
				printf("===+");
			} printf("\n");
			for (int row = 0; row < height; row++) {
				printf(" %d ||", row); // I want the first row to be zero, like its supposed to be
				for (int col = 0; col < width; col++) {
					printf(" %c |", board[col][row]);
				} printf("\n");
				printf("---++");
				for (int i = 0; i < width; i++) {
					printf("---+");
				} printf("\n");
			}
			printf("\n");
		}

		static char response[2];
		response[0] = ' ';
		response[1] = ' ';
		printf("Enter next target :");
		scanf("%s", response);
		attackColumn = colToInt(response[0]);
		attackRow = rowToInt(response[1]);

		usleep(10 * 1000);
	}
}

enum menuOption mainMenu() {
	printf("\033[2J");
	printf("         ________________________________\n");
	printf("         | B  A  T  T  L  E  S  H  I  P |\n");
	printf("         ********************************\n");
	printf("         1. Play the game\n");
	printf("         2. List all missiles\n");
	printf("         3. Exit\n");
	char option = ' ';
	scanf("%c", &option);
	switch(option) {
		case '1':
			return PLAY;
			break;
		case '2':
			return MISSILES;
			break;
		case '3':
			return EXIT;
			break;
		default:
			return INVALID;
			break;
	}
}

int colToInt(char letter) {
	letter = toupper(letter);
	for (int i = 0; i < 26; i++) {
		if(alphabet[i] == letter) return i;
	}
	return -1;
}


int rowToInt(char digit) {
	switch(digit) {
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		default:
			return -1;
	}
}
