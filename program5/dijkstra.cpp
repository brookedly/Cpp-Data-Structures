#include <string>
#include <iostream>
#include <fstream>
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "hash_graph.hpp"
#include "dijkstra.hpp"


std::string get_node_in_graph(const ics::DistGraph& g, std::string prompt, bool allow_QUIT) {
  std::string node;
  for(;;) {
    node = ics::prompt_string(prompt + " (must be in graph" + (allow_QUIT ? " or QUIT" : "") + ")");
    if ((allow_QUIT && node == "QUIT") || g.has_node(node))
      break;
  }
  return node;
}


int main() {
  try {
      std::ifstream input_file;
      ics::safe_open(input_file, "Enter graph file name", "flightcost.txt");
      ics::DistGraph graph;
      graph.load(input_file, ";");
      //Print out the corresponding airport/city with their LocalInfo.
      std::cout << graph << std::endl;
      std::string starting_node;
      std::string last_stop;
      starting_node = get_node_in_graph(graph, "Enter start node", false);
      ics::CostMap d_graph = extended_dijkstra(graph, starting_node);
      //Print out the corresponding airport/city with their LocalInfo when given a starting_node.
      std::cout << d_graph << std::endl;
      std::cout << std::endl;
      while (true)
      {
          last_stop = get_node_in_graph(graph, "Enter stop node", true);
          int total_fare = d_graph[last_stop].cost;
          if (last_stop == "QUIT")
          {
              break;
          }
          else {
              std::cout << "Cost is " << total_fare << "; path is " << recover_path(d_graph, last_stop) << std::endl;
              std::cout << std::endl;
          }
      }

  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
