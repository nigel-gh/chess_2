// Game.h

#ifndef GAME_H
#define GAME_H

#include "TypesAndEnums.h"
#include "Players/Player.h"
#include <iostream>
#include <string>

class Board;
class Player;

class Game {
private:
    bool            isWhiteToMove;
    Board*          board;
    GameStatus      gameStatus;
    Player*         playerWhite;
    Player*         playerBlack;
    GameStatus      handleAction(const Action*, const Player*);
    GameStatus      handleNoLegalMoves(int, GameStatus, Colour);
    void            printGameResults(const MoveSet*, Colour);
    Player*         switchActivePlayer(const Player*);
    Game(
        bool,
        Board*,
        GameStatus,
        Player*,
        Player*
    );
public:
    Game(Player*, Player*);
    ~Game();
    void play();
    Game clone() const;
};

#endif