#ifndef TYPEDEFS_H
#define TYPEDEFS_H
typedef enum {human = 0, computer} player_t;
typedef enum {false = 0, true} bool;
typedef struct {
    int board[2][7];
    player_t player;
} game_t;

typedef enum {
        INIT_STATE,
        PLAY_STATE,
        ENDGAME_STATE,
        /* CONFIG_STATE, pour plus tard */
        /* ERROR_STATE,  */
        EXIT_STATE
} state_t;
#endif
