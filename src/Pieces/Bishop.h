// Bishop.h
#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

class Board;
class Move;

class Bishop : public Piece {
public:
    Bishop();
    Bishop(Colour);
    Bishop(Colour, bool);
    ~Bishop() = default;
    MoveSet* getLegalMoves(Board*, int) const override;
    Piece* clone() const override;
    PieceType getPieceType() const override;
};

#endif