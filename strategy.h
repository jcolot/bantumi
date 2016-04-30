#ifndef STRATEGY_H
#define STRATEGY_H
int getBestValue(game_t * gameStack, int move, int depth, int maxDepth, int alpha, int beta, long long maxTime);
int getBestMove(game_t * game, int maxDepth); 
int evalBoard(game_t * game);
#endif
