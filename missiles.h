enum missileType { SINGLE, VLINE, HLINE, SPLASH, MISSILE_EOF };

// didnt have time, sorry

struct missile {
    char* name;
    struct missile* next;
};
