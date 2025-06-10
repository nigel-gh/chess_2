// Queen.h

#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Board;
class Move;

class Queen : public Piece {
public:
    Queen(Colour);
    Queen(Colour, bool);
    ~Queen() = default;
    MoveSet* getLegalMoves(Board*, int) const override;
    Piece* clone() const override;
    PieceType getPieceType() const override;
};

#endif