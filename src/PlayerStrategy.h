#ifndef PLAYER_STRATEGY
#define PLAYER_STRATEGY
#include <set>
#include "Player.h"

class PlayerStrategy
{
    // -- abstract player strategy class --
    public:
        virtual bool issueOrder(Player* p, Order* order) = 0;
        virtual std::vector<int> toAttack(Player* p) = 0;
        virtual std::vector<int>& toDefend(Player* p) = 0;
};

// -- concrete player strategy classes --
class HumanStrategy : public PlayerStrategy
{
    public:
        bool issueOrder(Player* p, Order* order);
        std::vector<int> toAttack(Player* p);
        std::vector<int>& toDefend(Player* p);
};

class NeutralStrategy : public PlayerStrategy
{
    public:
        bool issueOrder(Player* p, Order* order);
        std::vector<int> toAttack();
        std::vector<int>& toDefend();
};

class AggressiveStrategy : public PlayerStrategy
{
    public:
        bool issueOrder(Player* p, Order* order);
        std::vector<int> toAttack();
        std::vector<int>& toDefend();
};

class BenevolentStrategy : public PlayerStrategy
{
    public:
        bool issueOrder(Player* p, Order* order);
        std::vector<int> toAttack();
        std::vector<int>& toDefend();
};

class CheaterStrategy : public PlayerStrategy
{
    public:
        bool issueOrder(Player* p, Order* order);
        std::vector<int> toAttack();
        std::vector<int>& toDefend();
};
#endif // !PLAYER_STRATEGY