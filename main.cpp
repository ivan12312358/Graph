#include "graph.hpp"
#include "driver.hpp"
#include <fstream>

int main(int argc, char** argv)
{
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
    
    if (kgraph.IsBipartite())
        kgraph.Coloration();    
    else
        std::cout << "Error: not a bipartite graph" << std::endl;

    std::cout << kgraph.Distance(2, 4) << std::endl;

    return 0;
}