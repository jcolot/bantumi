#ifndef STRATEGY_H
#define STRATEGY_H
int getBestScore(game_t * game, int move, int maxDepth);
int getBestMove(game_t * game, int maxDepth); 
int evalBoard(game_t * game);
#endif
