// Submitter: brookedl(Ly, Brooke)
// Partner  : jiayueg(Gao, Isley)
// We certify that we worked cooperatively on this programming
//   assignment, according to the rules for pair programming

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>                    //Biggest int: std::numeric_limits<int>::max()
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "array_priority_queue.hpp"
#include "array_set.hpp"
#include "array_map.hpp"


typedef ics::ArrayQueue<std::string>              CandidateQueue;
typedef ics::ArraySet<std::string>                CandidateSet;
typedef ics::ArrayMap<std::string,int>            CandidateTally;

typedef ics::ArrayMap<std::string,CandidateQueue> Preferences;
typedef ics::pair<std::string,CandidateQueue>     PreferencesEntry;

bool gt (const PreferencesEntry& a, const PreferencesEntry& b)
{return a.first<b.first;}

typedef ics::ArrayPriorityQueue<PreferencesEntry, gt> PreferencesEntryPQ; //Must supply gt at construction

typedef ics::pair<std::string,int>                TallyEntry;

bool get_min_number (const TallyEntry& a, const TallyEntry& b)
{return a.second<b.second;}

typedef ics::ArrayPriorityQueue<TallyEntry, get_min_number>       TallyEntryPQ;


//Read an open file stating voter preferences (each line is (a) a voter
//  followed by (b) all the candidates the voter would vote for, in
//  preference order (from most to least preferred candidate, separated
//  by semicolons), and return a Map of preferences: a Map whose keys are
//  voter names and whose values are a queue of candidate preferences.
Preferences read_voter_preferences(std::ifstream &file) {
    Preferences pref;
    std::string line;

    if (!file.is_open())
    {
        std::cout<< "Sorry, but this file does not exist" << std::endl;
    }
    else {
        while (std::getline(file, line))
        {
            std::vector<std::string> whole_line = ics::split(line, ";");
            int total = whole_line.size();
            std::string person_voting = whole_line[0];
            for (int x = 1; x < total; x++)
            {
                pref[person_voting].enqueue(whole_line.at(x));
            }
        }
    }
    return pref;
}


//Print a label and all the entries in the preferences Map, in alphabetical
//  order according to the voter.
//Use a "->" to separate the voter name from the Queue of candidates.
void print_voter_preferences(const Preferences& preferences) {
    std::cout<< "Voter -> queue[Preferences]" << std::endl;
    PreferencesEntryPQ sorted;
    sorted.enqueue_all(preferences);
    for (const PreferencesEntry& kv : sorted)
        std::cout << "  "<< kv.first << " -> " << kv.second << std::endl;
    std::cout << std::endl;
}


//Print the message followed by all the entries in the CandidateTally, in
//  the order specified by has_higher_priority: i is printed before j, if
//  has_higher_priority(i,j) returns true: sometimes alphabetically by candidate,
//  other times by decreasing votes for the candidate.
//Use a "->" to separate the candidat name from the number of votes they
//  received.
void print_tally(std::string message, const CandidateTally& tally, bool (*has_higher_priority)
        (const TallyEntry& i,const TallyEntry& j)) {
    TallyEntryPQ sorted(has_higher_priority);
    sorted.enqueue_all(tally);
    for (const TallyEntry& key_number : sorted)
        std::cout<< "  " << key_number.first << " -> " << key_number.second << std::endl;
    std::cout << std::endl;
}

//Return the CandidateTally: a Map of candidates (as keys) and the number of
//  votes they received, based on the unchanging Preferences (read from the
//  file) and the candidates who are currently still in the election (which changes).
//Every possible candidate should appear as a key in the resulting tally.
//Each voter should tally one vote: for their highest-ranked candidate who is
//  still in the the election.
CandidateTally evaluate_ballot(const Preferences& preferences, const CandidateSet& candidates) {
    CandidateTally result;
    for(auto x: candidates){
        result.put(x, 0);
    }
    for (auto kv : preferences)
    {
        for (auto inner : kv.second)
        {
            if (result.has_key(inner))
            {
                result[inner]++;
                break;
            }
        }
    }
    return result;
}


//Return the Set of candidates who are still in the election, based on the
//  tally of votes: compute the minimum number of votes and return a Set of
//  all candidates receiving more than that minimum; if all candidates
//  receive the same number of votes (that would be the minimum), the empty
//  Set is returned.
CandidateSet remaining_candidates(const CandidateTally& tally) {
    CandidateSet result;
    TallyEntryPQ sorted;
    sorted.enqueue_all(tally);
    int min_num;
    min_num = sorted.dequeue().second;
    //DEQUEUE returns and removes the recently add value from sorted (Basically the minimum value is at the top)
    //For instance if the CandidateTally is map[x->1 y->2 z->2] the PQ would be priority_queue[pair[x,
    for (auto candidate : tally)
        //since tally consist of <string, int> we will check the second value and compare it to the min_num
    {
        if(candidate.second > min_num)
        {result.insert(candidate.first);}
    }
    return result;
}


//Prompt the user for a file, create a voter preference Map, and print it.
//Determine the Set of all the candidates in the election, from this Map.
//Repeatedly evaluate the ballot based on the candidates (still) in the
//  election, printing the vote count (tally) two ways: with the candidates
//  (a) shown alphabetically increasing and (b) shown with the vote count
//  decreasing (candidates with equal vote counts are shown alphabetically
//  increasing); from this tally, compute which candidates remain in the
//  election: all candidates receiving more than the minimum number of votes;
//  continue this process until there are less than 2 candidates.
//Print the final result: there may 1 candidate left (the winner) or 0 left
//   (no winner).
int main() {
  try {
      std::ifstream input_file;
      ics::safe_open(input_file, "Enter graph file name", "flightcost.txt");
      std::cout << std::endl;
      Preferences pref = read_voter_preferences(input_file);
      print_voter_preferences(pref);
      std::string message;
      CandidateSet cset;
      int ballot_tally = 1;
      for (auto key_value : pref) //first loop that accesses key_value.first and key_value.second
      {
          for (auto votees : key_value.second ) //to get to the votees you need to loop inside the second
          {
              cset.insert(votees); //adds every candidate into the CandidateSet
          }
      }
      while (cset.size() > 1)
      {
          CandidateTally evaluated = evaluate_ballot(pref, cset);
          std::cout<< "Vote count on ballot #" << ballot_tally << " with candidates (alphabetically ordered); remaining candidates = " << cset << std::endl;
          print_tally(message, evaluated, get_min_number);
          std:: cout<< "Vote count on ballot #" << ballot_tally << " with candidates (numerically ordered); remaining candidates = " << cset << std::endl;
          print_tally(message, evaluated, get_min_number);
          cset = remaining_candidates(evaluated);
          ballot_tally += 1;
      }
      if (cset.size() ==  1)
      {
          for (auto c : cset) {
              std::cout << "Winner is " << c << std::endl;
          }
      }
      else
      {
          std::cout << "No winner; the election has come to a tie." << std::endl;
      }
  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
