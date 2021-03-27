#ifndef ELECTION
#define ELECTION

#include "Candidate.h"
#include "ElectoralMap.h"
#include <map>
#include <vector>
#include <string>

/*
Election class is responsible for registering candidates, having candidates campaign in districts,
voting, and tallying votes depending on which type of election (Direct or Rep). Below is the implementation
for a direct election.
*/
class Election{
    int candidate_id_counter_; //increments by one each time candidate is added
    std::map<Party, std::vector<Candidate>> candidate_parties_; 

    protected:
        ElectoralMap &electoral_map_ = ElectoralMap::getInstance();
        
    public:
        std::vector<Candidate> all_candidates_;
        Election(); // create new election, sets id counter to 0, 
        
        //getter methods
        int getCandidateIDCounter();
        std::map<Party, std::vector<Candidate>> getCandidatePartyMap();

        //other methods
        void displayAllDistricts();
        void increaseIDCounter();
        void registerCandidate(std::string name, Party p);
        void candidateCampaign(Candidate c, int district_id);
        std::map<int, std::map<int,int>> voting();
        virtual Candidate tallyVotes(std::map<int, std::map<int,int>> results); // tallies votes and declares winner


};

/*
Rep election tallies votes differently which is why there is a virtual keyword in election class. This method
overrides that class.
*/
class RepresentativeElection: public Election{
    public:
        Candidate tallyVotes(std::map<int, std::map<int,int>> results); // tallies votes and declares winner

};


#endif