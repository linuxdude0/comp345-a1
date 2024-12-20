#include "PlayerStrategy.h"
#include <set>
#include <limits>
#include <cmath>
#include <algorithm>
#include "common.h"

const char* player_strategy_strings[] = {
    "Human",
    "Neutral",
    "Aggressive",
    "Benevolent",
    "Cheater",
    "Draw",
};
 
// -- human strategy --

HumanStrategy::HumanStrategy() {
    this->player_strat = PlayerStrategyEnum::HUMAN_STRATEGY;
}

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


NeutralStrategy::NeutralStrategy() {
    this->player_strat = PlayerStrategyEnum::NEUTRAL_STRATEGY;
}


bool NeutralStrategy::issueOrder(Player* p, Order* order){ 
    UNUSED(order);
    /*nothing here*/
    // jus deploy? 

    p->orders->add(new DeployOrder(p,static_cast<unsigned>(p->toDefend()[0]),p->getReinforcementPool()));

    // ^^^ all troops deployed to the first available terr

    return true;
}

std::vector<int> NeutralStrategy::toAttack(Player* p){ 
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
    return can_attack; }
std::vector<int>& NeutralStrategy::toDefend(Player* p){return *(p->territoriesToDefend);}



// -- aggressive strategy --

AggressiveStrategy::AggressiveStrategy() {
    this->player_strat = PlayerStrategyEnum::AGGRESSIVE_STRATEGY;
}

bool AggressiveStrategy::issueOrder(Player* p, Order* o){
    UNUSED(o);
    // 1---- first we need to deploy everything

    // nothing to attack, deploy to territory belonging to self with highest number of troops
    if(p->toAttack().size() == 0){

        // initialize with the first available terr to defend and num of troops stationed there
        unsigned deployment_target = static_cast<unsigned>(p->toDefend()[0]);
        unsigned num_stationed;
        for(auto& entry : territory_owner_troops_mappings){

            if(std::get<0>(entry) == deployment_target){
                num_stationed = std::get<2>(entry);
            }
        }

        // loop and compare if there are territories to defend with more troops
        for(auto& terr : p->toDefend()){
            for(auto& entry : territory_owner_troops_mappings){

                if(std::get<0>(entry) == static_cast<unsigned>(terr)){

                    if(std::get<2>(entry) > num_stationed){

                        deployment_target = std::get<0>(entry);
                        num_stationed = std::get<2>(entry);
                    }
                }
            }
        }

        // time to deploy it all there
        p->orders->add(new DeployOrder(p, deployment_target, p->getReinforcementPool()));
    }
    else{

        // initialize the Attackable as the first in the list
        unsigned max_enemy_terr_index = p->toAttack()[0];
        unsigned num_stationed = 0; 
        for(auto& entry : territory_owner_troops_mappings){

            if(std::get<0>(entry) == max_enemy_terr_index){
                num_stationed = std::get<2>(entry);
            }
        }

        // find terr index where the maximum Attackable number of enemies is stationed, else it'll stay the same as initialized
        for(auto& index : p->toAttack()){
            for(auto& entry : territory_owner_troops_mappings){

                if(static_cast<unsigned>(index) == std::get<0>(entry)){

                    if(std::get<2>(entry) > num_stationed){

                        max_enemy_terr_index = index;
                        num_stationed = std::get<2>(entry);
                    }
                }
            }
        }

        unsigned deployment_target;

        // time to find adjacent belonging to current player
        Map::Territory toInvadeSoon = p->currMap->getTerritory(max_enemy_terr_index);
        for(unsigned i = 0; i < toInvadeSoon.num_adjacent_territories; i++){
            for(auto& terr : p->toDefend()){
                if(static_cast<int>(toInvadeSoon.adjacent_territories_indexes[i]) == terr){
                    deployment_target = static_cast<unsigned>(terr);
                }
            }
        }



        // DeployOrder::DeployOrder(Player* player, unsigned territory_target, unsigned num_troops)
        p->orders->add(new DeployOrder(p, deployment_target, p->getReinforcementPool())); // deploy all the troops available to that one terr
    }

    // -- 2) after deployment, try to bomb 
    if (!p->playCard(CardType::BOMB)) {
        std::cout << "[!] " << "BOMB order attempted and failed, no BOMB card for (Agressive) player [Player " << p->getName() << "]" << std::endl;          
    }
    else{

        unsigned enemy_target = getMostDefendedEnemyTerr(p);
        p->orders->add(new BombOrder(p,enemy_target));
    }

    //--3) for each of the owned territories, if adjacent is an enemy --> advance with all troops

    for(int i : p->toDefend()){

        // if any troops stationed there to begin with to be able to advance
        if(currStationedAt(static_cast<unsigned>(i)) > 0){

            // check if any adjacent doesnt belong to the current player        
            Map::Territory curr_terr = p->currMap->getTerritory(i);
            for(unsigned j = 0; j < curr_terr.num_adjacent_territories; j++){

                unsigned adjacent = curr_terr.adjacent_territories_indexes[j];
                if(isEnemyTerr(adjacent, p)){
                    p->orders->add(new AdvanceOrder(p, adjacent,static_cast<unsigned>(i), currStationedAt(static_cast<unsigned>(i)), p->currMap));
                    break;
                    // order to advance with everything is issued no need to check more, breaking the loop checking for other territories.
                }
            }
        }
    }
    return true;
}
std::vector<int> AggressiveStrategy::toAttack(Player* p){

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
std::vector<int>& AggressiveStrategy::toDefend(Player* p){
    return *(p->territoriesToDefend);
}
unsigned AggressiveStrategy::getMostDefendedEnemyTerr(Player* p){
    UNUSED(p);
    unsigned max = 0;
    unsigned enemy = 0;
    for(auto& entry : territory_owner_troops_mappings){

        if(std::get<2>(entry) > max){
            enemy = std::get<0>(entry);
        }
    }

    return enemy;
}
bool AggressiveStrategy::isEnemyTerr(unsigned terr_index, Player* p){
    for(auto& entry : territory_owner_troops_mappings){

        if(std::get<0>(entry) == terr_index && std::get<1>(entry) != p){
            return true;
        }
    }
    return false;
}
unsigned AggressiveStrategy::currStationedAt(unsigned terr_index){

    for(auto& entry : territory_owner_troops_mappings){

        if(std::get<0>(entry) == terr_index){
            return std::get<2>(entry);
        }
    }

    return 0;
}


// -- benevolent strategy --

BenevolentStrategy::BenevolentStrategy() {
    this->player_strat = PlayerStrategyEnum::BENEVOLENT_STRATEGY;
}


bool BenevolentStrategy::issueOrder(Player* p,Order* order) {

    UNUSED(order);
    // deploys only on its weakest territories
    // never attacks
    // uses only peaceful cards if any --> Diplomacy card afaik

    // deploy: 
    p->orders->add(new DeployOrder(p, getOwnWeakestTerr(p), p->getReinforcementPool()));

    // try to negotiate lmaooo:
    tryToNegotiate(p);

    return true;

}

unsigned BenevolentStrategy::getOwnWeakestTerr(Player* p){

    unsigned weakest = std::numeric_limits<unsigned>::max();
    unsigned troops_stationed = std::numeric_limits<unsigned>::max();
    for(int i : p->toDefend()){

        for(auto& entry : territory_owner_troops_mappings){

            if(std::get<0>(entry) == static_cast<unsigned>(i)){
                if(std::get<2>(entry) < troops_stationed){
                    weakest = std::get<0>(entry);
                    troops_stationed = std::get<2>(entry);
                    break;
                }
            }
        }
    }
    return weakest;
}

void BenevolentStrategy::tryToNegotiate(Player* p){



    // if owns a card: 
    Hand* h = p->getHand();
    for(Card* c : h->handCards){
        if(c->getType() == CardType::DIPLOMACY){

            // find a player to negotiate with in the global mapping
            for(auto& entry : territory_owner_troops_mappings){

                if(std::get<1>(entry) != p){

                    p->orders->add(new NegotiateOrder(p, std::get<1>(entry)));
                    break;
                }
            }
        }
    }
}

std::vector<int> BenevolentStrategy::toAttack(Player* p){
    
    
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
std::vector<int>& BenevolentStrategy::toDefend(Player* p){
    return *(p->territoriesToDefend);
}

// -- cheater strategy --

CheaterStrategy::CheaterStrategy() {
    this->player_strat = PlayerStrategyEnum::CHEATING_STRATEGY;
}


bool CheaterStrategy::issueOrder(Player* p,Order* order){
    UNUSED(order);
    // every turn conquers all adjacent to his
    
    //dump the deployments
    int assignment_per_terr = std::ceil(p->getReinforcementPool() / p->toDefend().size());

    // i decided to uniformly assign em
    for(auto& terr_index : p->toDefend()){
        p->orders->add(new DeployOrder(p,static_cast<unsigned>(terr_index),assignment_per_terr));
    }

    // time to cheat, conquer all adjacent every turn automatically
    for(auto& stealable_terr : p->toAttack()){
        for(auto& entry : territory_owner_troops_mappings){

            if(std::get<0>(entry) == static_cast<unsigned>(stealable_terr)){
            
            
                Player* victim = std::get<1>(entry);
                vector<int>& losersTerrs = victim->toDefend();
                losersTerrs.erase(std::remove(losersTerrs.begin(), losersTerrs.end(), std::get<0>(entry)), losersTerrs.end()); // remove terr from the victim's list
                std::get<1>(entry) = p; // new owner in the global mapping
                std::get<2>(entry) = 0; // set current troops stationed to 0
                p->toDefend().push_back(stealable_terr); // put it in the owned terrs
                p->setCardToIssueFlag(); // flag to issue a card at the end of this turn, given that a territory was captured
            
                std::cout << "[info] Hey, " << victim->getName() << " it seems that a cheater (who could it be?) stole your territory = " << stealable_terr << " such is life..." << std::endl;

            }

        }
    }

    return true;
}
std::vector<int> CheaterStrategy::toAttack(Player* p){
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
std::vector<int>& CheaterStrategy::toDefend(Player* p){
    return *(p->territoriesToDefend);
}

