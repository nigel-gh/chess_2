// BruteForcePlayer.h

// very dumb smart player

#ifndef BRUTE_FORCE_PLAYER_H
#define BRUTE_FORCE_PLAYER_H

class Player;
class Move;

constexpr int DEFAULT_DEPTH_OF_BRUTE_FORCE_PLAYER = 3;
struct MoveScore {
    const Move* move;
    int score;
};

class BruteForcePlayer : public Player {
private:
    int maxSearchDepth;
    int getMoveScoreRecursively(int, Board*, Colour, int*) const;
    // int evaluateBoardScore(Board*, Colour) const;
public:
    BruteForcePlayer();
    BruteForcePlayer(Colour);
    BruteForcePlayer(Colour, int);
    ~BruteForcePlayer();
    const Action chooseAction(const MoveSet*, const Board*) const override;
    Player* clone() const override;
};

#endif