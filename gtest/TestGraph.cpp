#include <gtest/gtest.h>
#include "graph.hpp"

TEST(TestBipartite, graph1)
{
    graph::Graph kgraph {{1, 2}, {1, 3}, {2, 3}};
    EXPECT_EQ(kgraph.IsBipartite(), false);
}

TEST(TestBipartite, graph2)
{
    graph::Graph kgraph {{1, 2}, {1, 3}, {3, 2}};
    EXPECT_EQ(kgraph.IsBipartite(), false);
}

TEST(TestBipartite, graph3)
{
    graph::Graph kgraph {{1, 2}, {3, 1}, {2, 3}};
    EXPECT_EQ(kgraph.IsBipartite(), false);
}

TEST(TestBipartite, graph4)
{
    graph::Graph kgraph {{1, 2}, {3, 1}, {3, 2}};
    EXPECT_EQ(kgraph.IsBipartite(), false);
}

TEST(TestBipartite, graph5)
{
    graph::Graph kgraph {{2, 1}, {1, 3}, {2, 3}};
    EXPECT_EQ(kgraph.IsBipartite(), false);
}

TEST(TestBipartite, graph6)
{
    graph::Graph kgraph {{3, 1}, {1, 2}, {2, 3}};
    EXPECT_EQ(kgraph.IsBipartite(), false);
}

TEST(TestBipartite, graph7)
{
    graph::Graph kgraph {{3, 1}, {2, 1}, {3, 2}};
    EXPECT_EQ(kgraph.IsBipartite(), false);
}

TEST(TestBipartite, graph8)
{
    graph::Graph kgraph {{3, 1}, {3, 2}, {2, 1}};
    EXPECT_EQ(kgraph.IsBipartite(), false);
}

TEST(TestBipartite, graph9)
{
    graph::Graph kgraph {{1, 1}};
    EXPECT_EQ(kgraph.IsBipartite(), false);
}

TEST(TestBipartite, graph10)
{
    graph::Graph kgraph {{2, 1}};
    EXPECT_EQ(kgraph.IsBipartite(), true);
}

TEST(TestBipartite, graph11)
{
    graph::Graph kgraph {{1, 2}, {2, 3}, {3, 4}};
    EXPECT_EQ(kgraph.IsBipartite(), true);
}

TEST(TestBipartite, graph12)
{
    graph::Graph kgraph {{1, 2}, {2, 3}, {3, 4}, {4, 1}};
    EXPECT_EQ(kgraph.IsBipartite(), true);
}

TEST(TestBipartite, graph13)
{
    graph::Graph kgraph {{3, 4}, {1, 2}, {2, 3}, {4, 1}};
    EXPECT_EQ(kgraph.IsBipartite(), true);
}

TEST(TestBipartite, graph14)
{
    graph::Graph kgraph {{2, 3}, {2, 1}, {4, 3}, {4, 1}};
    EXPECT_EQ(kgraph.IsBipartite(), true);
}

TEST(TestBipartite, graph15)
{
    graph::Graph kgraph {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {4, 5}, {5, 6}, {5, 7}};
    EXPECT_EQ(kgraph.IsBipartite(), true);
}

TEST(TestBipartite, graph16)
{
    graph::Graph kgraph {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {2, 4}};
    EXPECT_EQ(kgraph.IsBipartite(), false);
}

TEST(TestBipartite, graph17)
{
    graph::Graph kgraph {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {3, 1}, {2, 4}};
    EXPECT_EQ(kgraph.IsBipartite(), false);
}

TEST(TestDistance, graph1)
{
    graph::Graph kgraph {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {3, 1}, {2, 4}};
    EXPECT_EQ(kgraph.Distance(1, 1), 0);
}

TEST(TestDistance, graph2)
{
    graph::Graph kgraph {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {3, 1}, {2, 4}};
    EXPECT_EQ(kgraph.Distance(1, 4), 1);
}

TEST(TestDistance, graph3)
{
    graph::Graph kgraph {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {4, 5}, {5, 7}};
    EXPECT_EQ(kgraph.Distance(1, 7), 3);
}

TEST(TestDistance, graph4)
{
    graph::Graph kgraph {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {4, 5}, {5, 7}};
    EXPECT_EQ(kgraph.Distance(1, 6), -1);
}

TEST(TestDistance, graph5)
{
    graph::Graph kgraph {{1, 2}, {2, 3}, {3, 1}};
    EXPECT_EQ(kgraph.Distance(1, 3), 1);
}

TEST(TestDistance, graph6)
{
    graph::Graph kgraph {{1, 2}, {2, 3}, {3, 1}};
    EXPECT_EQ(kgraph.Distance(1, 3), 1);
}

TEST(TestDistance, graph7)
{
    graph::Graph kgraph {{1, 2}, {2, 3}, {3, 1}};
    EXPECT_EQ(kgraph.Distance(1, 35), -1);
}