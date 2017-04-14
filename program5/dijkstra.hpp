#ifndef DIJKSTRA_HPP_
#define DIJKSTRA_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>                    //Biggest int: std::numeric_limits<int>::max()
#include "array_queue.hpp"
#include "array_stack.hpp"
#include "heap_priority_queue.hpp"
#include "hash_graph.hpp"

namespace ics {

class Info {
  public:
    Info() { }

    Info(std::string a_node) : node(a_node) { }

    bool operator==(const Info &rhs) const { return cost == rhs.cost && from == rhs.from; }

    bool operator!=(const Info &rhs) const { return !(*this == rhs); }

    friend std::ostream &operator<<(std::ostream &outs, const Info &i) {
      outs << "Info[" << i.node << "," << i.cost << "," << i.from << "]";
      return outs;
    }

    //Public instance variable definitions
    std::string node = "?";
    int cost = std::numeric_limits<int>::max();
    std::string from = "?";
  };

  bool gt_info(const Info &a, const Info &b) { return a.cost < b.cost; }

  typedef ics::HashGraph<int> DistGraph;
  typedef ics::HeapPriorityQueue<Info, gt_info> CostPQ;
  typedef ics::HashMap<std::string, Info, DistGraph::hash_str> CostMap;
  typedef ics::pair<std::string, Info> CostMapEntry;

//Return the final_map as specified in the lecture-node description of
//  extended Dijkstra algorithm
  CostMap extended_dijkstra(const DistGraph &g, std::string start_node) {
        CostMap final_map;
        CostMap info_map;
        std::string min_node;
        int min_cost;
        for (auto vertex_pair : g.all_nodes())
        {
            info_map[vertex_pair.first] = Info(vertex_pair.first);
        }
        info_map[start_node].cost = 0;
        CostPQ info_pq;
        for (auto stuff : info_map)
        {
            info_pq.enqueue(stuff.second);
        }
        //Loop so long as the info_map is not empty
        if (info_pq.empty() == false) {
            for (Info smallest_priced = info_pq.dequeue();;/* see body */) {   //ALWAYS REMOVING THE INFO WITH THE LOWEST COST IN THE PQ
                //If its cost is infinity, then no more nodes in info_map are reachable, so terminate
                if (smallest_priced.cost == std::numeric_limits<int>::max()){
                    break;
                }
                //If its node is already in answer_map, skip it and remove another
                if (final_map.has_key(smallest_priced.node) == false) {
                    //call "min_node" the node from this Info and "min_cost" its cost
                    //Remove this key->value from the info_map and put it into the answer_map
                    min_node = smallest_priced.node;
                    min_cost = smallest_priced.cost;
                    final_map[min_node] = smallest_priced;
                    info_map.erase(min_node);
                    const HashGraph<int>::NodeSet &destination = g.out_nodes(min_node);
                    //For every node d that is a destination from the min_node
                    for (auto d : destination) {
                        //not already in the answer_map
                        if (final_map.has_key(d) == false) {
//                            get d's Info using info_map and see if the cost is infinite or greater than the cost of the path from
//                            the start node to min_node, plus the cost of the edge from min_node to d.
//                            If it is infinte, or the sum is smaller,
//                            (1) In info_map, update the cost in Info to this smaller number,
//                            and update the predecessor of d to be min,
//                            (2) add the updated Info to the info_pq
                            int lowest_fare = min_cost + g.edge_value(min_node, d);
                            if (lowest_fare < info_map[d].cost or lowest_fare == std::numeric_limits<int>::max() ) {
                                info_map[d].cost = lowest_fare;
                                info_map[d].from = min_node;
                                info_pq.enqueue(info_map[d]);
                            }
                        }
                    }
                }
                smallest_priced = info_pq.dequeue(); //increment
            }
        }
        return final_map;
  }


//Return a queue whose front is the start node (implicit in answer_map) and whose
//  rear is the end node
  ArrayQueue <std::string> recover_path(const CostMap &answer_map, std::string end_node) {
        ArrayQueue<std::string> result;
        ArrayStack<std::string> answer;
        answer.push(end_node);
        std::string starting_node;
        for (auto thing : answer_map){
            if (thing.second.cost == 0)
            {
                starting_node = thing.second.node;
            }
        }
        while (answer.peek() != starting_node) {
            for (auto value : answer_map){
                if (value.first == end_node){
                    answer.push(value.second.from);
                    end_node = value.second.from;
                    if (end_node == starting_node)
                        break;
                }
            }
        }
        for (auto airport: answer){
            result.enqueue(airport);
        }
        return result;
  }


}

#endif /* DIJKSTRA_HPP_ */
