// Knight.h
#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Board;
class Move;


// Offsets for knight movement. Long coorinate is first.
constexpr int UP_LEFT_OFFSET    = -17;
constexpr int UP_RIGHT_OFFSET   = -15;
constexpr int RIGHT_UP_OFFSET   = - 6;
constexpr int RIGHT_DOWN_OFFSET =  10;
constexpr int DOWN_RIGHT_OFFSET =  17;
constexpr int DOWN_LEFT_OFFSET  =  15;
constexpr int LEFT_DOWN_OFFSET  =   6;
constexpr int LEFT_UP_OFFSET    = -10;

class Knight : public Piece {
public:
    Knight(Colour);
    Knight(Colour, bool);
    ~Knight() = default;
    MoveSet* getLegalMoves(Board*, int) const override;
    Piece* clone() const override;
    PieceType getPieceType() const override;
};

#endif