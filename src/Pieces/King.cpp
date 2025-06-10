// King.cpp

#include "King.h"
#include "../Move.h"
#include "../Board.h"
#include "../TypesAndEnums.h"
#include "PieceAccessibleSquares.h"
#include <iostream>

King::King(Colour _colour) {
    setColour(_colour);
    if (_colour == WHITE) {
        setDisplayChar('k');
    } else {
        setDisplayChar('K');
    }
    setHasMoved(false);
}

King::King(Colour _colour, bool _hasMoved) {
    setColour(_colour);
    if (_colour == WHITE) {
        setDisplayChar('k');
    } else {
        setDisplayChar('K');
    }
    setHasMoved(_hasMoved);
}

Piece* King::clone() const {
    return new King(getColour(), getHasMoved());
}

PieceType King::getPieceType() const {
    return KING;
}

// castleAvailable = rook not moved AND king not moved AND no Pieces in way AND not passing through check

MoveSet* King::getLegalMoves(Board* board, int currentPos) const {
    
    MoveSet* moves = new MoveSet();
    Colour startSquareColour    = this->getColour();
    bool leftIsOnBoard          = (currentPos % 8) != 0;
    bool rightIsOnBoard         = (currentPos % 8) != 7;
    bool hasMoved               = getHasMoved();
    
    // Generally, kings stay on the top and bottom of the board. Should check if up and down are allowed
    // before up left and up right as opposed to left allowed then left up
    
    OnBoardMoves possibleMoveSquares = getKingAccessibleSquaresFrom(currentPos);
    int canidateSquare;
    int enemyKingPos = board->getKingPos(getOppositeColour(startSquareColour));
    
    for (size_t i = 0; i < possibleMoveSquares.count; i++) {
        
        canidateSquare = possibleMoveSquares.moves[i];
        
        if (canidateSquare != enemyKingPos) {
            addMoveIfNotCheck(currentPos, canidateSquare, RESET_ENPASSANT_SQUARE, hasMoved, false, startSquareColour, board, moves);
        }
        
    }
    // Left
    if (leftIsOnBoard) {        
        // Left castle
        if (castleIsAllowed(board, currentPos, false)) {
            moves->addMove(new Move(currentPos, currentPos - 2, -1, currentPos - 4, currentPos - 1, RESET_ENPASSANT_SQUARE, KING, KING, !hasMoved));
        }
    }
    // Right
    if (rightIsOnBoard) {        
        // Right castle
        if (castleIsAllowed(board, currentPos, true)) {
            moves->addMove(new Move(currentPos, currentPos + 2, -1, currentPos + 3, currentPos + 1, RESET_ENPASSANT_SQUARE, KING, KING, !hasMoved));
        }
    }
    return moves;
}

const bool King::castleIsAllowed(Board* board, int startKingSquare, bool isKingSideCastle) const {

    Colour kingColour = this->getColour();
    // False if king has already moved.
    if (this->getHasMoved()) {
        return false;
    }
    int rookOffset = isKingSideCastle ? 3 : -4;
    const Piece* pieceAtRookSquare = board->getPieceAtPos(startKingSquare + rookOffset);

    // False if there is no piece at the rook square.
    if (pieceAtRookSquare == nullptr) {
        return false;
    }

    // False if there is a piece at the rook square, but it has moved.
    if (pieceAtRookSquare->getHasMoved()) {
        return false;
    }
    
    // Try moving the king to each square and see if it is in check.
    int const * squaresKingPassesThrough;
    if (kingColour == WHITE) {
        squaresKingPassesThrough = isKingSideCastle ? WHITE_KING_SIDE_CASTLE_SQUARES_TO_CHECK : WHITE_QUEEN_SIDE_CASTLE_SQUARES_TO_CHECK;
    } else {
        squaresKingPassesThrough = isKingSideCastle ? BLACK_KING_SIDE_CASTLE_SQUARES_TO_CHECK : BLACK_QUEEN_SIDE_CASTLE_SQUARES_TO_CHECK;
    }
    
    Move* testMove;
    int numSquares = isKingSideCastle ? 2 : 3;
    for (int i = 0; i < numSquares; i++) {
        if (board->getSquareColour(squaresKingPassesThrough[i]) == NO_COLOUR) {
            testMove = new Move(startKingSquare, squaresKingPassesThrough[i], -1, -1, -1, RESET_ENPASSANT_SQUARE, KING, KING, false);
            board->applyMove(testMove, kingColour);
            if (board->kingInCheck(kingColour)) {
                board->undoMove();
                // False if the move would put the king in check.
                return false;
            }
            board->undoMove();
        } else {
            // False if there is a piece obstructing the king.
            return false;
        }
    }
    return true;
}