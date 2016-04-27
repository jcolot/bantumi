#ifndef STRATEGY_H
#define STRATEGY_H
int getBestScore(game_t * game, int move, int depth);
int getBestMove(game_t * game); 
int evalBoard(game_t * game);
#endif
