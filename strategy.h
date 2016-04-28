#ifndef STRATEGY_H
#define STRATEGY_H
int getBestValue(game_t * game, int move, int depth, int alpha, int beta);
int getBestMove(game_t * game, int maxDepth); 
int evalBoard(game_t * game);
#endif
