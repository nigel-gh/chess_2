// Rook.h

#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Board;
class Move;

class Rook : public Piece {
public:
    Rook(Colour);
    Rook(Colour, bool);
    ~Rook() = default;
    MoveSet* getLegalMoves(Board*, int) const override;
    Piece* clone() const override;
    PieceType getPieceType() const override;
};

#endif