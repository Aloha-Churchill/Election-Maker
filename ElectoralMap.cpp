#include "ElectoralMap.h"
#include <vector>

/*
Constructor. Creates certain number of districts and initializes district with random amount of land
between 5 and 29 and random amount of people in each party from 0 to 10. This also assigns
ids to all of the newly created districts.
*/
ElectoralMap::ElectoralMap() {
    int max_area = 29;
    int min_area = 5;

    int num_districts = this->num_districts_;
    srand(time(NULL));
    for(int i=0; i<num_districts; i++){
        std::map<Party, int> party_map;
        
        party_map[Party::None] = rand()%10;
        party_map[Party::PartyA] = rand()%10;
        party_map[Party::PartyB] = rand()%10;

        int area = rand()%(max_area-min_area+1)+min_area;
        District * d = new District(party_map, area);
        //this->district_map_[i] = District(area);
        this->district_map_[i] = *(d);
    }
    
}

/*
Returns district associated with certain district id number
*/
District ElectoralMap::getDistrict(int id){
    return this->district_map_[id];
}

/*
getter method for map
*/
std::map<int, District> ElectoralMap::getDistrictMap(){
    return this->district_map_;
}
