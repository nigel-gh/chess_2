// BruteForcePlayer.cpp

#include "../Player.h"
#include "BruteForcePlayer.h"
#include "../../Board.h"
#include "../../Move.h"
#include "../../Pieces/Piece.h"
#include "../../TypesAndEnums.h"

#include <iostream>
#include <stdlib.h>

BruteForcePlayer::BruteForcePlayer() : Player(WHITE) {
    maxSearchDepth = DEFAULT_DEPTH_OF_BRUTE_FORCE_PLAYER;
}

BruteForcePlayer::BruteForcePlayer(Colour _colour) : Player(_colour) {
    maxSearchDepth = DEFAULT_DEPTH_OF_BRUTE_FORCE_PLAYER;
}

BruteForcePlayer::BruteForcePlayer(Colour _colour, int _searchDepth) : Player(_colour), maxSearchDepth(_searchDepth) {}

BruteForcePlayer::~BruteForcePlayer() {}

const Action BruteForcePlayer::chooseAction(const MoveSet* possibleMoves, const Board* board) const {

    Board*          boardClone      = board->clone();
    int             moveScore;
    int             bestMoveScore   = -999;
    const Move*     bestMove        = nullptr;
    Colour          clr             = getColour();
    int             miniMaxSign     = clr * 2 - 1;
    int             DEBUGGING_numNodesSearched = 0;
    
    for (const Move* move : possibleMoves->getMoves()) {
        
        boardClone->applyMove(move, clr);
        
        if (maxSearchDepth <= 1) {                  // leaf node
            moveScore = evaluateBoardScore(boardClone);
        } else {                                    // recursion
            moveScore = getMoveScoreRecursively(2, boardClone, getOppositeColour(clr), &DEBUGGING_numNodesSearched);
        }
        
        boardClone->undoMove();
        
        if ((moveScore * miniMaxSign) > bestMoveScore) {
            bestMoveScore = moveScore;
            bestMove = move;
        }
        
    }
        
    delete boardClone;
    
    std::cout << "BruteForcePlayer.cpp::chooseAction()::debugging, searched " << DEBUGGING_numNodesSearched << " positions\n";

    return Action(MOVE, bestMove->clone());
}

int BruteForcePlayer::getMoveScoreRecursively(int depth, Board* board, Colour clr, int* DEBUGGING_numNodesSearched) const {
    
    int             moveScore;
    int             bestMoveScore   = -999;
    MoveSet*        moveSet         = board->calcLegalMoves(clr);
    int             miniMaxSign     = clr * 2 - 1;    
    
    for (const Move* move : moveSet->getMoves()) {
        
        (*DEBUGGING_numNodesSearched)++;

        board->applyMove(move, clr);
        
        if (depth >= maxSearchDepth) {              // leaf node
            moveScore = evaluateBoardScore(board);
        } else {                                    // recursion
            moveScore = getMoveScoreRecursively(depth + 1, board, getOppositeColour(clr), DEBUGGING_numNodesSearched);
        }
        
        board->undoMove();
        
        if ((moveScore * miniMaxSign) > bestMoveScore) {
            bestMoveScore = moveScore;
        }
        
    }
    
    return bestMoveScore;

}

// higher score     = more helpful for white
// negative score   = more helpful for black
int BruteForcePlayer::evaluateBoardScore(Board* board) const {
    
    int score = 0;
    const Piece* piece;
    int sign;
    
    for (int i = 0; i < NUM_SQUARES_ON_BOARD; i++) {
        
        piece = board->getPieceAtPos(i);
        
        if (piece == nullptr) {
            continue;
        }
        
        sign = - 1 * (piece->getColour() * 2 - 1);
        score += PIECE_VALUES[piece->getPieceType()] * sign;
        
    }
    
    return score;
}

Player* BruteForcePlayer::clone() const {
    return new BruteForcePlayer(getColour(), maxSearchDepth);
}