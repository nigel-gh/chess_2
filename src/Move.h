// Move.h

/*
    Contains: Move, MoveStack, MoveSet
*/

#ifndef MOVE_H
#define MOVE_H

#include "TypesAndEnums.h"
#include <functional>   //  
#include <cstdint>      // uint64_t
#include <set>
#include <string>

enum PieceType;
class Piece;

class Move {
private:
    // value of -1 means not applicable. Ex. captureSqure=-1 => no piece was captured 
    int piecePosBefore;
    int piecePosAfter;
    int captureSquare; // not necessarily piecePosAfter (enpassent)
    int castleStartSquare;
    int castleEndSquare;
    int enpassantSquareAfter;
    PieceType pieceTypeBefore;
    PieceType pieceTypeAfter;
    bool flipHasMoved;

public:
    Move();
    Move(int, int, int, int, int, int, PieceType, PieceType, bool);
    ~Move() = default;
    bool operator==(const Move& other) const;
    void display() const;
    const std::string toDebugString() const;
    void verboseDisplay() const;
    bool pieceBeingRemoved() const;
    uint64_t pack() const;    
    int             getPiecePosBefore()         const;
    int             getPiecePosAfter()          const;
    int             getCaptureSquare()          const; // not necessarily piecePosAfter (enpassent)
    PieceType       getPieceTypeAfter()         const;
    PieceType       getPieceTypeBefore()        const;
    bool            getFlipHasMoved()           const;
    int             getCastleSquareStart()      const;
    int             getCastleSquareEnd()        const;
    int             getEnpassentSquareAfter()   const;
    PieceType       getPromotionType()          const;
    bool            moveIsCastle()              const;
    Move*           clone()                     const;
};

// MoveStack

class MoveStack {
private:
    std::vector<const Move*> moveStack;
public:
    MoveStack() = default;
    void        push(const Move* move);
    const Move* pop();
    const Move* peek()      const;
    bool        isEmpty()   const;
    size_t      size()      const;
    MoveStack   clone()     const;

    MoveStack(const MoveStack&);
    MoveStack& operator=(const MoveStack&) = delete;
};

// MoveSet

struct MovePtrLessThan {
    bool operator()(Move* lhs, Move* rhs) const;
};

typedef std::set<Move*, MovePtrLessThan> Mvs;

class MoveSet {
    private:
        Mvs moves;
        int rankFileToBoardIndex(std::string) const;
    public:
        MoveSet();
        ~MoveSet();
        void            addMove(const Move*);
        void            addMoveSet(MoveSet*);
        std::string     toDisplayString()           const;
        const Mvs&      getMoves()                  const;
        bool            isEmpty()                   const;
        const Move*     getMoveClone(std::string)     const;
        const Move*     getMoveClone(int)             const;
        size_t          size()                      const;
};

#endif