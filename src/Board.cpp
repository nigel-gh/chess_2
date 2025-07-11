// Board.cpp

#include <string>
#include <random>
#include <iostream>
#include <bitset>
#include <algorithm>

#include "Board.h"
#include "Players/Player.h"
#include "ZobristHash.h"
#include "Move.h"
#include "Pieces/Piece.h"
#include "Pieces/Rook.h"
#include "Pieces/PieceAccessibleSquares.h"
#include "TypesAndEnums.h"

Board::Board() {
    enpassantSquare = RESET_ENPASSANT_SQUARE;
    numMovesPlayed  = 0;
    fiftyMoveRuleStack[numMovesPlayed] = 0;
    whiteKingPos    = WHITE_KING_START_SQUARE;
    blackKingPos    = BLACK_KING_START_SQUARE;
    pieceAtSquareMap = new BoardPieceMap();
}

void Board::display(const MoveSet* legalMoves, Colour clrToMove) const {

    std::string kingInCheckStr = kingInCheck(clrToMove) ? "true" : "false";
    std::string leftPadding = "   ";
    std::string rowSeparator = "  --- --- --- --- --- --- --- ---\n";
    std::string namePadding = "                  ";
    std::string guiString    = "\n ___________________________________________\n";
    guiString               += "/                                           \\\n";
    guiString += namePadding + "black\n";
    guiString += "      a   b   c   d   e   f   g   h\n";
    guiString += leftPadding + rowSeparator + leftPadding + "8";
    int pieceChar;
    for (int i = 0; i < 64; i++) {
        if (pieceAtSquareMap->getPieceAtPos(i) == nullptr) {
            pieceChar = ' ';
        }
        else {
            pieceChar = pieceAtSquareMap->getPieceAtPos(i)->getDisplayChar();
        }
        if (i % 8 == 0 && i != 0) {
            guiString += "|\n";
            guiString += leftPadding + rowSeparator + leftPadding + std::to_string(8 - i / 8);
        }
        guiString += "| " + std::string(1, pieceChar) + " ";
        
    }
    guiString += "|\n";
    guiString += leftPadding + rowSeparator + namePadding + "white\n\n";
    unsigned long long zobPos = pieceAtSquareMap->getZobHashCurrentKey();
    int numTimesPosOccured = pieceAtSquareMap->getNumTimesPosOccurred(zobPos);
    guiString += "  zobKey            : " + std::to_string(zobPos) + "\n";
    guiString += "  num ply played    : " + std::to_string(numMovesPlayed) + "\n";
    guiString += "  Times pos occured : " + std::to_string(numTimesPosOccured) + "\n";
    guiString += "  Moves since last capture or pawn move: " + std::to_string(fiftyMoveRuleStack[numMovesPlayed]) + "\n";
    guiString += "  En passent sqaure : " + squareToString(enpassantSquare) + " (" + std::to_string(enpassantSquare) + ")\n";
    guiString += "  king in check     : " + kingInCheckStr + "\n";
    guiString += "  \n  " + colourToString(clrToMove) + " to move\n\n";
    guiString += legalMoves->toDisplayString();
    guiString += "\\___________________________________________/\n";
    std::cout << guiString;
}

MoveSet* Board::calcLegalMoves(Colour colour) {
    
    MoveSet* legalMoves = new MoveSet();
    MoveSet* pieceLegalMoves;
    SquarePiecePairSet squarePiecePairs = getPlayerPieces(colour);
    
    for (const SquarePiecePair& pair : squarePiecePairs) {
        
        int square      = pair.first;
        Piece* piece    = pair.second;
        pieceLegalMoves = piece->getLegalMoves(this, square);
        legalMoves->addMoveSet(pieceLegalMoves);
        
        delete pieceLegalMoves;
    }
        
    return legalMoves;
}

const int Board::getEnpassantSquare()           const   { return enpassantSquare; }

void Board::setEnpassantSquare(int sqr)                 { enpassantSquare = sqr; }

const Piece* Board::getPieceAtPos(int position) const   { return pieceAtSquareMap->getPieceAtPos(position); }

int Board::getKingPos(Colour _colour)           const   { return _colour == WHITE ? whiteKingPos : blackKingPos; }

void Board::setKingPos(Colour _colour, int pos) {
    if (_colour == WHITE) {
        whiteKingPos = pos;
    } else {
        blackKingPos = pos;
    }
}

GameStatus Board::applyMove(const Move* move, Colour playerColour) {
    
    /*
    1. send the captured piece to the captured pile on that square if applciable
    2. move the primary piece to destination
    3. promote the piece if applicable
    4. change castling rights if applicable
    5. move the king if applicable
    push the move to the move stack

    THERE ARE KNOWN ISSUES WITH APPLYMOVE() AND UNDOMOVE() - a fix is left as an exercise to the reader.
    */

   int captureSquare           = move->getCaptureSquare();
   int piecePosBefore          = move->getPiecePosBefore();
   int piecePosAfter           = move->getPiecePosAfter();  
   int castleRookPosBefore     = move->getCastleSquareStart();
   int castleRookPosAfter      = move->getCastleSquareEnd();
   PieceType pieceTypeBefore   = move->getPieceTypeBefore();
   PieceType pieceTypeAfter    = move->getPieceTypeAfter();
      
   GameStatus status = {true, IN_PROGRESS, NO_COLOUR, false, true};
   
   // If piece is a king, update the board's KingPos field
   if (pieceTypeBefore == KING) {
       setKingPos(playerColour, move->getPiecePosAfter());
    }
    
    // Flip has-moved field
    if (move->getFlipHasMoved()) {
        pieceAtSquareMap->flipPieceHasMoved(piecePosBefore);
    }
    
    // Capture piece if applicable
    if (captureSquare >= 0) {
        pieceAtSquareMap->capturePiece(captureSquare);
    }

    // Move the active piece
    pieceAtSquareMap->moveActivePiece(piecePosBefore, piecePosAfter);
    
    // Handle promotions if applicable
    if (pieceTypeBefore != pieceTypeAfter) {
        pieceAtSquareMap->promotePiece(pieceTypeAfter, playerColour, piecePosAfter);
    }
    
    // Handle Castling if needed
    if (move->moveIsCastle()) {
        pieceAtSquareMap->moveActivePiece(castleRookPosBefore, castleRookPosAfter);
    }

    // Set enpassent square
    enpassantSquare = move->getEnpassentSquareAfter();
    
    // handle 50-move rule
    if (pieceTypeBefore == PAWN || captureSquare >= 0) {        // if there was a pawn move or a capture, set next value in the stack to 0
        fiftyMoveRuleStack[numMovesPlayed + 1] = 0;
    } else {
        fiftyMoveRuleStack[numMovesPlayed + 1] = fiftyMoveRuleStack[numMovesPlayed] + 1;        // otherwise, increment the counter by 1
    }
    // 50 moves = 100 ply, end the game if necessary
    if (fiftyMoveRuleStack[numMovesPlayed + 1] >= NUM_PLY_FOR_50_MOVE_RULE) {     // TODO: is this the right number? should it be 99 or 101?
        status = {false, FIFTY_MOVE_RULE, NO_COLOUR, false, true};
    }
    
    moveStack.push(move);
    pieceAtSquareMap->incrementHashAtCurrentPosition();
    
    // check if 3-fold repetition has been hit
    unsigned long long zobPos = pieceAtSquareMap->getZobHashCurrentKey();
    int numTimesPosOccurred = pieceAtSquareMap->getNumTimesPosOccurred(zobPos);
    if (numTimesPosOccurred == 3) {
        status = {false, THREE_FOLD_REPETITION, NO_COLOUR, false, true};
    }

    numMovesPlayed++;
    
    return status;
}

// just apply but in reverse
void Board::undoMove() {
    
    pieceAtSquareMap->decrementHashAtCurrentPosition();
    const Move* move          = moveStack.pop();
    int captureSquare   = move->getCaptureSquare();
    int piecePosBefore  = move->getPiecePosBefore();
    int piecePosAfter   = move->getPiecePosAfter();
    int castleRookPosBefore     = move->getCastleSquareStart();
    int castleRookPosAfter      = move->getCastleSquareEnd();
    PieceType pieceTypeBefore   = move->getPieceTypeBefore();
    PieceType pieceTypeAfter    = move->getPieceTypeAfter();

    // Flip has-moved field
    if (move->getFlipHasMoved()) {
        pieceAtSquareMap->flipPieceHasMoved(piecePosAfter);
    }
    
    // revert the enpassent square
    if (moveStack.size() < 2) {
        enpassantSquare = -1;
    } else {
        const Move* prevMove = moveStack.peek(); // Needed to undo enpassant
        enpassantSquare = prevMove->getEnpassentSquareAfter();
    }

    // un-castle the rooks
    if (move->moveIsCastle()) {
        pieceAtSquareMap->moveActivePiece(castleRookPosAfter, castleRookPosBefore);
    }
    
    // un-promote piece
    if (pieceTypeBefore != pieceTypeAfter) {
        pieceAtSquareMap->unpromotePiece(piecePosAfter);
    }
    
    // un-move the active piece
    pieceAtSquareMap->moveActivePiece(piecePosAfter, piecePosBefore);
    
    // un-capture piece if applicable
    if (captureSquare >= 0) {
        pieceAtSquareMap->uncapturePiece(captureSquare);
    }

    // If piece is a king, update the board's KingPos field
    if (pieceTypeBefore == KING) {
        Piece*  kingPtr = pieceAtSquareMap->getPieceAtPos(piecePosBefore);
        setKingPos(kingPtr->getColour(), move->getPiecePosBefore());
    }    

    // delete move;

    numMovesPlayed--;
}

// Undo numTimes moves at once.
void Board::undoMove(int numTimes) {
    for (int i = 0; i < numTimes; i++) {
        undoMove();
    }
}

Colour Board::getSquareColour(int square) const {
    Piece* piece = pieceAtSquareMap->getPieceAtPos(square);
    if (piece == nullptr) {
        return NO_COLOUR;
    } else {
        return piece->getColour();
    }
}

PieceType Board::getPieceTypeAtPos(int square) const {
    Piece* piece = pieceAtSquareMap->getPieceAtPos(square);
    if (piece == nullptr) {
        return NULL_PIECE;
    } else {
        return piece->getPieceType();
    }
}

bool Board::squaresAreEnemies(int sq1, int sq2) const {
    return (getSquareColour(sq1) ^ getSquareColour(sq2)) == 1;
}

bool Board::kingInCheck(Colour kingClr) const {
    
    int kingPos     = getKingPos(kingClr);
    bool inCheck;
    OnBoardMoves onBoardMoves;

    // bishop NE
    onBoardMoves    = getBishopAccessibleSquaresNEFrom(kingPos);
    inCheck         = kingInCheckFromDirectMovingPiece(kingClr, onBoardMoves, BISHOP);
    if (inCheck) {return true;}
    
    // bishop SE
    onBoardMoves    = getBishopAccessibleSquaresSEFrom(kingPos);
    inCheck         = kingInCheckFromDirectMovingPiece(kingClr, onBoardMoves, BISHOP);
    if (inCheck) {return true;}
    
    // bishop SW
    onBoardMoves    = getBishopAccessibleSquaresSWFrom(kingPos);
    inCheck         = kingInCheckFromDirectMovingPiece(kingClr, onBoardMoves, BISHOP);
    if (inCheck) {return true;}
    
    // bishop SE
    onBoardMoves    = getBishopAccessibleSquaresSEFrom(kingPos);
    inCheck         = kingInCheckFromDirectMovingPiece(kingClr, onBoardMoves, BISHOP);
    if (inCheck) {return true;}
    
    // rook north
    onBoardMoves    = getRookAccessibleSquaresNorthFrom(kingPos);
    inCheck         = kingInCheckFromDirectMovingPiece(kingClr, onBoardMoves, ROOK);
    if (inCheck) {return true;}
    
    // rook south
    onBoardMoves    = getRookAccessibleSquaresSouthFrom(kingPos);
    inCheck         = kingInCheckFromDirectMovingPiece(kingClr, onBoardMoves, ROOK);
    if (inCheck) {return true;}
    
    // rook east
    onBoardMoves    = getRookAccessibleSquaresEastFrom(kingPos);
    inCheck         = kingInCheckFromDirectMovingPiece(kingClr, onBoardMoves, ROOK);
    if (inCheck) {return true;}
    
    // rook west
    onBoardMoves    = getRookAccessibleSquaresWestFrom(kingPos);
    inCheck         = kingInCheckFromDirectMovingPiece(kingClr, onBoardMoves, ROOK);
    if (inCheck) {return true;}
    
    // NOT: DO NOT NEED TO DIRECTLY CHECK QUEEN, HANDLED IN ROOK AND BISHOP CHECKS.
    
    // knight
    onBoardMoves    = getKnightAccessibleSquaresFrom(kingPos);
    inCheck         = kingInCheckFromKnightOrPawn(kingClr, onBoardMoves);
    if (inCheck) {return true;}

    // pawn
    onBoardMoves    = getPawnAccessibleSquaresFrom(kingPos, kingClr);
    inCheck         = kingInCheckFromKnightOrPawn(kingClr, onBoardMoves);
    if (inCheck) {return true;}

    // king
    onBoardMoves    = getKingAccessibleSquaresFrom(kingPos);
    inCheck         = kingInCheckFromKing(kingClr, onBoardMoves);
    if (inCheck) {return true;}
    
    return false;
}

bool Board::kingInCheckFromDirectMovingPiece(Colour kingClr, OnBoardMoves moveSquares, PieceType pieceTypeToCheckFor) const {
    
    int         square;
    Colour      sqrClr;
    PieceType   pieceType;

    for (size_t i = 0; i < moveSquares.count; i++) {
        square = moveSquares.moves[i];
        sqrClr = getSquareColour(square);
        if (isOppositeColour(sqrClr, kingClr)) {
            pieceType = getPieceTypeAtPos(square);
            return pieceType == pieceTypeToCheckFor || pieceType == QUEEN ? true : false;
        } else if (sqrClr == kingClr) {
            return false;
        }
    }

    return false;

}

bool Board::kingInCheckFromKnightOrPawn(Colour kingClr, OnBoardMoves moveSquares) const {
    
    int         square;
    Colour      sqrClr;
    PieceType   pieceType;

    for (size_t i = 0; i < moveSquares.count; i++) {
        square = moveSquares.moves[i];
        sqrClr = getSquareColour(square);
        if (isOppositeColour(sqrClr, kingClr)) {
            pieceType = getPieceTypeAtPos(square);
            if (pieceType == KNIGHT || PAWN) {
                return true;
            }
        }
    }

    return false;

}

bool Board::kingInCheckFromKing(Colour kingClr, OnBoardMoves moveSquares) const {
    
    int         square;
    Colour      sqrClr;
    PieceType   pieceType;

    for (size_t i = 0; i < moveSquares.count; i++) {
        square = moveSquares.moves[i];
        sqrClr = getSquareColour(square);
        if (isOppositeColour(sqrClr, kingClr)) {
            pieceType = getPieceTypeAtPos(square);
            if (pieceType == KING) {
                return true;
            }
        }
    }

    return false;

}

Board* Board::clone() const {
    
    unsigned int   fiftyMoveRuleClone[MAX_NUM_MOVES_IN_GAME];
    std::copy(std::begin(fiftyMoveRuleStack), std::end(fiftyMoveRuleStack), fiftyMoveRuleClone);
    
    BoardPieceMap*  squareMapClone   = pieceAtSquareMap->clone();
    MoveStack       moveStackClone   = moveStack.clone();
    
    return new Board(
        enpassantSquare, 
        whiteKingPos, 
        blackKingPos, 
        numMovesPlayed, 
        fiftyMoveRuleClone, 
        squareMapClone, 
        moveStackClone
    );
    
}

Board::Board(
   int                  _enpassantSquare,
   int                  _whiteKingPos,
   int                  _blackKingPos,
   unsigned int         _numMovesPlayed,
   unsigned int*        _fiftyMoveRule,
   BoardPieceMap*       _pieceAtSquareMap,
   const MoveStack&     _moveStack
) : 
   enpassantSquare  (_enpassantSquare),
   whiteKingPos     (_whiteKingPos),
   blackKingPos     (_blackKingPos),
   numMovesPlayed   (_numMovesPlayed),
   pieceAtSquareMap (_pieceAtSquareMap),
   moveStack        (_moveStack.clone())
{
    std::copy(
        _fiftyMoveRule,
        _fiftyMoveRule + MAX_NUM_MOVES_IN_GAME,
        fiftyMoveRuleStack
    );
}

SquarePiecePairSet& Board::getPlayerPieces(Colour clr) {
    return pieceAtSquareMap->getPlayerPieces(clr);
}

int Board::evaluateLeafPositionScore(Colour clrToMove) {
    
    int          score = 0;
    const Piece* piece;
    SquarePiecePairSet& whitePieces = getPlayerPieces(WHITE);
    SquarePiecePairSet& blackPieces = getPlayerPieces(BLACK);

    constexpr int CHECKMATE_VALUE   = 999;
    constexpr int CHECK_VALUE       = 0.1;

    // return huge score for checkmate, small bonus for non-checkmate check
    if (kingInCheck(clrToMove)) {
        if ((getPlayerPieces(clrToMove)).empty()) {
            return clrToMove == WHITE ? -CHECKMATE_VALUE : CHECKMATE_VALUE;
        } else {
            return clrToMove == WHITE ? -CHECK_VALUE     : CHECK_VALUE;
        }
    }

    // score white pieces
    for (const SquarePiecePair& whiteMovePair : whitePieces) {
        
        piece  = whiteMovePair.second;
        score += PIECE_VALUES[piece->getPieceType()];
        
    }
    
    // score black pieces
    for (const SquarePiecePair& blackMovePair : blackPieces) {
        
        piece  = blackMovePair.second;
        score -= PIECE_VALUES[piece->getPieceType()];

    }
    
    return score;
}