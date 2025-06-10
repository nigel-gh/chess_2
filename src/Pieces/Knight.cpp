// Knight.cpp

#include "Knight.h"
#include "../Move.h"
#include "../Board.h"
#include "../TypesAndEnums.h"
#include <iostream>

Knight::Knight(Colour _colour) {
    setColour(_colour);
    if (_colour == WHITE) {
        setDisplayChar('n');
    } else {
        setDisplayChar('N');
    }
    setHasMoved(false);
}

Knight::Knight(Colour _colour, bool _hasMoved) {
    setColour(_colour);
    if (_colour == WHITE) {
        setDisplayChar('n');
    } else {
        setDisplayChar('N');
    }
    setHasMoved(_hasMoved);
}

Piece* Knight::clone() const {
    return new Knight(getColour(), getHasMoved());
}

PieceType Knight::getPieceType() const {
    return KNIGHT;
}

MoveSet* Knight::getLegalMoves(Board* board, int currentPos) const {
    MoveSet* moves = new MoveSet();
    Colour startSquareColour    = this->getColour();
    int startCol = currentPos % 8;
    bool hasMoved = getHasMoved();

    // Up 2, left 1
    int i = currentPos + UP_LEFT_OFFSET;
    if (((i % 8) < startCol) && (i >=0)) {addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE,hasMoved, false, startSquareColour, board, moves);}
    
    // Up 2, right 1
    i = currentPos + UP_RIGHT_OFFSET;
    if (((i % 8) > startCol) && (i >=0)) {addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE,hasMoved, false, startSquareColour, board, moves);}
    
    // Right 2, up 1
    i = currentPos + RIGHT_UP_OFFSET;
    if (((i % 8) > startCol) && (i >=0)) {addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE,hasMoved, false, startSquareColour, board, moves);}
    
    // Right 2, down 1
    i = currentPos + RIGHT_DOWN_OFFSET;
    if (((i % 8) > startCol) && (i < 64)) {addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE,hasMoved, false, startSquareColour, board, moves);}

    // Down 2, right 1
    i = currentPos + DOWN_RIGHT_OFFSET;
    if (((i % 8) > startCol) && (i < 64)) {addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE,hasMoved, false, startSquareColour, board, moves);}

    // Down 2, left 1
    i = currentPos + DOWN_LEFT_OFFSET;
    if (((i % 8) < startCol) && (i < 64)) {addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE,hasMoved, false, startSquareColour, board, moves);}

    // Left 2, down 1
    i = currentPos + LEFT_DOWN_OFFSET;
    if (((i % 8) < startCol) && (i < 64)) {addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE,hasMoved, false, startSquareColour, board, moves);}

    // Left 2, up 1
    i = currentPos + LEFT_UP_OFFSET;
    if (((i % 8) < startCol) && (i >=0)) {addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE,hasMoved, false, startSquareColour, board, moves);}
    
    return moves;
}