// Pawn.h

#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Board;
class Move;

class Pawn : public Piece {
public:
    Pawn(Colour);
    Pawn(Colour, bool);
    ~Pawn() = default;
    MoveSet* getLegalMoves(Board*, int) const override;
    Piece* clone() const override;
    PieceType getPieceType() const override;
private:
    const bool squareOnBoard(const int) const;
};

#endif