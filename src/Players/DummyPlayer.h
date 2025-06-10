// DummyPlayer.h

#ifndef DUMMY_PLAYER_H
#define DUMMY_PLAYER_H

class Player;
class Move;

class DummyPlayer : public Player {
private:
    static ActionType getActionFromString(std::string);
public:
    DummyPlayer() = default;
    DummyPlayer(Colour);
    ~DummyPlayer() = default;
    const Action chooseAction(const MoveSet*, const Board*) const override;
    Player* clone() const override;
};

#endif