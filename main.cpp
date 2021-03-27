/*
Name: Aloha Churchill
Description: This program runs two types of elections: Direct and Representative. 
The user can register candidates, have candidates campaign in different districts, and 
then have those districts vote according to a certain set of rules. Finally the votes are
tallied and a winner is declared depending on which type of election was originally chosen.

User Interface: the entire user interface is implemented in the main method with different helper methods.

How to run:
make all
./election

then just follow the prompts to run election
*/

#include "Election.h"
#include "ElectoralMap.h"
#include "Candidate.h"
#include <iostream>


/*
The campaign section of the user interface allows different candidates to campaign in different districts.
A candidate is selected based on their id and then is allowed to go to different districts to campaign
based on the district id selected. This also displays information about the success of their campaign in
a district with std::cout lines built into the candidateCampaign function in the election class.
*/
void campaign(Election* current_election){
    //display candidates and their parties
    int continue_campaign;
    std::cout << "Press 1 to start campaiging and -1 to stop: " << std::endl;
    std::cin >> continue_campaign;

    while(continue_campaign != -1){
        std::map<Party, std::vector<Candidate>> m = current_election->getCandidatePartyMap();
        std::map<Party, std::vector<Candidate>>::iterator it;
        for (it = m.begin(); it != m.end(); it++)
        {
            std::cout << "-----------------------------" << it->first << "-----------------------------" << std::endl;
            std::vector<Candidate> candidates = it->second;
            for(Candidate c: candidates){
                std::cout << c << std::endl;
            }
        }
        std::cout << "------------------ District Information ------------------------" << std::endl;
        //display all  districts
        current_election->displayAllDistricts();

        std::cout << "------------------ Choices ---------------------------" << std::endl;
        
        int candidate_to_campaign;
        std::cout << "Which candidate is campaigning (press -1 to finish): ";
        std::cin >> candidate_to_campaign;

        if(candidate_to_campaign != -1){

            Candidate curr_candidate;
            for(Candidate c: current_election->all_candidates_){
                int id = c.getCandidateID();
                if(candidate_to_campaign == id){
                    curr_candidate = c;
                }
            }

            int district_to_campaign;
            std::cout << "\nWhich district is this candidate campaigning in (enter ID): ";
            std::cin >> district_to_campaign;
            current_election->candidateCampaign(curr_candidate, district_to_campaign);
            
        }
        else{
            std::map<int, std::string> candidate_id_names;
            for(Candidate c: current_election->all_candidates_){
                int id = c.getCandidateID();
                candidate_id_names[id] = c.getCandidateName();
            }

            std::cout << "\nAnd the winner is...\n" << std::endl;
            
            //display voting results
            std::map<int, std::map<int,int>> v = current_election->voting();
            std::map<int, std::map<int,int>>::iterator it;

            for (it = v.begin(); it != v.end(); it++)
            {
                std::cout << "-----------------------DISTRICT ID: " << it->first << "-----------------------------" << std::endl;
                std::map<int,int> votes = it->second;

                std::map<int,int>::iterator it2;
                for(it2 = votes.begin(); it2 != votes.end(); it2++){
                    std::cout << "Candidate Name: " << candidate_id_names[it2->first] << std::endl;
                    std::cout << "Number of Votes: " << it2->second << std::endl;
                }
            }
            
            Candidate winner = current_election->tallyVotes(v); // tallies votes and declares winner


            //display winner
            std::cout << "\n********************\n";
            std::cout << "Congratulations to...\n";
            std::cout << winner;
            std::cout << "for winning the election!\n";
            return;
        }
    }
}

/*
This helper method displays registration information and helps users register different candidates.
It registers candidates for a certain party until they specify otherwise and then starts the campaign
once registration is finished.
*/
void registration(Election* current_election){
    //make sure to stop election if user ever types a zero
    //register candidates;
    std::vector<std::string> party_choice = {"PartyA", "PartyB"};
    std::vector<Party> curr_party = {Party::PartyA, Party::PartyB};
    int party_choice_index = 0;

    int registration_choice;
    while(party_choice_index <= party_choice.size()){   
        if(party_choice_index == party_choice.size()){
            registration_choice = 2;
        }
        else{ 
            std::cout << "Do you want to register a candidate for " << party_choice[party_choice_index] << " (1 = yes, 2 = no): ";
            std::cin >> registration_choice;
        }

        switch(registration_choice){
            case 1:{
                //register candidate
                std::string name;
                std::cout << "\nWhat is their name?: ";
                std::cin >> name;
                current_election->registerCandidate(name, curr_party[party_choice_index]);
                break;
            }
            case 2:{
                if(party_choice_index==party_choice.size()){
                    
                    campaign(current_election);
                    return;
                }
                else{
                    party_choice_index++;
                }
                break;
            }
            case 0:{
                break;
            }
            
        }
    }
}

/*
The main method is a continuous loop until the user specifies to stop. It asks the user to start
either a direct or representitive election and then begins that election by calling to register candidates.
*/
int main(){
    //user chooses election type
    int election_type;
    int confirm_continue;
    std::cout << "Would you like to have an election? Press 3 to continue and -1 to exit: ";
    std::cin >> confirm_continue;
    while(confirm_continue != -1){
        std::cout << "Direct (type 1) or Representative (type 2) Election? (type -1 to exit):  ";
        std::cin >> election_type;

        switch(election_type){
            case 1:
            {
                std::cout << "A direct election has begun!\n";
                Election *direct_election = new Election();
                registration(direct_election);
                break;
            }
            case 2:
            {
                std::cout << "A representative election has begun!\n";
                Election *representative_election = new RepresentativeElection();
                registration(representative_election);
                break;
            }
            case -1:
            {
                std::cout << "Exited\n";
                exit(0); 
            }  
        }
    }


}