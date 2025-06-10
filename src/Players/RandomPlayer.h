// RandomPlayer.h

#ifndef RANDOM_PLAYER_H
#define RANDOM_PLAYER_H

class Player;
class Move;

class RandomPlayer : public Player {
public:
    RandomPlayer() = default;
    RandomPlayer(Colour);
    ~RandomPlayer() = default;
    const Action chooseAction(const MoveSet*, const Board*) const override;
    Player* clone() const override;
};

#endif