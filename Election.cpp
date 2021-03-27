#include "Election.h"
#include <algorithm>
#include <unordered_map>
#include <cmath>


/*
Election constructor
*/
Election::Election(){
    this->candidate_id_counter_ = 0;
    this->candidate_parties_ = {};
    this->candidate_parties_[Party::None] = {};
    this->all_candidates_ = {};
}

/*
returns candidate id counter
*/
int Election::getCandidateIDCounter(){
    return this->candidate_id_counter_;
}

/*
returns map of party and corresponding candidates associated with that party
*/
std::map<Party, std::vector<Candidate>> Election::getCandidatePartyMap(){
    return this->candidate_parties_;
}

/*
Displays all districts, it is a helper method for the user interface implemented in main.
*/
void Election::displayAllDistricts(){
    std::map<int, District>::iterator it;
    std::map<int, District> district_map = this->electoral_map_.getDistrictMap();  
    for(it = district_map.begin(); it != district_map.end(); it++){
        std::cout << "~District ID: " << it->first << "~\n";
        std::cout << it->second;
    }
}

/*
increases id counter
*/
void Election::increaseIDCounter(){
    this->candidate_id_counter_ ++;
}

/*
registers candidates
*/
void Election::registerCandidate(std::string name, Party p){
    Candidate *c = new Candidate(this->candidate_id_counter_, name, p);
    this->candidate_parties_[p].push_back(*(c));
    this->candidate_id_counter_++;
    this->all_candidates_.push_back(*(c));    
}

/*
switches people (or not) according to district that candidate is campaigning in. Also outputs
text helpful to user interface about success of campaign.
*/
void Election::candidateCampaign(Candidate c, int district_id){
    //variables for equation
    Party candidate_party = c.getCandidateParty();
    District campaign_district = this->electoral_map_.getDistrict(district_id);
    int num_constituants_in_party = campaign_district.countAffiliates(candidate_party);
    int num_constituants_in_none = campaign_district.countAffiliates(Party::None);
    int num_constituants_other_parties = campaign_district.getTotalConstituants() - num_constituants_in_none - num_constituants_in_party;
    int area_of_district = campaign_district.getLandArea();

    //equation
    double compare = 100.0*double(((2.0*(double(num_constituants_in_party)+1.0))/(double(num_constituants_other_parties)))*((2.0*(double(num_constituants_in_party)+1.0))/double(area_of_district)));
    double P_success = std::min(100.0, compare);
    double P_extra_success = P_success*0.1;

    double random_chance = double(rand()%100+1);

    int scenario1_scenario3 = (random_chance>P_extra_success && random_chance<P_success);
    int scenario2 = (random_chance<P_extra_success);
    int scenario4 = (random_chance>P_success);

    std::cout << "\nChances to convert: " << P_success << std::endl;
    std::cout << "Chances to convert from another party: " << P_extra_success << std::endl; 

    Party majority_party = campaign_district.findMajorityParty(); 
    //1. consituant is converted from none to party of candidate
    if(scenario1_scenario3 && num_constituants_in_none!=0){
        campaign_district.changeParty(1, candidate_party, Party::None);
        std::cout << "Congrats, you have converted somebody from none to your party!" << std::endl;
    }

    //2. constituant converted from none to party of candidae
    //and consituant is converted from majority party to candidate's party
    else if(scenario2 && num_constituants_in_none>0){
        campaign_district.changeParty(1, candidate_party, Party::None);

        //checking that majority party is not equal to none
        if(campaign_district.countAffiliates(Party::None) != campaign_district.countAffiliates(majority_party)){
            campaign_district.changeParty(1, candidate_party, majority_party);
        }

        std::cout << "Congrats, you have converted somebody from majority and none to your party!" << std::endl;
        
    }
    //3. constituant converted from majority party to candidate party
    else if(scenario1_scenario3 && num_constituants_in_none==0){
        campaign_district.changeParty(1, candidate_party, majority_party);
        std::cout << "Congrats, you have converted somebody from majority to your party!" << std::endl;     
    }
    //4. No one is converted
    else{
        std::cout << "Campaign had no effect. " << std::endl;
        return;
    } 
}


/*
voting method for determining total votes for candidates in each district
*/
std::map<int, std::map<int,int>> Election::voting(){
    //seed random number generator used later
    srand(time(NULL));

    //this map maps District id to a nested map of candidate id as a key to number of votes as value
    std::map<int, std::map<int,int>> m;
    
    std::map<Party, std::vector<Candidate>> candidate_map = this->candidate_parties_;
    
    std::map<int, District>::iterator it;
    std::map<int, District> district_map = this->electoral_map_.getDistrictMap();  
    for(it = district_map.begin(); it != district_map.end(); it++){
        District current_district = it->second;
        int key = it->first; // district_id

        //loop through parties with candidate_map

        //initialize map with each candidate's vote set to 0.
        std::map<int, int> candidate_votes_per_district; //first is candidate id, second is num votes
        
        for(Candidate c: this->all_candidates_){
            int id = c.getCandidateID();
            candidate_votes_per_district[id] = 0;    
        }
        
        std::map<Party, std::vector<Candidate>>::iterator it2;
        for(it2 = candidate_map.begin(); it2 != candidate_map.end(); it2++){
            Party current_party = it2->first;
            std::vector<Candidate> candidate_vector = it2->second;

            //begin algorithm
            int num_candidates = candidate_vector.size();
            if(num_candidates == 0){
                //find majority party
                Party majority_party = current_district.findMajorityParty();

                //checking that majority party has candidates running in it
                std::vector<Candidate> candidates_in_majority = candidate_map[majority_party];
                int size_candidates_in_majority = candidates_in_majority.size();
                int vote_size = current_district.countAffiliates(current_party);

                if(size_candidates_in_majority == 0){
                    int random_candidate_max_num = this->all_candidates_.size();
                    int random_candidate_i = rand()%random_candidate_max_num;
                    Candidate c = this->all_candidates_[random_candidate_i];
                    int id = c.getCandidateID();
                    candidate_votes_per_district[id] += vote_size;
        
                }
                else if(size_candidates_in_majority == 1){
                    Candidate c = candidates_in_majority[0];
                    int id = c.getCandidateID();
                    candidate_votes_per_district[id] += vote_size;
                }
                else{
                    //pick random candidate in majority
                    int random_candidate = rand()%size_candidates_in_majority;
                    Candidate c = candidates_in_majority[random_candidate];
                    int id = c.getCandidateID();
                    candidate_votes_per_district[id] += vote_size;
                    
                }
                
            }
            else if(num_candidates == 1){
                Candidate c = candidate_vector[0];
                int id = c.getCandidateID();
                candidate_votes_per_district[id] += current_district.countAffiliates(current_party);
            }
            else{
                //multiple candidates are with party
                int random_candidate = rand()%num_candidates; //random from within party
                Candidate c = candidate_vector[random_candidate];
                int id = c.getCandidateID();
                candidate_votes_per_district[id] += current_district.countAffiliates(current_party);
            
            }
        }

        //add nested map to map, i.e add candidates and their votes to current district
        m[key] = candidate_votes_per_district;

    }
    return m;
}

//direct election: for each candidate, total votes across district and see who has won
Candidate Election::tallyVotes(std::map<int, std::map<int,int>> results){
    
    std::map<int, int> totals;

    //initialize each candidate's totals to 0
    for(Candidate c: this->all_candidates_){
        int id = c.getCandidateID();
        totals[id] = 0;    
    }

    //iterate through voting results map, cumulativly summing votes for candidates
    std::map<int, std::map<int,int>>::iterator it;
    for (it = results.begin(); it != results.end(); it++)
    {
        int current_district_id = it->first;
        std::map<int,int> candidates_votes = it->second;

        std::map<int,int>::iterator it2;
        for(it2 = candidates_votes.begin(); it2 != candidates_votes.end(); it2++){
            totals[it2->first] += it2->second;
        }
    }

    //records id of candidate who got highest vote
    int highest_count = -1;
    int highest_candidate = -1;
    std::map<int, int>::iterator it3;
    for(it3 = totals.begin(); it3!=totals.end(); it3++){
        if(it3->second > highest_count){
            highest_count = it3->second;
            highest_candidate = it3->first;
        }
    }

    //corresponds id to candidate object
    for(Candidate c: this->all_candidates_){
        int id = c.getCandidateID();
        if(highest_candidate == id){
            return c;
        }    
    }
       
}

/*
tally votes for representative election -- majority candidate wins all votes in a district. All votes
in district calculated according to specific formula. 
*/
Candidate RepresentativeElection::tallyVotes(std::map<int, std::map<int,int>> results){
    //map keeps track of candidate id and total number of votes as key and value respectivly
    std::map<int,int> m;

    //initializes candidate votes at 0
    for(Candidate c: this->all_candidates_){
        int id = c.getCandidateID();
        m[id] = 0;    
    }
    
    //determines variables for total number of constituatns in election and number of districts for formula
    int total_num_constituants = 0;
    int num_districts = 0;
    std::map<int, District> d = this->electoral_map_.getDistrictMap();
    std::map<int, District>::iterator it2;
    for(it2=d.begin(); it2 != d.end(); it2++){
        num_districts ++;
        District curr = it2->second;
        total_num_constituants += curr.getTotalConstituants();
    }

    //iterates through to add number of respective district votes for ultimate winner of a district (they get all district votes)
    std::map<int, std::map<int,int>>::iterator it; 
    for (it = results.begin(); it != results.end(); it++)
    {
        int current_district_id = it->first;
        std::map<int,int> candidates_votes = it->second;

        int highest_count = -1;
        int highest_candidate = -1;
        std::map<int,int>::iterator it2;
        for(it2 = candidates_votes.begin(); it2 != candidates_votes.end(); it2++){
            if(it2->second>highest_count){
                highest_count = it2->second;
                highest_candidate = it2->first;
            }
        }
        District current_district = this->electoral_map_.getDistrict(current_district_id);
        int num_district_constituants = current_district.getTotalConstituants();
        
        //formula
        int votes_district = std::floor(((num_district_constituants*1.0)/total_num_constituants)*(5*num_districts));
        
        m[highest_candidate] += votes_district;
    }

    //finds candidate with highest num total votes
    int highest_count_final = -1;
    int highest_candidate_final = -1;
    std::map<int, int>::iterator it3;
    for(it3 = m.begin(); it3!=m.end(); it3++){
        if(it3->second > highest_count_final){
            highest_count_final = it3->second;
            highest_candidate_final = it3->first;
        }
    }

    //corresponds candidate with their id
    for(Candidate c: this->all_candidates_){
        int id = c.getCandidateID();
        if(highest_candidate_final == id){
            return c;
        }    
    }
   
}