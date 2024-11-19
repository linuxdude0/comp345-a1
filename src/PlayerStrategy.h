#ifndef PLAYER_STRATEGY
#define PLAYER_STRATEGY

#include "Mappings.h"
class PlayerStrategy
{
    // -- abstract player strategy class --
    public:
        virtual bool issueOrder(Player*, Order*) = 0;
        virtual std::vector<int> toAttack(Player*) = 0;
        virtual std::vector<int>& toDefend(Player*) = 0;
};

extern const char* player_strategy_strings[5];

enum class PlayerStrategyEnum {
    HUMAN_STRATEGY,
    NEUTRAL_STRATEGY,
    AGGRESSIVE_STRATEGY,
    BENEVOLENT_STRATEGY,
    CHEATING_STRATEGY,
    STRATEGIES_MAX,
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
        std::vector<int> toAttack(Player* p);
        std::vector<int>& toDefend(Player* p);
};

class AggressiveStrategy : public PlayerStrategy
{
    public:
        bool issueOrder(Player* p, Order* o); // just call with Order = null here
        std::vector<int> toAttack(Player* p);
        std::vector<int>& toDefend(Player* p);
        unsigned getMostDefendedEnemyTerr(Player* p);
        bool isEnemyTerr(unsigned, Player*);
        unsigned currStationedAt(unsigned);

};

class BenevolentStrategy : public PlayerStrategy
{
    public:
        bool issueOrder(Player* p, Order* order);
        std::vector<int> toAttack(Player* p);
        std::vector<int>& toDefend(Player* p);
        unsigned getOwnWeakestTerr(Player* p);
        void tryToNegotiate(Player* p);

};

class CheaterStrategy : public PlayerStrategy
{
    public:
        bool issueOrder(Player* p, Order* order);
        std::vector<int> toAttack(Player* p);
        std::vector<int>& toDefend(Player* p);
};
#endif // !PLAYER_STRATEGY