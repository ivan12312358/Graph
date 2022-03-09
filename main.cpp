#include "graph.hpp"

int main()
{
    graph::Graph<int, int> kgraph {{1,2},{1,3},{2,3},{2,4},{3,4}};

    kgraph.FillData(1, 123);
    kgraph.FillData(2, 71);
    kgraph.FillData(3, 216);
    kgraph.FillData(4, 584);

    kgraph.FillData({1, 4}, 584);
    kgraph.FillData({1, 2}, 212);
    kgraph.FillData({1, 3}, 754);
    kgraph.FillData({4, 2}, 1000);
    kgraph.FillData({5, 7}, 12000);


    kgraph.Dump();

    return 0;
}