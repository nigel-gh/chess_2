// Rook.cpp

#include "Bishop.h"
#include "../Move.h"
#include "../Board.h"
#include "../TypesAndEnums.h"
#include <iostream>

Bishop::Bishop(Colour _colour) {
    setColour(_colour);
    if (_colour == WHITE) {
        setDisplayChar('b');
    } else {
        setDisplayChar('B');
    }
    setHasMoved(false);
}

Bishop::Bishop(Colour _colour, bool _hasMoved) {
    setColour(_colour);
    if (_colour == WHITE) {
        setDisplayChar('b');
    } else {
        setDisplayChar('B');
    }
    setHasMoved(_hasMoved);
}

Piece* Bishop::clone() const {
    return new Bishop(getColour(), getHasMoved());
}

PieceType Bishop::getPieceType() const {
    return BISHOP;
}

MoveSet* Bishop::getLegalMoves(Board* board, int currentPos) const {
    MoveSet*  moves = new MoveSet();
    Colour   startSquareColour = this->getColour();
    bool hasMoved = getHasMoved();

    // Northwestern moves
    int i = currentPos - 9;
    while ((i % 8 != 7) && i >= 0 && addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE,hasMoved, false, startSquareColour, board, moves)) {
        i -= 9;
    }
    // Northeastern moves
    i = currentPos - 7;
    while ((i % 8) && i >= 0 && addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE,hasMoved, false, startSquareColour, board, moves)) {
        i -= 7;
    }
    // Southeastern moves
    i = currentPos + 9;
    while ((i % 8) && i < 64 && addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE,hasMoved, false, startSquareColour, board, moves)) {
        i += 9;
    }
    // Southwestern moves
    i = currentPos + 7;
    while ((i % 8 != 7) && i < 64 && addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE,hasMoved, false, startSquareColour, board, moves)) {
        i += 7;
    }
    return moves;
}