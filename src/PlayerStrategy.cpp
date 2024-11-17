#include "PlayerStrategy.h"


 
// -- human strategy --
bool HumanStrategy::issueOrder(Player* p, Order* order){
    
    switch (order->orderKind) {
        case OrderKind::DEPLOY:
        case OrderKind::ADVANCE:
            p->orders->add(order);
            break;
        case OrderKind::BOMB:
            if (!p->playCard(CardType::BOMB)) {
                std::cout << "[!] " << *order << "failed, no BOMB card for player [Player " << p->getName() << "]" << std::endl;
                return false;
            }
            p->orders->add(order);
            break;
        case OrderKind::AIRLIFT:
            if (!p->playCard(CardType::AIRLIFT)) {
                std::cout << "[!] " << *order << "failed, no AIRLIFT card for player [Player " << p->getName() << "]" << std::endl;
                return false;
            }
            p->orders->add(order);
            break;
        case OrderKind::BLOCKADE:
            if (!p->playCard(CardType::BLOCKADE)) {
                std::cout << "[!] " << *order << "failed, no BLOCKADE card for player [Player " << p->getName() << "]" << std::endl;
                return false;
            }
            p->orders->add(order);
            break;
        case OrderKind::NEGOTIATE:
            if (!p->playCard(CardType::DIPLOMACY)) {
                std::cout << "[!] " << *order << "failed, no NEGOTIATE card for player [Player " << p->getName() << "]" << std::endl;
                return false;
            }
            p->orders->add(order);
            break;
    }
    return true;
}
std::vector<int> HumanStrategy::toAttack(Player* p) {
    
    vector<int> empty{};
    if (p->territoriesToDefend->size() == 0){
        return empty;
    }
    std::set<unsigned> to_attack_territories;
    for (int terrID : *(p->territoriesToDefend)){
        
        Map::Territory ownedTerr = p->currMap->getTerritory(terrID);
        for (size_t i=0; i<ownedTerr.num_adjacent_territories; i++) {
            if (!p->ownsTerritory(ownedTerr.adjacent_territories_indexes[i])) {
                to_attack_territories.insert(ownedTerr.adjacent_territories_indexes[i]);
            }
        }
    }
    vector<int> can_attack(to_attack_territories.begin(), to_attack_territories.end());
    return can_attack;
}
std::vector<int>& HumanStrategy::toDefend(Player* p) {
    return *(p->territoriesToDefend);
}

// -- neutral strategy --

bool NeutralStrategy::issueOrder(Player* p, Order* order) {}
std::vector<int> NeutralStrategy::toAttack() {}
std::vector<int>& NeutralStrategy::toDefend() {}

// -- aggressive strategy --

bool AggressiveStrategy::issueOrder(Player* p, Order* order) {}
std::vector<int> AggressiveStrategy::toAttack() {}
std::vector<int>& AggressiveStrategy::toDefend() {}

// -- benevolent strategy --

bool BenevolentStrategy::issueOrder(Player* p,Order* order) {}
std::vector<int> BenevolentStrategy::toAttack() {}
std::vector<int>& BenevolentStrategy::toDefend() {}

// -- cheater strategy --

bool CheaterStrategy::issueOrder(Player* p,Order* order) {}
std::vector<int> CheaterStrategy::toAttack() {}
std::vector<int>& CheaterStrategy::toDefend() {}

