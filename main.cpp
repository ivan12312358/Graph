#include "graph.hpp"
#include "driver.hpp"
#include <fstream>

#define DEBUG 1

int main(int argc, char** argv)
{
#ifndef DEBUG
    if (argc != 2)
    {
        std::cout << "Correct usage example: ./Graph <filename>" << std::endl;
        return 0;
    }

    std::fstream fEdges {argv[1], std::ios::in};

    if (!fEdges.is_open())
    {
        std::cout << "Couldn't open file." << std::endl;
        return 1;
    }

    std::streambuf* cin_buff = std::cin.rdbuf();
    std::cin.rdbuf (fEdges.rdbuf());

    yy::Driver driver;

    try { driver.parse(); }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::cin.rdbuf (cin_buff);
    graph::Graph kgraph(driver.begin(), driver.end());
#endif

    // graph::Graph kgraph {{1, 2}, {1, 3}, {2, 3}};
    // graph::Graph kgraph {{3, 2}, {3, 1}, {2, 1}};
    // graph::Graph kgraph {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {6, 7}, {6, 8}, {8, 9}};
    // graph::Graph kgraph {{1, 2}, {2, 3}};
    // graph::Graph kgraph {{4, 1}, {4, 3}, {3, 2}, {2, 1}};
    // graph::Graph kgraph {{1, 2}, {1, 3}, {3, 4}};
    graph::Graph kgraph {{1, 1}};

    kgraph.Dump();

    if (kgraph.IsBipartite() == 0)
        std::cout << "Error: not a bipartite graph" << std::endl;


    return 0;
}