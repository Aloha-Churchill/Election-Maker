#ifndef DISTRICT
#define DISTRICT

#include <map>
#include <iostream>

//enum class for Party types
enum class Party{None, PartyA, PartyB};

//operator overloader for enum class
inline std::ostream& operator<<(std::ostream& os, Party p)
{
    switch(p)
    {
        case Party::None: return os << "None" ;
        case Party::PartyA: return os << "PartyA" ;
        case Party::PartyB: return os << "PartyB" ;
    }
    return os << "Error: not a party type";
}

/*
District class details number of people in district, land area, and number of people associated with each party
*/
class District{

    std::map<Party, int> constituants_per_party_;
    int num_constituants_;
    int land_area_;

    public:
        //constructors
        District();
        District(int land_area);
        District(std::map<Party, int> set_parties, int land_area);
 
        //getter methods
        std::map<Party, int> getPartyTotals();
        int getTotalConstituants();
        int getLandArea();

        //setter methods
        void addConstituantsToParty(int num_people, Party p);

        //other methods
        int countAffiliates(Party p);
        void changeParty(int num_constituants, Party aquired, Party abandoned);
        Party findMajorityParty();

        //overloading << operator
        friend std::ostream& operator<<(std::ostream& os, const District& d){
            os << "square miles: " << d.land_area_ << "\nDistrict Population: " << d.num_constituants_ << "\n";
            
            os << "Constituants Per Party: {";
            std::map<Party, int> party_nums = d.constituants_per_party_;
            std::map<Party, int>::iterator it;
            for(it = party_nums.begin(); it != party_nums.end(); it++){
                os << it->first << ":" << it->second << ", ";
            }
            os << "}\n";

            return os;
        }

};



#endif