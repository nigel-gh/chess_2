// Game.cpp

#include "Game.h"
#include "Players/Player.h"
#include "Board.h"
#include "Pieces/Piece.h"
#include <iostream>
#include <string>


Game::Game(Player* white, Player* black) {
    board                     = new Board();
    playerWhite               = white;
    playerBlack               = black;
    gameStatus.inProgress     = true;
    gameStatus.winningPlayer  = NO_COLOUR;
    gameStatus.gameOverReason = IN_PROGRESS;
    gameStatus.drawIsOffered  = false;
}

Game::~Game() {
    delete board;
    delete playerWhite;
    delete playerBlack;

    board       = nullptr;
    playerWhite = nullptr;
    playerBlack = nullptr;
}

void Game::play() {
    
    Player*     activePlayer = playerBlack;
    MoveSet*    legalMoves;
    Action      action;
    int         numMovesAvailable;
    
    while(gameStatus.inProgress) {
        
        activePlayer        = switchActivePlayer(activePlayer);
        
        legalMoves          = board->calcLegalMoves(activePlayer->getColour());
        
        numMovesAvailable   = legalMoves->size();
        
        gameStatus          = handleNoLegalMoves(numMovesAvailable, gameStatus, activePlayer->getColour());
        
        board->display(legalMoves, activePlayer->getColour());
        
        if (legalMoves->size() != 0) {
            
            action      = activePlayer->chooseAction(legalMoves, board);
            
            gameStatus  = handleAction(&action, activePlayer);
            
        }
        
    }
    
    printGameResults(legalMoves, activePlayer->getColour());

}

GameStatus Game::handleNoLegalMoves(int numMovesAvailable, GameStatus currentGameStatus, Colour currentColour) {

    if (numMovesAvailable) {
        return currentGameStatus;
    }

    constexpr bool  _inProgress         = false;
    GameOverReason  _gameOverReason;
    Colour          _winningColour;
    constexpr bool  _drawIsOffered      = false;
    constexpr bool  _playerActionValid  = true;

    if (board->kingInCheck(currentColour)) {
        _gameOverReason = CHECKMATE;
        _winningColour  = getOppositeColour(currentColour);
    } else {
        _gameOverReason = STALEMATE;
    }

    return {_inProgress, _gameOverReason, _winningColour, _drawIsOffered, _playerActionValid};

}

Player* Game::switchActivePlayer(const Player* player) {
    return player->getColour() == WHITE ? playerBlack : playerWhite;
}

GameStatus Game::handleAction(const Action* action, const Player* activePlayer) {
    
    ActionType actionType = action->getActionType();

    switch (actionType) {
        case MOVE:
        return board->applyMove(action->getMove(), activePlayer->getColour());
        break;
        
        case RESIGN:
        return {false, RESIGNATION, getOppositeColour(activePlayer->getColour()), false, true};
        break;
        
        case UNDO:
        board->undoMove();
        return {true, IN_PROGRESS, NO_COLOUR, false, true};
        break;
        
        default:
        throw std::invalid_argument("Game::handleAction() passed bad input.\n");
        break;
    }
        
}

void Game::printGameResults(const MoveSet* legalMoves, Colour clr) {

    if (gameStatus.inProgress) {
        throw std::invalid_argument("Game::printGameResults() was called but the game is still in progress!");
    }

    bool gameFinishedByWinning    = gameStatus.didGameFinishByWin();
    std::string gameOverReasonStr = gameStatus.gameOverReasonToString();
    
    std::cout << "\n";
    std::cout << " ------------ \n";
    std::cout << "| GAME OVER! |\n";
    std::cout << " ------------ \n\n";
    
    if (gameFinishedByWinning) {
        std::string winningPlayerName = gameStatus.getWinningPlayerName();
        std::cout << "::: " << winningPlayerName <<" won by " << gameOverReasonStr << " :::\n";
    } else {
        std::cout << "::: DRAW by " << gameOverReasonStr << " :::\n";
    }

    std::cout << "--- FINAL BOARD BELOW ---\n";

    board->display(legalMoves, clr);

    std::cout << "\n";

}

Game::Game(bool _isWhiteToMove, Board* _board, GameStatus _gameStatus, Player* _white, Player* _black) {

    isWhiteToMove           = _isWhiteToMove;
    board                   = _board;
    gameStatus              = _gameStatus;
    playerWhite             = _white;
    playerBlack             = _black;

}

Game Game::clone() const {

    return Game(isWhiteToMove, board->clone(), gameStatus, playerWhite->clone(), playerBlack->clone());

}