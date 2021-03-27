#ifndef CANDIDATE
#define CANDIDATE

#include "District.h" // for party enum class
#include <string>

/*
Candidate class keeps track of all individual candidate information like candidate ids, candidate name, and candidate party
it has the respective getter methods for each of these private variables
*/
class Candidate{
    int candidate_id_;
    std::string candidate_name_;
    Party candidate_party_;

    public:
        //constructors
        Candidate();
        Candidate(int id, std::string name, Party p);

        //getter methods
        int getCandidateID();
        std::string getCandidateName();
        Party getCandidateParty();

        //prints out candidate info
        friend std::ostream& operator<<(std::ostream& os, const Candidate& c){
            os << "---Candidate Info---\n" << "Name: " << c.candidate_name_ << "\nID: " << c.candidate_id_ << "\nParty: " << c.candidate_party_ << "\n";
            return os;
        }
    
        
};



#endif