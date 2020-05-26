int*** ships;
int numOfShips;
int width, height;

void server_initialize(void* _missiles, int*** _ships, int _numOfShips, int _width, int _height) { // shoot, im trying to do OOP again
	ships = _ships;
	numOfShips = _numOfShips;
	width = _width;
	height = _height;
}


// -2 = miss, -1 = hit, 0 and onwards are ship numbers for if it sunk
int server_isHit(int attackColumn, int attackRow) { // if this is gonna be multiplayer, I gotta do some server side input validation too
	for (int ship = 0; ship < numOfShips; ship++) {
		for (int block = 0; block < 12; block++) {
			if ((ships[ship][block][0] == attackColumn) && (ships[ship][block][1] == attackRow)) {
				// check for sink
				ships[ship][block][2] == 1; // its hit
				for (block1/*cant think of a better name*/ = 0; block1 < 12; block++) {
					if (!(ships[ship][block1][2] == 1)) return -1; // this is a hit if all blocks havent been hit
				}
				return ship;
			}
		}
	}
	return -2;
}
