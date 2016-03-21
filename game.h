#ifndef GAME_H
#define GAME_H

typedef enum {human = 0, computer} player_t;
typedef enum {false = 0, true} bool;
typedef enum {
        INIT_STATE,
        PLAY_STATE,
        ENDGAME_STATE,
        CONFIG_STATE,
        ERROR_STATE,
        EXIT_STATE
} state_t;

typedef struct {
    int board[2][7];
    player_t player;
} game_t;


game_t * initGame(); 
state_t playGame(game_t * game);
bool isEndGame(game_t * game);
void doMove(game_t * game, int bowlNum);
void displayBoard(game_t * game);
void stateMachine(); 
void printScore(game_t * game);
void displayError();
#endif

