// Queen.cpp

#include "Queen.h"
#include "../Move.h"
#include "../Board.h"
#include "../TypesAndEnums.h"
#include <iostream>

Queen::Queen(Colour _colour) {
    setColour(_colour);
    if (_colour == WHITE) {
        setDisplayChar('q');
    } else {
        setDisplayChar('Q');
    }
    setHasMoved(false);
}

Queen::Queen(Colour _colour, bool _hasMoved) {
    setColour(_colour);
    if (_colour == WHITE) {
        setDisplayChar('q');
    } else {
        setDisplayChar('Q');
    }
    setHasMoved(_hasMoved);
}

Piece* Queen::clone() const {
    return new Queen(getColour(), getHasMoved());
}

PieceType Queen::getPieceType() const {
    return QUEEN;
}

MoveSet* Queen::getLegalMoves(Board* board, int currentPos) const {
    MoveSet* moves = new MoveSet();
    Colour startSquareColour    = this->getColour();
    bool hasMoved = getHasMoved();

    // BISHOP-LIKE MOVES
    // Northwestern moves
    int i = currentPos - 9;
    while ((i % 8 != 7) && i >= 0 && addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE, hasMoved, false, startSquareColour, board, moves)) {
        i -= 9;
    }
    // Northeastern moves
    i = currentPos - 7;
    while ((i % 8) && i >= 0 && addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE, hasMoved, false, startSquareColour, board, moves)) {
        i -= 7;
    }
    // Southeastern moves
    i = currentPos + 9;
    while ((i % 8) && i < 64 && addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE, hasMoved, false, startSquareColour, board, moves)) {
        i += 9;
    }
    // Southwestern moves
    i = currentPos + 7;
    while ((i % 8 != 7) && i < 64 && addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE, hasMoved, false, startSquareColour, board, moves)) {
        i += 7;
    }

    // ROOK-LIKE MOVES
    // Northern moves
    i = currentPos - 8;
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