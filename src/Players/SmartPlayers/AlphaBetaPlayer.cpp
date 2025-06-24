// ABPlayer.cpp

#include "AlphaBetaPlayer.h"
#include "../../Pieces/Piece.h"

#include <iostream>
#include <stdlib.h>

ABPlayer::ABPlayer() : Player(WHITE) {
    maxSearchDepth = DEFAULT_DEPTH_OF_ALPHA_BETA_PLAYER;
}

ABPlayer::ABPlayer(Colour _colour) : Player(_colour) {
    maxSearchDepth = DEFAULT_DEPTH_OF_ALPHA_BETA_PLAYER;
}

ABPlayer::ABPlayer(Colour _colour, int _searchDepth) : Player(_colour), maxSearchDepth(_searchDepth) {}

ABPlayer::~ABPlayer() {}

const Action ABPlayer::chooseAction(const MoveSet* possibleMoves, const Board* board) const {

    Board*      boardClone  = board->clone();
    MoveSet*    moves       = boardClone->calcLegalMoves(getColour());
    Colour      clr         = getColour();
    int         alpha       = EVAL_MIN_SCORE;
    int         beta        = EVAL_MAX_SCORE;
    int         score;
    int         bestScore   = EVAL_MIN_SCORE;
    Move*       bestMove;
    
    int DEBUGGING_numPositionsEvaluated = 0;

    for (Move* move : moves->getMoves())  {
        
        boardClone->applyMove(move, clr);

        score = -1 * alphaBeta( -beta, -alpha, maxSearchDepth - 1, boardClone, getOppositeColour(clr), &DEBUGGING_numPositionsEvaluated);

        boardClone->undoMove();
      
        if ( score > bestScore ) {
         
            bestScore   = score;
            bestMove    = move;
            alpha       = std::max(alpha, score);
            
        }
        
        if ( score >= beta ) {
            break;
        }
    }
    
    bestMove = bestMove->clone();
    delete moves;
    std::cout << "AlphaBetaPlayer.cpp::chooseAction()::debugging, searched " << DEBUGGING_numPositionsEvaluated << " positions\n";
    return Action(ActionType::MOVE, bestMove);
    
}

// adapted from chessprogramming.org/Alpha-Beta
int ABPlayer::alphaBeta(int alpha, int beta, int depthRemaining, Board* board, Colour clr, int* DEBUGGING_numNodesEvaluated) const {
   
    int         score;
    int         bestScore   = EVAL_MIN_SCORE;
    MoveSet*    moves       = board->calcLegalMoves(clr);
    // Termination condition
    if ( depthRemaining == 0 ) {
        int sign = clr == WHITE ? 1 : -1;
        return sign * board->evaluateLeafPositionScore(clr);
    }

    for (Move* move : moves->getMoves())  {
        
        (*DEBUGGING_numNodesEvaluated)++;

        board->applyMove(move, clr);
        // since -min(-a, -b) = max(a, b)
        score = -1 * alphaBeta( -beta, -alpha, depthRemaining - 1, board, getOppositeColour(clr), DEBUGGING_numNodesEvaluated);
        board->undoMove();
      
        if ( score > bestScore ) {
         
            bestScore   = score;
            alpha       = std::max(alpha, score);
        }
        
        if ( score >= beta ) {
            delete moves;
            return bestScore;   //  fail soft beta-cutoff, existing the loop here is also fine  
        } 
    }
    
    delete moves;
    return bestScore;
}

// // TODO: make this function relative to the player calling it.
// int ABPlayer::evaluateBoardScore(Board* board) const {
    
//     int             score = 0;
//     const Piece*    piece;
//     int             sign;
    
//     for (int i = 0; i < NUM_SQUARES_ON_BOARD; i++) {
        
//         piece = board->getPieceAtPos(i);
        
//         if (piece == nullptr) {
//             continue;
//         }
        
//         sign = - 1 * (piece->getColour() * 2 - 1);
//         score += PIECE_VALUES[piece->getPieceType()] * sign;
        
//     }
    
//     return score;
// }

Player* ABPlayer::clone() const {
    return new ABPlayer(getColour(), maxSearchDepth);
}