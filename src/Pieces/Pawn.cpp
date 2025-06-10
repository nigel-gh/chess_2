// Pawn.cpp

#include "Pawn.h"
#include "../Move.h"
#include "../Board.h"
#include "../TypesAndEnums.h"
#include <iostream>

Pawn::Pawn(Colour _colour) {
    setColour(_colour);
    if (_colour == WHITE) {
        setDisplayChar('p');
    } else {
        setDisplayChar('P');
    }
    setHasMoved(false);
}

Pawn::Pawn(Colour _colour, bool _hasMoved) {
    setColour(_colour);
    if (_colour == WHITE) {
        setDisplayChar('p');
    } else {
        setDisplayChar('P');
    }
    setHasMoved(_hasMoved);
}

Piece* Pawn::clone() const {
    return new Pawn(getColour(), getHasMoved());
}

PieceType Pawn::getPieceType() const {
    return PAWN;
}

const bool Pawn::squareOnBoard(const int square) const {
    Colour colour = this->getColour();
    if (colour == WHITE) {
        return square >= 0;
    } else {
        return square < 64;
    }
}

MoveSet* Pawn::getLegalMoves(Board* board, int currentPos) const {
    MoveSet* moves = new MoveSet();
    Move* proposedMove;
    Colour startSquareColour = getColour();
    Colour endSquareColour;
    // This stuff can be put in a constexpr and then moved to the pawn's attributes at creation.
    // Leaving it for now, thought. I want to get the game working first.
    int oneSquareOffset = startSquareColour == WHITE ? -8 : 8;
    int twoSquareOffset = 2 * oneSquareOffset;
    int captureLeftOffset = oneSquareOffset - 1;
    int captureRightOffset = oneSquareOffset + 1;
    bool hasMoved = getHasMoved();
    const int enpassantSqr = board->getEnpassantSquare();
    
    // Set a promotion flag if applicable
    bool promotionFlag;
    if (!squareOnBoard(currentPos + twoSquareOffset)) {  
        promotionFlag = true;
    } else {
        promotionFlag = false;
    }
    
    // Single move
    int i = currentPos + oneSquareOffset;
    if (squareOnBoard(i) && board->getSquareColour(i) == NO_COLOUR) {
        
        // Move forward
        addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE, hasMoved, promotionFlag, startSquareColour, board, moves);
        
    }
    
    // Capture left
    i = currentPos + captureLeftOffset;
    if (squareOnBoard(i)) {
        
        endSquareColour = board->getSquareColour(i);
        if (isOppositeColour(startSquareColour, endSquareColour) && ((i % 8) !=7 ) && squareOnBoard(i)) {
            addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE, hasMoved, promotionFlag, startSquareColour, board, moves);
            
            // Enpassant left
        } else if (endSquareColour == NO_COLOUR) {
            if (i == enpassantSqr) {
                proposedMove = new Move(currentPos, i, currentPos - 1, -1, -1, -1, PAWN, PAWN, !hasMoved);
                board->applyMove(proposedMove, startSquareColour);
                if (!board->kingInCheck(startSquareColour)) {
                    moves->addMove(proposedMove);
                }
                board->undoMove();
            }
        }
    }
    
    // Capture right
    i = currentPos + captureRightOffset;
    if (squareOnBoard(i)) {
        
        endSquareColour = board->getSquareColour(i);
        if (isOppositeColour(startSquareColour, endSquareColour) && (i % 8) && squareOnBoard(i)) {
            addMoveIfNotCheck(currentPos, i, RESET_ENPASSANT_SQUARE, hasMoved, promotionFlag, startSquareColour, board, moves);
            
            // Enpassant right
        } else if (endSquareColour == NO_COLOUR) {
            if (i == enpassantSqr) {
                proposedMove = new Move(currentPos, i, currentPos + 1, -1, -1, -1, PAWN, PAWN, !hasMoved);
                board->applyMove(proposedMove, startSquareColour);
                if (!board->kingInCheck(startSquareColour)) {
                    moves->addMove(proposedMove);
                }
                board->undoMove();
            }
        }
    }
    
    // Double move
    i = currentPos + twoSquareOffset;
    if (squareOnBoard(i)) {
        if (!(this->getHasMoved())) {
            addMoveIfNotCheck(currentPos, i, i - oneSquareOffset, hasMoved, false, startSquareColour, board, moves);
        }
    }
    
    return moves;
}