// King.h
#ifndef KING_H
#define KING_H

#include "Piece.h"

class Board;
class Move;

constexpr int WHITE_KING_SIDE_CASTLE_SQUARES_TO_CHECK [2] = {WHITE_RIGHT_BISHOP_START_SQUARE, WHITE_RIGHT_KNIGHT_START_SQUARE}; 
constexpr int WHITE_QUEEN_SIDE_CASTLE_SQUARES_TO_CHECK[3] = {WHITE_QUEEN_START_SQUARE, WHITE_LEFT_BISHOP_START_SQUARE, WHITE_LEFT_KNIGHT_START_SQUARE}; 
constexpr int BLACK_KING_SIDE_CASTLE_SQUARES_TO_CHECK [2] = {BLACK_RIGHT_BISHOP_START_SQUARE, BLACK_RIGHT_KNIGHT_START_SQUARE}; 
constexpr int BLACK_QUEEN_SIDE_CASTLE_SQUARES_TO_CHECK[3] = {BLACK_QUEEN_START_SQUARE, BLACK_LEFT_BISHOP_START_SQUARE, BLACK_LEFT_KNIGHT_START_SQUARE}; 

class King : public Piece {
public:
    King(Colour);
    King(Colour, bool);
    ~King() = default;
    MoveSet* getLegalMoves(Board*, int) const override;
    Piece* clone() const override;
    PieceType getPieceType() const override;
private:
    const bool castleIsAllowed(Board*, int, bool) const;
};

#endif