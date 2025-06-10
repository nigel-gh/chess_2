// Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "../TypesAndEnums.h"
#include <string>

class Move;
class MoveSet;
class Board;

enum ActionType {
    MOVE,
    RESIGN,
    UNDO,
    NO_ACTION
};

// wrapper around an existing move
class Action {
private:
    ActionType  actionType;
    const Move*       move;
public:
    Action();
    Action(ActionType, const Move*);
    const ActionType getActionType() const;
    const Move* getMove() const;
};

class Player {
private:
    Colour colour;
    std::string name;
public:
    Player();
    Player(Colour);
    Player(Colour, std::string);
    virtual ~Player();
    virtual const Action chooseAction(const MoveSet*, const Board*) const = 0;
    Colour getColour() const;
    std::string getPlayerName() const;
    virtual Player* clone() const = 0;
};

#endif