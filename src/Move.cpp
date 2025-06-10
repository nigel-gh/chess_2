//Move.cpp
#include "Move.h"
#include <cstdint>
#include <string>
#include <unordered_set>
#include <iostream>
#include <stdlib.h>

Move::Move() :
    piecePosBefore(-1),
    piecePosAfter(-1),
    captureSquare(-1),
    castleStartSquare(-1),
    castleEndSquare(-1),
    enpassantSquareAfter(-1),
    pieceTypeBefore(NULL_PIECE),
    pieceTypeAfter(NULL_PIECE),
    flipHasMoved(false) {}

Move::Move(

    int             _piecePosBefore,
    int             _piecePosAfter,
    int             _captureSquare,
    int             _castleStartSquare,
    int             _castleEndSquare,
    int             _enpassantSquareAfter,
    PieceType       _pieceTypeBefore,
    PieceType       _pieceTypeAfter,
    bool            _flipHasMoved
) :
    piecePosBefore          ( _piecePosBefore      ),
    piecePosAfter           ( _piecePosAfter       ),
    captureSquare           ( _captureSquare       ),
    castleStartSquare       ( _castleStartSquare   ),
    castleEndSquare         ( _castleEndSquare     ),
    enpassantSquareAfter    ( _enpassantSquareAfter),
    pieceTypeBefore         ( _pieceTypeBefore     ),
    pieceTypeAfter          ( _pieceTypeAfter      ),
    flipHasMoved            ( _flipHasMoved        ) {}

void Move::display() const {
    std::string startSquare = squareToString(piecePosBefore);
    std::string endSquare = squareToString(piecePosAfter);
    std::cout           << "Move: " << startSquare << " -> " << endSquare << std::endl;
}

const std::string Move::toDebugString() const {

    std::string str = "";
    str += "MOVE(" + pieceTypeToString(pieceTypeBefore) + ", " + pieceTypeToString(pieceTypeAfter) + "): " + squareToString(piecePosBefore) + "->" + squareToString(piecePosAfter) + "flipHasMoved=" + boolToString(flipHasMoved);
    return str;

}

void Move::verboseDisplay() const {
    std::cout << "VERBOSE MOVE:"
        << "\npiecePosBefore ......... " << piecePosBefore 
        << "\npiecePosAfter .......... " << piecePosAfter
        << "\ncaptureSquare .......... " << captureSquare
        << "\ncastleStartSquare ...... " << castleStartSquare
        << "\ncastleEndSquare ........ " << castleEndSquare
        << "\nenpassantSquareAfter ... " << enpassantSquareAfter
        << "\npieceTypeBefore ........ " << pieceTypeToString(pieceTypeBefore)
        << "\npieceTypeAfter ......... " << pieceTypeToString(pieceTypeAfter)
        << "\nflipHasMoved ........... " << flipHasMoved
        << std::endl;
}

bool Move::pieceBeingRemoved() const {
        return true;
}

uint64_t Move::pack() const {
    uint64_t packed = 0;
    packed ^= static_cast<uint64_t>(piecePosBefore)         << 3 ;
    packed ^= static_cast<uint64_t>(piecePosAfter)          << 11;
    packed ^= static_cast<uint64_t>(captureSquare)          << 19;
    packed ^= static_cast<uint64_t>(castleStartSquare)      << 27;
    packed ^= static_cast<uint64_t>(castleEndSquare)        << 35;
    packed ^= static_cast<uint64_t>(pieceTypeBefore)        << 37;
    packed ^= static_cast<uint64_t>(pieceTypeAfter)         << 36;
    packed ^= static_cast<uint64_t>(flipHasMoved)           << 41;
    packed ^= static_cast<uint64_t>(enpassantSquareAfter)   << 49;
    return packed;
}

// MoveStack
void MoveStack::push(const Move* move) {
    moveStack.push_back(move);
}

const Move* MoveStack::pop() {
    if (isEmpty()) {
        throw std::out_of_range("Stack is empty. Cannot pop.");
    }
    const Move* move = moveStack.back();
    moveStack.pop_back();
    return move;
}

const Move* MoveStack::peek() const {
    if (isEmpty()) {
        throw std::out_of_range("Stack is empty. Cannot access top.");
    }
    return moveStack.back();
}

bool MoveStack::isEmpty() const {
    return moveStack.empty();
}

size_t MoveStack::size() const {
    return moveStack.size();
}

MoveStack MoveStack::clone() const {

    MoveStack newStack;

    for (const Move* movePtr : moveStack) {
        if (movePtr != nullptr) {
            newStack.push(movePtr->clone());
        } else {
            newStack.push(nullptr);
        }
    }

    return newStack;
}

MoveStack::MoveStack(const MoveStack& other) {

    for (const Move* movePtr : other.moveStack) {
    
        if (movePtr != nullptr) {
            moveStack.push_back(movePtr->clone());
        } else {
            moveStack.push_back(nullptr);
        }
    
    }

}

int             Move::getPiecePosBefore()           const {return piecePosBefore;   }
int             Move::getPiecePosAfter()            const {return piecePosAfter;    }
int             Move::getCaptureSquare()            const {return captureSquare;    }
PieceType       Move::getPieceTypeAfter()           const {return pieceTypeAfter;   }
PieceType       Move::getPieceTypeBefore()          const {return pieceTypeBefore;  }
bool            Move::getFlipHasMoved()             const {return flipHasMoved;     }
int             Move::getCastleSquareStart()        const {return castleStartSquare;}
int             Move::getCastleSquareEnd()          const {return castleEndSquare;  }
int             Move::getEnpassentSquareAfter()     const {return enpassantSquareAfter;}
bool            Move::moveIsCastle()                const {return castleStartSquare == -1 ? false : true;}
PieceType       Move::getPromotionType()            const {return pieceTypeBefore == pieceTypeAfter ? NULL_PIECE : pieceTypeAfter;}


// MoveSet
bool MovePtrLessThan::operator()(Move* left, Move* right) const {
    return left->pack() < right->pack();
}

MoveSet::MoveSet() {

}

MoveSet::~MoveSet() {
    for (Move* move : moves) {
        delete move;
    }
    moves.clear();
}


void MoveSet::addMove(const Move* move) {
    moves.insert(move->clone());
}

void MoveSet::addMoveSet(MoveSet* otherMoveSet) {
           
    for (const Move* move : otherMoveSet->getMoves()) {
        moves.insert(move->clone());
    }
    
}

std::string MoveSet::toDisplayString() const {

    std::vector<std::pair<int, std::string>> strMoves[NUM_SQUARES_ON_BOARD];
    std::string str = "";

    
    if (moves.size() == 0) {
        return "EMPTY MOVE SET\n";
    }
    
    for (const auto& move : moves) {

        int startSquare = move->getPiecePosBefore();
        int endSquare   = move->getPiecePosAfter();
        std::string specifier;
        
        if (move->getPieceTypeBefore() != move->getPieceTypeAfter()) {
            specifier = "PROMOTE: " + pieceTypeToString(move->getPieceTypeAfter());
        }
        
        strMoves[startSquare].emplace_back(endSquare, specifier);
    }
    
    for (int i = 0; i < NUM_SQUARES_ON_BOARD; ++i) {
        if (!strMoves[i].empty()) {
            str += squareToString(i) + ": [";
            for (const auto& [endSquare, specifier] : strMoves[i]) {
                if (specifier.empty()) {
                    str += squareToString(endSquare) + ", ";
                } else {
                    str += "\n  " + squareToString(endSquare) + " (" + specifier + ")";
                }
            }
            str += "]\n";
        }
    }
    
    return str;
}

const Mvs& MoveSet::getMoves() const {
    return moves;
}

bool MoveSet::isEmpty() const {
    return moves.empty();
}

size_t MoveSet::size() const {
    return moves.size();
}

// Assumes clean inputs. Give me clean inputs. Please.
int MoveSet::rankFileToBoardIndex(std::string str) const {
    int fileInt = str.at(0) - 'a'; // now in range [0, 7]
    int rankInt = str.at(1) - '1'; // now in range [0, 7]
    return 
        8 * (7 - rankInt)
            + 
        fileInt;
}

const Move* MoveSet::getMoveClone(std::string str) const {

    std::size_t strLen = str.length();
    char file1 = str.at(0);

    const Move* defaultMove = nullptr;

    // Dirty way of closing the program.
    if (file1 == 'x') {
        #ifdef __linux__
            quick_exit(0);
        #elif _WIN32
            exit(-1);
        #else
            std::cout << "ERROR: COULD NOT FIND TARGET PLATFORM!!\n";
        #endif
        
    }

    if (strLen < 4) {
        std::cout << "User provided a string of length " << strLen << ", but it must be 4 characters long." << std::endl;
        return defaultMove;
    }
    if (strLen > 5) {
        std::cout << "User provided a string of length " << strLen << ", but it must be 4 characters long." << std::endl;
        return defaultMove;
    }

    // Starting square
    if (file1 < 'a' || file1 > 'h') {
        std::cout << "User provided file " << file1 << ", but it must within range [a, h]." << std::endl;
        return defaultMove;
    }
    char rank1 = str.at(1);
    if (rank1 < '1' || rank1 > '8') {
        std::cout << "User provided rank " << rank1 << ", but it must within range [1, 8]." << std::endl;
        return defaultMove;
    }
    // Ending square
    char file2 = str.at(2);
    if (file2 < 'a' || file2 > 'h') {
        std::cout << "User provided file " << file2 << ", but it must within range [a, h]." << std::endl;
        return defaultMove;
    }
    char rank2 = str.at(3);
    if (rank2 < '1' || rank2 > '8') {
        std::cout << "User provided rank " << rank2 << ", but it must within range [1, 8]." << std::endl;
        return defaultMove;
    }

    int startSquare = rankFileToBoardIndex(str.substr(0,2));
    int endSquare = rankFileToBoardIndex(str.substr(2,2));
    std::cout << "startSquare: " << startSquare << " endSquare: " << endSquare << std::endl;
    if (startSquare == endSquare) {
        std::cout << "Cannot move piece to the same location!" << std::endl;
        return defaultMove;
    }

    // is move a promotion?
    PieceType promotionPieceType;
    if (strLen == 5) {
        char promoChar = str.at(4);
        if (promoChar == 'r') {
            promotionPieceType = ROOK;
        } else if (promoChar == 'b') {
            promotionPieceType = BISHOP;
        } else if (promoChar == 'n') {
            promotionPieceType = KNIGHT;
        } else if (promoChar == 'q') {
            promotionPieceType = QUEEN;
        } else {
            std::cout << "Unkown promotion piece type \"" << promoChar << "\" (must be \"r\", \"b\", \"n\", \"q\")"<< std::endl;
            return defaultMove;
        }
    } else { // Move must not be a promotion
        promotionPieceType = NULL_PIECE;
    }

    // now have the start and end square - how to find the damn thing?
    for (const Move* move : moves) {
        if (move->getPiecePosBefore() == startSquare && move->getPiecePosAfter()  == endSquare && move->getPromotionType() == promotionPieceType) {
            move->display();
            return move->clone();
        }
    }

    return defaultMove;
}

// get a move at an index
// O(n) time because i suck
const Move* MoveSet::getMoveClone(int index) const {
    
    if (index < 0 || index >= static_cast<int>(moves.size())) {
        throw std::out_of_range(std::string("out of range in MoveSet::getMove() - passed value ") + std::to_string(index));
    }

    auto it = moves.begin();
    std::advance(it, index);

    return (*it)->clone();
}

Move* Move::clone() const {
    return new Move(
        piecePosBefore,
        piecePosAfter,
        captureSquare,
        castleStartSquare,
        castleEndSquare,
        enpassantSquareAfter,
        pieceTypeBefore,
        pieceTypeAfter,
        flipHasMoved
    );
}








