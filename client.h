#ifndef CLIENT_H
#define CLIENT_H

bool client_playGame();
bool client_initialize();
bool client_listMissiles();
void client_quit();
enum menuOption { EXIT, PLAY, MISSILES, INVALID};
enum menuOption client_mainMenu();

#endif
