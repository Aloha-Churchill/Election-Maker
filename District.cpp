#include "District.h"

/*
Default constructor for district
*/
District::District(){
    std::map<Party, int> parties;
    this->constituants_per_party_ = parties;
    this->land_area_ = 0;
    this->num_constituants_ = 0;
}

/*
overloaded constructor for district just based on land areas
*/
District::District(int land_area){
    std::map<Party, int> parties;
    this->constituants_per_party_ = parties;
    this->land_area_ = land_area;
    this->num_constituants_ = 0;
}

/*
overloaded constructor for district where user defines everything -- this was used mainly for testing purposes
*/
District::District(std::map<Party, int> set_parties, int land_area){
    this->constituants_per_party_ = set_parties;
    
    int sum = 0;
    std::map<Party, int>::iterator it;
    for(it = set_parties.begin(); it != set_parties.end(); it++){
        sum += it->second;
    }

    this->num_constituants_ = sum;
    this->land_area_ = land_area;

}

/*
getter method for total constituants in a given party
*/
std::map<Party, int> District::getPartyTotals(){
    return this->constituants_per_party_;
}

/*
getter method for total constituants
*/
int District::getTotalConstituants(){
    return this->num_constituants_;
}

/*
getter method for land area
*/
int District::getLandArea(){
    return this->land_area_;
}

/*
adds constituants to party
*/
void District::addConstituantsToParty(int num_people, Party p){
    this->constituants_per_party_[p] = num_people;
    this->num_constituants_ += num_people;
}

/*
counts affiliates in given party
*/
int District::countAffiliates(Party p){
    int sum = 0;
    std::map<Party, int>::iterator it;
    for(it = this->constituants_per_party_.begin(); it != constituants_per_party_.end(); it++){
        if(it->first == p){
            sum += it->second;
        }
    }    
    return sum;
}

/*
adds constituants to party -- used in the campaigning method when people switch parties
*/
void District::changeParty(int num_constituants, Party aquired, Party abandoned){
    this->constituants_per_party_[aquired] += num_constituants;
    this->constituants_per_party_[abandoned] -= num_constituants;
}

/*
Returns majority party that is not equal to party none.
*/
Party District::findMajorityParty(){


    //temporarily set consituants in Party::None to -1 so that it is automatically discluded from being majoirty
    int temp_subtract = this->countAffiliates(Party::None);
    this->addConstituantsToParty(-1*temp_subtract-1, Party::None);

    int highest_count = -1;
    Party highest_party = Party::None;
    
    std::map<Party, int>::iterator it;
    for(it = this->constituants_per_party_.begin(); it != constituants_per_party_.end(); it++){
        if(it->second >= highest_count){ //since >=, this will break tie by not returning Party::None
            highest_count = it->second;
            highest_party = it->first;
        }
    }

    //add back people to Party::None
    this->addConstituantsToParty(temp_subtract+1, Party::None);    
    return highest_party;    
}
