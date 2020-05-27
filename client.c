#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "server.h"
#include "client.h"
#include "configuration.h"

char** board; /* board that will be printed */

/* Forward declarations */
int client_width, client_height;
char alphabet[26];
void printColorCode(char mark);
int colToInt(char letter);
int rowToInt(char digit);

bool client_playGame() {
	static int attackColumn = -1;
	static int attackRow = -1;
	static int shotsDone = 0;
	while(true) {
		static int sunkShip = -1;
		static int itersSinceSunk = 999;
		printf("\033[2J\n");
		if ((attackRow > -1) && (attackRow < client_height) && (attackColumn > -1) && (attackColumn < client_width)) {
			static int result;
			result = server_hit(attackColumn, attackRow);
			if (result == -1) {
				board[attackColumn][attackRow] = '0';
			} else if (result >= 0) {
				board[attackColumn][attackRow] = 'S'; // S for sunk
				sunkShip = result;
				itersSinceSunk = 0;
			} else {
				board[attackColumn][attackRow] = 'X';
			}
		}

		{ // print the board
			printf(":) ||");
			for (int i = 0; i < client_width; i++) {
				printf(" %c |", alphabet[i]);
			} printf("\n");
			printf("---++");
			for (int i = 0; i < client_width; i++) {
				printf("===+");
			} printf("\n");
			for (int row = 0; row < client_height; row++) {
				printf(" %d ||", row); // I want the first row to be zero, like its supposed to be
				for (int col = 0; col < client_width; col++) {
					#ifndef MONO
					printColorCode(board[col][row]);
					#endif
					#ifdef DEBUG
					if (server_isShip(col, row)) {
						printf("\033[1;35m");
					}
					#endif
					printf(" %c ", board[col][row]);
					printf("\033[0m");
					printf("|");
				} printf("\n");
				printf("---++");
				for (int i = 0; i < client_width; i++) {
					printf("---+");
				} printf("\n");
			}
			printf("\n");
		}
		if (itersSinceSunk < 1) {
			printf(" SHIP %s HAS BEEN SUNK\n\n", server_getShipName(sunkShip));
		}
		static char response[2];
		response[0] = ' ';
		response[1] = ' ';
		printf(" Shots : %d\n\n", shotsDone);
		printf("Enter next target : ");
		scanf("%s", response);
		attackColumn = colToInt(response[0]);
		attackRow = rowToInt(response[1]);
		itersSinceSunk++;
		shotsDone++;
		usleep(10 * 1000);
	}
	return false;
}

enum menuOption client_mainMenu() {
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

void printColorCode(char mark) {
	switch(mark) {
		case '#':
			printf("\033[1;34m");
			break;
		case 'X':
			printf("\033[0;31m");
			break;
		default:
			printf("\033[0;32m");
			break;
	}
}

bool client_listMissiles() {
	//TODO
	return false;
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


/* Deallocate the board */
void client_quit() {
	for (int col = 0; col < client_width; col++)
		free(board[col]);
	free(board);
}

/* Allocate the board */
bool client_initialize() {
	client_width = server_getWidth();
	client_height = server_getHeight();

	board = (char **)malloc(client_width * sizeof(char *));
	for (int col = 0; col < client_width; col++)
		board[col] = (char *)malloc(client_height * sizeof(char));
	for (int i = 0; i < client_width; i++) {
		for (int j = 0; j < client_height; j++) {
			board[i][j] = '#';
		}
	}
	return false;
}

char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                 'Y', 'Z'};
