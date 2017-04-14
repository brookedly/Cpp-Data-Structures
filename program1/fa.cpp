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
//typedef ics::ArrayQueue<std::string>                InputsQueue;
//typedef ics::ArrayMap<std::string,std::string>      InputStateMap;
//
//typedef ics::ArrayMap<std::string,InputStateMap>    FA;
//typedef ics::pair<std::string,InputStateMap>        FAEntry;
//
//bool gt_FAEntry (const FAEntry& a, const FAEntry& b)
//{return a.first<b.first;}
//
//typedef ics::ArrayPriorityQueue<FAEntry,gt_FAEntry> FAPQ;
//
//typedef ics::pair<std::string,std::string>          Transition;
//typedef ics::ArrayQueue<Transition>                 TransitionQueue;
//
//
////Read an open file describing the finite automaton (each line starts with
////  a state name followed by pairs of transitions from that state: (input
////  followed by new state, all separated by semicolons), and return a Map
////  whose keys are states and whose associated values are another Map with
////  each input in that state (keys) and the resulting state it leads to.
//const FA read_fa(std::ifstream &file) {
//    FA result;
//    std::string line;
//    if (!file.is_open()) {
//        std::cout << "Sorry, but this file does not exist" << std::endl;
//    } else {
//        while (std::getline(file, line)) {
//            std::vector<std::string> whole_line = ics::split(line, ";");
//            std::string key = whole_line[0];
//            for (int x = 1; x < whole_line.size(); x++) {
//                result[key][whole_line[x]] = whole_line[x += 1];
//            }
//        }
//        return result;
//    }
//}
//
//
////Print a label and all the entries in the finite automaton Map, in
////  alphabetical order of the states: each line has a state, the text
////  "transitions:" and the Map of its transitions.
//void print_fa(const FA& fa) {
//    //std::cout<< fa<< std::endl;
//    std::cout<< "The Finite Automaton's Description" << std::endl;
//    for (auto kv : fa)
//    {
//        std::cout << "  " << kv.first << " transitions: " << kv.second << std::endl;
//    }
//    std::cout<< std::endl;
//
//}
//
//
////Return a queue of the calculated transition pairs, based on the finite
////  automaton, initial state, and queue of inputs; each pair in the returned
////  queue is of the form: input, new state.
////The first pair contains "" as the input and the initial state.
////If any input i is illegal (does not lead to a state in the finite
////  automaton), then the last pair in the returned queue is i,"None".
//TransitionQueue process(const FA& fa, std::string state, const InputsQueue& inputs) {
//    TransitionQueue result;
//    result.enqueue(Transition("", state));
//    for (ics::ArrayQueue<std::string>::Iterator i = inputs.begin(); i != inputs.end(); ++i)
//    {
//        if (fa[state].has_key(*i))
//        {
//            state = fa[state][*i];
//            result.enqueue(Transition(*i,state));
//        }
//        else
//        {
//            result.enqueue(Transition(*i, "None"));
//            return result;
//        }
//    }
//    return result;
//}
//
//
////Print a TransitionQueue (the result of calling the process function above)
//// in a nice form.
////Print the Start state on the first line; then print each input and the
////  resulting new state (or "illegal input: terminated", if the state is
////  "None") indented on subsequent lines; on the last line, print the Stop
////  state (which may be "None").
//void interpret(TransitionQueue& tq) {  //or TransitionQueue or TransitionQueue&&
//    for (auto kv : tq) {
//
//        if (kv.first == "") {
//            std::cout << "Start state = " << kv.second << std::endl;
//        }
//        else if (kv.second == "None")
//        {
//            std::cout << "  Input = " << kv.first << "; illegal input: terminated" << std::endl;
//        }
//        else {
//            std::cout << "  Input = " << kv.first << "; new state = " << kv.second << std::endl;
//        }
//    }
//    std::string stop_state;
//    for (auto k : tq)
//    {
//        stop_state = k.second;
//    }
//    std::cout << "Stop State = " << stop_state << std::endl;
//
//}
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
//      ics::safe_open(input_file, "Enter a finite automaton's file", "faparity.txt");
//      FA file = read_fa(input_file);
//      print_fa(file);
//      std::string each_line;
//      std::ifstream new_file;
//      ics::safe_open(new_file, "Enter a start-state and input file", "fainputparity.txt");
//      while (std::getline(new_file, each_line))
//      {
//          std::vector<std::string> whole_line = ics::split(each_line, ";");
//          ics::ArrayQueue<std::string> inputq;
//          inputq.enqueue_all(whole_line);
//          std::string state = inputq.dequeue();
//          TransitionQueue processme = process(file, state, inputq);
//          std::cout << std::endl << "Starting a new simulation with description: " << each_line << std::endl;
//          interpret(processme);
//      }
//
//  } catch (ics::IcsError& e) {
//    std::cout << e.what() << std::endl;
//  }
//  return 0;
//}
