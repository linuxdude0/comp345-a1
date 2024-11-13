#ifndef PLAYER_STRATEGY
#define PLAYER_STRATEGY

#include "Player.h"

class PlayerStrategy
{
    // -- abstract player strategy class --
    public:
        virtual bool issueOrder(Order* order) = 0;
        virtual std::vector<int> toAttack() = 0;
        virtual std::vector<int>& toDefend() = 0;
};

// -- concrete player strategy classes --
class HumanStrategy : public PlayerStrategy
{
    public:
        bool issueOrder(Order* order);
        std::vector<int> toAttack();
        std::vector<int>& toDefend();
};

class NeutralStrategy : public PlayerStrategy
{
    public:
        bool issueOrder(Order* order);
        std::vector<int> toAttack();
        std::vector<int>& toDefend();
};

class AggressiveStrategy : public PlayerStrategy
{
    public:
        bool issueOrder(Order* order);
        std::vector<int> toAttack();
        std::vector<int>& toDefend();
};

class BenevolentStrategy : public PlayerStrategy
{
    public:
        bool issueOrder(Order* order);
        std::vector<int> toAttack();
        std::vector<int>& toDefend();
};

class CheaterStrategy : public PlayerStrategy
{
    public:
        bool issueOrder(Order* order);
        std::vector<int> toAttack();
        std::vector<int>& toDefend();
};
#endif // !PLAYER_STRATEGY