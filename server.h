void server_initialize(void* _missiles, int*** _ships, int _numOfShips, int _width, int _height);

// -2 = miss, -1 = hit, 0 and onwards are ship numbers for if it sunk
int server_isHit(int attackColumn, int attackRow);
