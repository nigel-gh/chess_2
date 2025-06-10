// Piece.h

#ifndef PIECE_H
#define PIECE_H

#include "../TypesAndEnums.h"
#include "../ZobristHash.h"
#include <list>

class Board;
class MoveSet;
class Move;

class Piece {
private:
    int     displayChar;
    Colour  colour;
    bool    hasMoved;
protected:
    bool addMoveIfNotCheck(int, int, int, bool, bool, Colour, Board*, MoveSet*) const;
public:
    Piece();
    Piece(Colour, int);
    Piece(Colour, int, bool);
    virtual ~Piece();
    virtual MoveSet* getLegalMoves(Board*, int) const = 0;
    virtual Piece* clone()                      const = 0;
    virtual PieceType getPieceType()            const = 0;
    void setDisplayChar(int);
    void setColour(Colour);
    void setPieceType(PieceType);
    char getDisplayChar() const;
    Colour getColour() const;
    bool isActive() const;
    void activate();
    void deactivate();
    void setHasMoved(bool);
    void flipHasMoved();
    bool getHasMoved() const;
    bool operator==(const Piece& other) const;
    std::string getDebugString() const;
};

struct PieceHash { std::size_t operator()(const Piece* piece) const; };
struct PiecePairSetHash { std::size_t operator()(const std::pair<int, Piece*> squarePiecePair) const; };

typedef std::pair               < Piece* , std::list< Piece* > >        PiecePieceListPair;
typedef std::unordered_multiset < Piece& , PieceHash >                  PieceSet;
typedef std::pair               < int , Piece* >                        SquarePiecePair;
typedef std::unordered_multiset < SquarePiecePair, PiecePairSetHash >   SquarePiecePairSet;

void DEBUGGING_display(SquarePiecePair);
void DEBUGGING_display(SquarePiecePairSet);

class BoardPieceMap {
private:
    PiecePieceListPair* boardPieceMap;  // pointer to an array of PiecePieceListPair[64]
    SquarePiecePairSet whitePieces;
    SquarePiecePairSet blackPieces;
    ZobristHash        zobHash;
    BoardPieceMap(PiecePieceListPair*, SquarePiecePairSet, SquarePiecePairSet, ZobristHash);
public:
    BoardPieceMap();
    ~BoardPieceMap();
    void capturePiece(int);
    void uncapturePiece(int);
    void moveActivePiece(int, int);
    void spawnPieceIntoExistance(int, Piece*);
    void promotePiece(PieceType, Colour, int);
    void unpromotePiece(int);
    void flipPieceHasMoved(int);
    Piece* getPieceAtPos(int) const;
    SquarePiecePairSet& getPlayerPieces(Colour);
    unsigned long long getZobHashCurrentKey() const;
    int getNumTimesPosOccurred(unsigned long long zobKey) const;
    void incrementHashAtCurrentPosition();
    void decrementHashAtCurrentPosition();
    BoardPieceMap* clone() const;
};

#endif