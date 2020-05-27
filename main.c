#include "server.h"
#include "client.h"
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char ** argv) {
	bool shouldQuit = false; /* so we don't have multiple exits, even though this seems kind of stupid */
	if (argc < 2) {
		printf("Not enough arguments!\n");
		shouldQuit = true;
	}

	{ /* INITIALIZE SERVER */
		/* INITIALIZE BOARD */
		if (!shouldQuit) shouldQuit |= server_initializeBoard(argv[1]);
		/* INITIALIZE MISSILES */
		if (!shouldQuit) shouldQuit |= server_initializeMissiles(argv[2]);
	}
	{ /* INITIALIZE CLIENT */
		if (!shouldQuit) shouldQuit |= client_initialize();
	}

	while(!shouldQuit) {
		switch(client_mainMenu()) {
			case EXIT:
				shouldQuit = false;
				break;
			case PLAY:
				shouldQuit |= client_playGame();
				break;
			case MISSILES:
				shouldQuit |= client_listMissiles();
				break;
			default:
				break;
		}
	}

	client_quit();
	return !shouldQuit;
}
