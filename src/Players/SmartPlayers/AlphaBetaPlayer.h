// ABPlayer.h

// dumb, but less dumb, smart player

#ifndef ALPHA_BETA_PLAYER_H
#define ALPHA_BETA_PLAYER_H

#include "../Player.h"
#include "../../Board.h"

class Player;
class Move;

constexpr int DEFAULT_DEPTH_OF_ALPHA_BETA_PLAYER = 7;
constexpr int EVAL_MAX_SCORE =  999999;
constexpr int EVAL_MIN_SCORE = -999999;

class ABPlayer : public Player {
private:
    int maxSearchDepth;
    int alphaBetaMin(int, int, int, Board*, Colour, Move*) const;
    int alphaBetaMax(int, int, int, Board*, Colour, Move*) const;
    int alphaBeta(int, int, int, Board*, Colour, int*) const;
    int evaluateBoardScore(Board*) const;
public:
    ABPlayer();
    ABPlayer(Colour);
    ABPlayer(Colour, int);
    ~ABPlayer();
    const Action chooseAction(const MoveSet*, const Board*) const override;
    Player* clone() const override;
};

#endif