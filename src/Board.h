// Board.h

/*
   Much of the functionality is in Board, not Player or Piece.
*/

#ifndef BOARD_H
#define BOARD_H

#include "TypesAndEnums.h"
#include "Move.h"
#include "Pieces/Piece.h"
// #include "CStack.h"

#include <string>
#include <vector>

class Player;
class ZobristHash;
class BoardPieceMap;
class Piece;
struct OnBoardMoves;

class Board {
private:
   int            enpassantSquare;
   int            whiteKingPos;  // applyMove() responsible for changing this
   int            blackKingPos;
   unsigned int   numMovesPlayed;
   unsigned int   fiftyMoveRuleStack[MAX_NUM_MOVES_IN_GAME];
   BoardPieceMap* pieceAtSquareMap; // hash where key is board index, value is list of pieces that exist there or have been captured
   MoveStack      moveStack;
   // CStack<bool, 512> whiteKingInCheckStack; // UNIMPLEMENTED.
   // CStack<bool, 512> blackKingInCheckStack;
   void        setKingPos(Colour, int);
   void        incrementFiftyMoveRuleCounter();
   void        decrementFiftyMoveRuleCounter();
   void        resetFiftyMoveRuleCounter();
   bool        kingInCheckFromDirectMovingPiece(Colour, OnBoardMoves, PieceType) const;
   bool        kingInCheckFromKnightOrPawn(Colour, OnBoardMoves) const;
   bool        kingInCheckFromKing(Colour, OnBoardMoves) const;
   Board(
      int,
      int,
      int,
      unsigned int,
      unsigned int*,
      BoardPieceMap*,
      const MoveStack&
   );
public:
   Board();
   ~Board() = default;
   const Piece*   getPieceAtPos(int)               const;
   Colour         getSquareColour(int)             const;
   PieceType      getPieceTypeAtPos(int)           const;
   MoveSet*       calcLegalMoves(Colour);
   const int      getEnpassantSquare()             const;
   int            getKingPos(Colour)               const;
   void           setEnpassantSquare(int);
   void           display(const MoveSet*, Colour)  const;
   GameStatus     applyMove(const Move*, Colour);                  // WARNING: MUST IMPLEMENT CHANGING THE GAME STATUS HERE, RIGHTN NOW IT JUST ALWAYS RETURNS THAT THE GAME CONTINUES //
   void           undoMove();
   void           undoMove(int);
   bool           squaresAreEnemies(int, int)      const;
   bool           kingInCheck(Colour)              const;
   Board*         clone()                          const;
   SquarePiecePairSet& getPlayerPieces(Colour);
   int            evaluateLeafPositionScore(Colour); //const
};

#endif