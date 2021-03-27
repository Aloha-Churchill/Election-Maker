#ifndef ELECTORAL_MAP
#define ELECTORAL_MAP

#include "District.h"
#include <map>

/*
The electoral map class is a singleton class that creates one instance of an electoral map which
keeps track of districts in the election. It also assigns unique identifiers to districts.
*/


//singleton class Electoral Map
class ElectoralMap{
    //private constructor
    ElectoralMap();

    //private variables
    std::map<int, District> district_map_;
    static const int num_districts_ = 4;

    public:
        //gets instantiated
        static ElectoralMap& getInstance(){
            static ElectoralMap instance;
            return instance;
        };

        //delete copy constructor
        ElectoralMap(ElectoralMap const&) = delete;
        void operator=(ElectoralMap const&) = delete;

        //get methods
        District getDistrict(int id);
        std::map<int, District> getDistrictMap();

        //operator overloader
        friend std::ostream& operator<<(std::ostream& os, ElectoralMap& em){
            for(int i=0; i<num_districts_; i++){
                os << "District #" << i << ": \n"; 
                os << em.getDistrict(i);
            }
            return os;
        }

};

#endif