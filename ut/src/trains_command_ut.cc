/*
 * Unit test file for trains command
 */

#include "command.h"
#include "gtest/gtest.h"

class TRAINS_COMMAND_UT : public ::testing::Test
{
public: 
    virtual void SetUp() 
    {
    }

    virtual void TearDown() 
    { 
    }

    virtual void TestBody(){}

public:
   // put in any custom data members that you need 
   EdgeList edgeList;
   Graph    graph;
};


TEST_F (TRAINS_COMMAND_UT, GraphInit)
{
    EdgeList edgeList;
    EdgeWithWeight ew;
    ew.edge.start = 'A';
    ew.edge.end   = 'B';
    ew.w          = 5;
    edgeList.push_back(ew);
    InitGraph(edgeList, graph);
    EXPECT_EQ(0, 0);
}


