// Rook.cpp

#include "Rook.h"
#include "../Move.h"
#include "../Board.h"
#include "../TypesAndEnums.h"
#include <iostream>

Rook::Rook(Colour _colour) {
    setColour(_colour);
    if (_colour == WHITE) {
        setDisplayChar('r');
    } else {
        setDisplayChar('R');
    }
    setHasMoved(false);
}

Rook::Rook(Colour _colour, bool _hasMoved) {
    setColour(_colour);
    if (_colour == WHITE) {
        setDisplayChar('r');
    } else {
        setDisplayChar('R');
    }
    setHasMoved(_hasMoved);
}

Piece* Rook::clone() const {
    return new Rook(getColour(), getHasMoved());
}

PieceType Rook::getPieceType() const {
    return ROOK;
}

MoveSet* Rook::getLegalMoves(Board* board, int currentPos) const {
    MoveSet* moves = new MoveSet();
    // Move* proposedMove;
    Colour startSquareColour    = this->getColour();
    bool hasMoved = getHasMoved();

    // Northern moves
    int i = currentPos - 8;
    while (i >= 0 && addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE, hasMoved, false, startSquareColour, board, moves)) {
        i -= 8;
    }
    // Southern moves
    i = currentPos + 8;
    while (i < 64 && addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE, hasMoved, false, startSquareColour, board, moves)) {
        i += 8;
    }
    // Eastern moves
    i = currentPos + 1;
    while ((i % 8) && i < 64 && addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE, hasMoved, false, startSquareColour, board, moves)) {
        i += 1;
    }
    // Western moves
    i = currentPos - 1;
    while ((i % 8 != 7) && i >= 0 && addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE, hasMoved, false, startSquareColour, board, moves)) {
        i -= 1;
    }

    return moves;
}