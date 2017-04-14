//// Submitter: brookedl(Ly, Brooke)
//// Partner  : jiayueg(Gao, Isley)
//// We certify that we worked cooperatively on this programming
////   assignment, according to the rules for pair programming
//
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include "ics46goody.hpp"
//#include "array_queue.hpp"
//#include "array_priority_queue.hpp"
//#include "array_set.hpp"
//#include "array_map.hpp"
//
//typedef ics::ArraySet<std::string>                     States;
//typedef ics::ArrayQueue<std::string>                   InputsQueue;
//typedef ics::ArrayMap<std::string,States>              InputStatesMap;
//
//typedef ics::ArrayMap<std::string,InputStatesMap>       NDFA;
//typedef ics::pair<std::string,InputStatesMap>           NDFAEntry;
//
//bool gt_NDFAEntry (const NDFAEntry& a, const NDFAEntry& b)
//{return a.first<b.first;}
//
//typedef ics::ArrayPriorityQueue<NDFAEntry,gt_NDFAEntry> NDFAPQ;
//
//typedef ics::pair<std::string,States>                   Transitions;
//typedef ics::ArrayQueue<Transitions>                    TransitionsQueue;
//
//
////Read an open file describing the non-deterministic finite automaton (each
////  line starts with a state name followed by pairs of transitions from that
////  state: (input followed by a new state, all separated by semicolons), and
////  return a Map whose keys are states and whose associated values are another
////  Map with each input in that state (keys) and the resulting set of states it
////  can lead to.
//const NDFA read_ndfa(std::ifstream &file) {
//    NDFA ndfa;
//    std::string line;
//    if (!file.is_open()) {
//        std::cout << "Sorry, but this file does not exist" << std::endl;
//    } else {
//        while (std::getline(file, line)) {
//            std::vector<std::string> whole_line = ics::split(line, ";");
//            std::string key = whole_line[0];
//            InputStatesMap i;
//            States s;
//            for (int x = 1; x < whole_line.size(); x+=2)
//            {
//                if (i.has_key(whole_line.at(x)) == false)
//                {
//                    s.insert(whole_line.at(x+1));
//                    i.put(whole_line.at(x), s);
//                }
//                else
//                {
//                    i[whole_line.at(x)].insert(whole_line.at(x+1));
//                }
//            }
//            ndfa[key] = i;
//        }
//        return ndfa;
//    }
//}
//
////Print a label and all the entries in the finite automaton Map, in
////  alphabetical order of the states: each line has a state, the text
////  "transitions:" and the Map of its transitions.
//void print_ndfa(const NDFA& ndfa) {
//    std::cout<< "The Non-Deterministic Finite Automaton's Description" << std::endl;
//    NDFAPQ sorted;
//    sorted.enqueue_all(ndfa);
//    for (auto kv : sorted)
//    {
//        std::cout << "  " << kv.first << " transitions: " << kv.second << std::endl;
//    }
//    std::cout<< std::endl;
//}
//
//
////Return a queue of the calculated transition pairs, based on the non-deterministic
////  finite automaton, initial state, and queue of inputs; each pair in the returned
////  queue is of the form: input, set of new states.
////The first pair contains "" as the input and the initial state.
////If any input i is illegal (does not lead to any state in the non-deterministic finite
////  automaton), ignore it.
//TransitionsQueue process(const NDFA& ndfa, std::string state, const InputsQueue& inputs) {
//    TransitionsQueue result;
//    States s{state};
//    result.enqueue(Transitions("", s));
//    for (auto x : inputs)
//    {
//        States possible_states;
//        for (auto state : s)
//        {
//            if (ndfa[state].has_key(x))
//            {
//                possible_states.insert_all(ndfa[state][x]);
//            }
//        }
//        result.enqueue(Transitions(x,possible_states));
//        if (possible_states.size() == 0)
//        {
//            return result;
//        }
//        s = possible_states;
//    }
//    return result;
//}
//
////Print a TransitionsQueue (the result of calling process) in a nice form.
////Print the Start state on the first line; then print each input and the
////  resulting new states indented on subsequent lines; on the last line, print
////  the Stop state.
//void interpret(TransitionsQueue& tq) {  //or TransitionsQueue or TransitionsQueue&&
//    //std::cout<< "TQ " << tq << std::endl;
//        for (auto kv : tq) {
//            if (kv.first == "")
//            {
//                std:: cout << "Start state = " << kv.second << std::endl;
//            }
//            else
//            {
//                std::cout << "  Input = " << kv.first << "; new states = " << kv.second <<std::endl;
//            }
//        }
//    ics::ArraySet<std::string> stop_set;
//    for (auto k : tq)
//    {
//        stop_set = k.second;
//    }
//    std::cout << "Stop state(s) = " << stop_set << std::endl;
//}
//
//
//
////Prompt the user for a file, create a finite automaton Map, and print it.
////Prompt the user for a file containing any number of simulation descriptions
////  for the finite automaton to process, one description per line; each
////  description contains a start state followed by its inputs, all separated by
////  semicolons.
////Repeatedly read a description, print that description, put each input in a
////  Queue, process the Queue and print the results in a nice form.
//int main() {
//  try {
//      std::ifstream input_file;
//      ics::safe_open(input_file, "Enter a non-deterministic finite automaton's file", "ndfaendin01.txt");
//      NDFA file = read_ndfa(input_file);
//      print_ndfa(file);
//      std::string each_line;
//      std::ifstream new_file;
//      ics::safe_open(new_file, "Enter a start-state and input file", "ndfainputendin01.txt");
//      while (std::getline(new_file, each_line))
//      {
//          std::vector<std::string> whole_line = ics::split(each_line, ";");
//          ics::ArrayQueue<std::string> inputq;
//          inputq.enqueue_all(whole_line);
//          std::string state = inputq.dequeue();
//          TransitionsQueue processme = process(file, state, inputq);
//          std::cout << std::endl << "Starting a new simulation with description: " << each_line << std::endl;
//          interpret(processme);
//      }
//
//  } catch (ics::IcsError& e) {
//    std::cout << e.what() << std::endl;
//  }
//
//  return 0;
//}
