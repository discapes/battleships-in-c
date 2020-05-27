#ifndef SERVER_H
#define SERVER_H
#include "missile.h"
#include <stdbool.h>
bool server_initializeBoard(char* fileName);
bool server_initializeMissiles(char* fileName);
int server_hit(int attackColumn, int attackRow);
bool server_isShip(int x, int y);
int server_getHeight();
int server_getWidth();
char* server_getShipName();
//missileType getNextMissileType();
//int getNumMissilesLeft();
#endif
