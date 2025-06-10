// Piece.cpp

#include "Piece.h"
#include "Rook.h"
#include "King.h"
#include "Bishop.h"
#include "Knight.h"
#include "Queen.h"
#include "Pawn.h"
#include "../Board.h"
#include "../Move.h"

#include <iostream>
#include <stdexcept>

Piece::Piece(Colour _colour, int _displayChar) {
    colour      = _colour;
    displayChar = _displayChar;
    hasMoved    = false;
}

Piece::Piece() {
    displayChar = '?';
    colour = WHITE;
    hasMoved = false;
}

Piece::Piece(Colour _colour, int _displayChar, bool _hasMoved) {
    colour      = _colour;
    displayChar = _displayChar;
    hasMoved    = _hasMoved;
}

Piece::~Piece() = default;

void Piece::setHasMoved(bool _hasMoved) {
    hasMoved = _hasMoved;
}

bool Piece::getHasMoved() const {
    return hasMoved;
}

void Piece::setDisplayChar(int ch) {
    displayChar = ch;
}

void Piece::setColour(Colour _colour) {
    colour = _colour;
}

char Piece::getDisplayChar() const {
    return displayChar;
}

Colour Piece::getColour() const {
    return colour;
}

bool Piece::operator==(const Piece& other) const {
    return displayChar == other.displayChar;
}

bool Piece::addMoveIfNotCheck(int startSquare, int endSquare, int enpassentSquare, bool hasMoved, bool isPromotion, Colour startSquareColour, Board* board, MoveSet* moves) const {
    
    Colour endSquareColour = board->getSquareColour(endSquare);
    Move* proposedMove;
    PieceType pieceTypeBefore = board->getPieceTypeAtPos(startSquare);
    PieceType pieceTypeAfter  = board->getPieceTypeAtPos(endSquare);

    bool flipHasMoved = !hasMoved;
    
    if (    startSquareColour == endSquareColour || pieceTypeAfter == KING) {
        
        return false;
        
    } else {
        
        bool returnValue;
        int captureSquare;
        
        if (endSquareColour == NO_COLOUR) {
            returnValue = true;
            captureSquare = -1;
        } else {
            returnValue = false;
            captureSquare = endSquare;
        }
        
        if (isPromotion) {
            // Promote to rook
            proposedMove = new Move(startSquare, endSquare, captureSquare, -1, -1, enpassentSquare, pieceTypeBefore, ROOK, flipHasMoved);
            board->applyMove(proposedMove, startSquareColour);
            if (!board->kingInCheck(startSquareColour)) {
                moves->addMove(proposedMove);
            }
            board->undoMove();
            
            // Promote to biship
            proposedMove = new Move(startSquare, endSquare, captureSquare, -1, -1, enpassentSquare, pieceTypeBefore, BISHOP, flipHasMoved);
            board->applyMove(proposedMove, startSquareColour);
            if (!board->kingInCheck(startSquareColour)) {
                moves->addMove(proposedMove);
            }
            board->undoMove();
            
            // Promote to knight
            proposedMove = new Move(startSquare, endSquare, captureSquare, -1, -1, enpassentSquare, pieceTypeBefore, KNIGHT, flipHasMoved);
            board->applyMove(proposedMove, startSquareColour);
            if (!board->kingInCheck(startSquareColour)) {
                moves->addMove(proposedMove);
            }
            board->undoMove();
            
            // Promote to queen
            proposedMove = new Move(startSquare, endSquare, captureSquare, -1, -1, enpassentSquare, pieceTypeBefore, QUEEN, flipHasMoved);
            board->applyMove(proposedMove, startSquareColour);
            if (!board->kingInCheck(startSquareColour)) {
                moves->addMove(proposedMove);
            }
            board->undoMove();
            
            return returnValue;
            
        } else { // is NOT a promotion
            
            proposedMove = new Move(startSquare, endSquare, captureSquare, -1, -1, enpassentSquare, pieceTypeBefore, pieceTypeBefore, flipHasMoved);
            board->applyMove(proposedMove, startSquareColour);
            if (!board->kingInCheck(startSquareColour)) {
                moves->addMove(proposedMove);
            }
            board->undoMove();
            return returnValue;
        
        }
    }
}

void Piece::flipHasMoved() {
    hasMoved = !hasMoved;
}

std::string Piece::getDebugString() const {
    return "Piece::getDebugString(): " + colourToString(colour) + "::" + pieceTypeToString(getPieceType());
}

unsigned long long BoardPieceMap::getZobHashCurrentKey() const {
    return zobHash.getCurrentPositionHashKey();
}

int BoardPieceMap::getNumTimesPosOccurred(unsigned long long zobKey) const {
    return zobHash.getNumberOfTimesPositionOccured(zobKey);
}


BoardPieceMap::BoardPieceMap() {

    boardPieceMap = new PiecePieceListPair[64];

    for (int i = 0; i < 64; i++) {
        boardPieceMap[i] = std::make_pair<Piece*, std::list<Piece*>>(nullptr, std::list<Piece*>{});
    }
    whitePieces = SquarePiecePairSet();
    blackPieces = SquarePiecePairSet();
    zobHash     = ZobristHash();
    Piece* pieceToAdd;

    // WHITE //

    pieceToAdd = new Rook(WHITE);
    spawnPieceIntoExistance(WHITE_LEFT_ROOK_START_SQUARE,       pieceToAdd);
    
    pieceToAdd = new Knight(WHITE);
    spawnPieceIntoExistance(WHITE_LEFT_KNIGHT_START_SQUARE,     pieceToAdd);
    
    pieceToAdd = new Bishop(WHITE);
    spawnPieceIntoExistance(WHITE_LEFT_BISHOP_START_SQUARE,     pieceToAdd);
    
    pieceToAdd = new Queen(WHITE);
    spawnPieceIntoExistance(WHITE_QUEEN_START_SQUARE,           pieceToAdd);
    
    pieceToAdd = new King(WHITE);
    spawnPieceIntoExistance(WHITE_KING_START_SQUARE,            pieceToAdd);
    
    pieceToAdd = new Bishop(WHITE);
    spawnPieceIntoExistance(WHITE_RIGHT_BISHOP_START_SQUARE,    pieceToAdd);
    
    pieceToAdd = new Knight(WHITE);
    spawnPieceIntoExistance(WHITE_RIGHT_KNIGHT_START_SQUARE,    pieceToAdd);

    pieceToAdd = new Rook(WHITE);
    spawnPieceIntoExistance(WHITE_RIGHT_ROOK_START_SQUARE,      pieceToAdd);
    
    for (int i = WHITE_PAWN_RANK_START_SQUARE; i <= WHITE_PAWN_RANK_END_SQUARE; i++ ) {
        pieceToAdd = new Pawn(WHITE);
        spawnPieceIntoExistance(i, pieceToAdd);   
    }
    
    // BLACK //
    
    pieceToAdd = new Rook(BLACK);
    spawnPieceIntoExistance(BLACK_LEFT_ROOK_START_SQUARE,       pieceToAdd);
    
    pieceToAdd = new Knight(BLACK);
    spawnPieceIntoExistance(BLACK_LEFT_KNIGHT_START_SQUARE,     pieceToAdd);
    
    pieceToAdd = new Bishop(BLACK);
    spawnPieceIntoExistance(BLACK_LEFT_BISHOP_START_SQUARE,     pieceToAdd);
    
    pieceToAdd = new Queen(BLACK);
    spawnPieceIntoExistance(BLACK_QUEEN_START_SQUARE,           pieceToAdd);
    
    pieceToAdd = new King(BLACK);
    spawnPieceIntoExistance(BLACK_KING_START_SQUARE,            pieceToAdd);
    
    pieceToAdd = new Bishop(BLACK);
    spawnPieceIntoExistance(BLACK_RIGHT_BISHOP_START_SQUARE,    pieceToAdd);
    
    pieceToAdd = new Knight(BLACK);
    spawnPieceIntoExistance(BLACK_RIGHT_KNIGHT_START_SQUARE,    pieceToAdd);
    
    pieceToAdd = new Rook(BLACK);
    spawnPieceIntoExistance(BLACK_RIGHT_ROOK_START_SQUARE,      pieceToAdd);
    
    for (int i = BLACK_PAWN_RANK_START_SQUARE; i <= BLACK_PAWN_RANK_END_SQUARE; i++ ) {
        pieceToAdd = new Pawn(BLACK);
        spawnPieceIntoExistance(i, pieceToAdd);  
    }

    zobHash.incrementHashAtCurrentPosition();
}

BoardPieceMap::~BoardPieceMap() {

    for (int i = 0; i < 64; ++i) {
 
        if (boardPieceMap[i].first != nullptr) {
            
            delete boardPieceMap[i].first;
            boardPieceMap[i].first = nullptr;

        }

        for (Piece* p : boardPieceMap[i].second) {
            delete p;
        }
        boardPieceMap[i].second.clear();
    }

    whitePieces.clear();
    blackPieces.clear();

}


void BoardPieceMap::capturePiece(int captureSquare) {
    Piece* capturedPiece = boardPieceMap[captureSquare].first;
    // update zobhash
    zobHash.hashZobristPiece(capturedPiece->getColour(), capturedPiece->getPieceType(), captureSquare);
    boardPieceMap[captureSquare].second.push_back(capturedPiece);
    boardPieceMap[captureSquare].first = nullptr;
    SquarePiecePairSet* playerPieces = capturedPiece->getColour() == WHITE ? &whitePieces : &blackPieces;
    playerPieces->erase(std::make_pair(captureSquare, capturedPiece));
}

// ASSUMES (perhaps quite rashly) THAT THE LSIT IS NOT EMPTY WHEN UNCAPTURING THE PEICE
void BoardPieceMap::uncapturePiece(int uncaptureSquare) {
    Piece* recoveredPiece = boardPieceMap[uncaptureSquare].second.back();
    zobHash.hashZobristPiece(recoveredPiece->getColour(), recoveredPiece->getPieceType(), uncaptureSquare);
    boardPieceMap[uncaptureSquare].second.pop_back();
    boardPieceMap[uncaptureSquare].first = recoveredPiece;
    SquarePiecePairSet& playerPieces = recoveredPiece->getColour() == WHITE ? whitePieces : blackPieces;
    playerPieces.insert(std::make_pair(uncaptureSquare, recoveredPiece));
}

void BoardPieceMap::moveActivePiece(int from, int to) {

    Piece* movingPiece = boardPieceMap[from].first;
    
    if (!movingPiece) {
        std::cerr << "ERROR: moveActivePiece called on nullptr at from = " << from << std::endl;
        throw std::runtime_error("moveActivePiece: nullptr piece");
    }
    
    if (movingPiece == nullptr) {
        throw std::runtime_error("moveActivePiece: nullptr piece");
    }
    
    Colour pieceColour = movingPiece->getColour();
    PieceType pieceType = movingPiece->getPieceType();

    boardPieceMap[to].first = movingPiece;
    boardPieceMap[from].first = nullptr;
    SquarePiecePair oldPair = std::make_pair(from, movingPiece);
    SquarePiecePair newPair = std::make_pair(to, movingPiece);
    SquarePiecePairSet& playerPieces = movingPiece->getColour() == WHITE ? whitePieces : blackPieces;
    playerPieces.erase(oldPair);
    playerPieces.insert(newPair);

    zobHash.hashZobristPiece( pieceColour, pieceType, from );
    zobHash.hashZobristPiece( pieceColour, pieceType, to   );

}

void BoardPieceMap::spawnPieceIntoExistance(int square, Piece* newPiece) {

    boardPieceMap[square].first = newPiece;
    SquarePiecePairSet& playerPieces = newPiece->getColour() == WHITE ? whitePieces : blackPieces;
    playerPieces.insert(std::make_pair(square, newPiece));

    zobHash.hashZobristPiece(newPiece->getColour(), newPiece->getPieceType(), square);

}

void BoardPieceMap::promotePiece(PieceType promotionType, Colour colour, int square) {

    Piece* pieceBefore = boardPieceMap[square].first;
    SquarePiecePair oldPair = std::make_pair(square, pieceBefore);

    zobHash.hashZobristPiece(colour, pieceBefore->getPieceType(), square);

    capturePiece(square);
    Piece* newPiece;
    switch (promotionType) {
        case ROOK:
            newPiece = new Rook(colour);
            break;
        case KNIGHT:
            newPiece = new Knight(colour);
            break;
        case BISHOP:
            newPiece = new Bishop(colour);
            break;
        case QUEEN:
            newPiece = new Queen(colour);
            break;
        default:
            throw std::invalid_argument("Invalid promotion type provided (" + std::to_string(promotionType) + ", i.e.: " + pieceTypeToString(promotionType) + ")");
            break;
    }

    boardPieceMap[square].first = newPiece;

    zobHash.hashZobristPiece(colour, promotionType, square);

    SquarePiecePair newPair = std::make_pair(square, newPiece);
    SquarePiecePairSet& playerPieces = colour == WHITE ? whitePieces : blackPieces;
    playerPieces.erase(oldPair);
    playerPieces.insert(newPair);

}

void BoardPieceMap::unpromotePiece(int square) {

    Piece*              pieceToDelete   = boardPieceMap[square].first;
    Colour colour = pieceToDelete->getColour();
    zobHash.hashZobristPiece(colour, pieceToDelete->getPieceType(), square);

    SquarePiecePair     oldPair         = std::make_pair(square, pieceToDelete);
    SquarePiecePairSet& playerPieces    = pieceToDelete->getColour() == WHITE ? whitePieces : blackPieces;
    playerPieces.erase(oldPair);
    
    // delete pieceToDelete;
    uncapturePiece(square);
    
    Piece* unpromotedPiece = boardPieceMap[square].first;
    SquarePiecePair newPair = std::make_pair(square, unpromotedPiece);
    playerPieces.insert(newPair);

    zobHash.hashZobristPiece(colour, unpromotedPiece->getPieceType(), square);
    
}

Piece* BoardPieceMap::getPieceAtPos(int square) const {
    return boardPieceMap[square].first;
}


SquarePiecePairSet& BoardPieceMap::getPlayerPieces(Colour _colour) {
    return _colour == WHITE ? whitePieces : blackPieces;
}

void BoardPieceMap::incrementHashAtCurrentPosition() {
    zobHash.incrementHashAtCurrentPosition();
}

void BoardPieceMap::decrementHashAtCurrentPosition() {
    zobHash.decrementHashAtCurrentPosition();
}

void BoardPieceMap::flipPieceHasMoved(int pos) {
    boardPieceMap[pos].first->flipHasMoved();
}

BoardPieceMap::BoardPieceMap(

    PiecePieceListPair* _map,
    SquarePiecePairSet _white,
    SquarePiecePairSet _black,
    ZobristHash _zob
) : 
    boardPieceMap   ( _map                 ),
    whitePieces     (   std::move(_white)  ),
    blackPieces     (   std::move(_black)  ),
    zobHash         (   std::move(_zob)    ) {}

BoardPieceMap* BoardPieceMap::clone() const {

    PiecePieceListPair* newBoardPieceMap = new PiecePieceListPair[64];

    for (int i = 0; i < 64; ++i) {

        Piece* originalPiece = boardPieceMap[i].first;
        Piece* clonedPiece   = nullptr;
        
        if (originalPiece != nullptr) {
            clonedPiece = originalPiece->clone();
        }

        std::list<Piece*> clonedList;
        
        for (Piece* p : boardPieceMap[i].second) {
            if (p) {
                clonedList.push_back(p->clone());  // deep copy each piece
            } else {
                clonedList.push_back(nullptr);
            }
        }

        newBoardPieceMap[i] = std::make_pair(clonedPiece, std::move(clonedList));
    }

    SquarePiecePairSet clonedWhitePieces;
    for (int i = 0; i < 64; ++i) {
        Piece* piece = newBoardPieceMap[i].first;
        if (piece && piece->getColour() == WHITE) {
            clonedWhitePieces.insert({i, piece});
        }
    }
    
    SquarePiecePairSet clonedBlackPieces;
    for (int i = 0; i < 64; ++i) {
        Piece* piece = newBoardPieceMap[i].first;
        if (piece && piece->getColour() == BLACK) {
            clonedBlackPieces.insert({i, piece});
        }
    }

    ZobristHash clonedHash = zobHash;

    return new BoardPieceMap(newBoardPieceMap, clonedWhitePieces, clonedBlackPieces, clonedHash);

}

void DEBUGGING_display(SquarePiecePairSet pairSet) {
    std::cout << "DEBUGGING: printing SquarePiecePairSet" << std::endl;
    for (auto it = pairSet.begin(); it != pairSet.end(); ++it) {
        DEBUGGING_display((*it));
    }
    std::cout << "DEBUGGING: DONE printing SquarePiecePairSet" << std::endl;
}

void DEBUGGING_display(SquarePiecePair pair) {
    std::cout << "pair<" << pair.first << "," << pair.second->getDisplayChar() << ">" << std::endl;
}

std::size_t PiecePairSetHash::operator()(const std::pair<int, Piece*> squarePiecePair) const {
    const int square = squarePiecePair.first;
    Piece* piece = squarePiecePair.second;
    std::size_t hashSquare = std::hash<int>{}(square);
    std::size_t hashPiece = std::hash<char>{}(piece->getDisplayChar());    
    return hashSquare ^ (hashPiece << 1);  // Simple combination of the two
}

std::size_t PieceHash::operator()(const Piece* piece) const {
    return std::hash<int>()(piece->getDisplayChar());
}