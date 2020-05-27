#ifndef MISSILES_H
#define MISSILES_H
enum missileType { SINGLE, VLINE, HLINE, SPLASH, MISSILE_EOF };
struct missile {
    enum missileType type;
    struct missile* next;
};
#endif
