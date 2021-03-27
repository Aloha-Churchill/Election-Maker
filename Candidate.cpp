#include "Candidate.h"

/*
Default constructor for candidate.
*/
Candidate::Candidate(){
    this->candidate_id_ = -1;
    this->candidate_name_ = "null";
}

/*
Overloaded candidate constructor, assigns given id, name and party
*/
Candidate::Candidate(int id, std::string name, Party p){
    this->candidate_id_ = id;
    this->candidate_name_ = name;
    this->candidate_party_ = p;
}

/*
getter method for candidate id
*/
int Candidate::getCandidateID(){
    return this->candidate_id_;
}

/*
getter method for candidate name
*/
std::string Candidate::getCandidateName(){
    return this->candidate_name_;
}

/*
getter method for candidate party
*/
Party Candidate::getCandidateParty(){
    return this->candidate_party_;
}