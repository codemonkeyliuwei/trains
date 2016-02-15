/*
 * Unit test file for trains inout
 */

#include "inout.h"
#include "gtest/gtest.h"

class TRAINS_INOUT_UT : public ::testing::Test
{
public: 
    virtual void SetUp() 
    {
        input = "ut_input";
    }

    virtual void TearDown() 
    {
        DestroyCommandList(cmdList);
        edgeList.clear();
        system("rm ut_input");
    }

    virtual void TestBody(){}

   // put in any custom data members that you need 
   const char* input;
   
   EdgeList         edgeList;
   list<Command*>   cmdList;
};


TEST_F (TRAINS_INOUT_UT, ExtractGraphCommand)
{
    system("echo Graph: AB5, BC4, dE1 > ut_input");

    int ret = ExtractCommand(input, edgeList, cmdList);
    EXPECT_EQ(ret, 0);

    EdgeList::iterator it = edgeList.begin();
    EXPECT_EQ(it->edge.start, 'A');
    EXPECT_EQ(it->edge.end, 'B');
    EXPECT_EQ(it->w, 5);
    ++it;
    EXPECT_EQ(it->edge.start, 'B');
    EXPECT_EQ(it->edge.end, 'C');
    EXPECT_EQ(it->w, 4);
    ++it;
    EXPECT_EQ(it->edge.start, 'D');
    EXPECT_EQ(it->edge.end, 'E');
    EXPECT_EQ(it->w, 1);
    ++it;
    
    EXPECT_EQ(it, edgeList.end());
}

TEST_F (TRAINS_INOUT_UT, ExtractCalcDistanceCmd)
{
    system("echo get-distance ABC > ut_input");
    system("echo get-distance AD >> ut_input");

    int ret = ExtractCommand(input, edgeList, cmdList);
    EXPECT_EQ(ret, 0);

    list<Command*>::iterator it = cmdList.begin();
    EXPECT_EQ((*it)->type, CMD_ROUTE_DISTANCE);
    EXPECT_TRUE((*it)->id == 0);
    std::string* str = (string*)(*it)->args;
    EXPECT_STREQ(str->c_str(), "ABC");
    ++it;
    EXPECT_EQ((*it)->type, CMD_ROUTE_DISTANCE);
    EXPECT_TRUE((*it)->id == 1);
    str = (string*)(*it)->args;
    EXPECT_STREQ(str->c_str(), "AD");
    ++it;
    
    EXPECT_EQ(it, cmdList.end());
}

TEST_F (TRAINS_INOUT_UT, ExtractCountTripsCmd)
{
    system("echo count-routes CC -maxstops 3 > ut_input");
    system("echo count-routes AC -stops 4 >> ut_input");
    system("echo count-routes cd -limited-distance 29 >> ut_input");

    int ret = ExtractCommand(input, edgeList, cmdList);
    EXPECT_EQ(ret, 0);

    list<Command*>::iterator it = cmdList.begin();
    EXPECT_EQ((*it)->type, CMD_COUNT_TRIPS_MAX_STOPS);
    EXPECT_TRUE((*it)->id == 0);
    CountTripsArg* arg = (CountTripsArg*)(*it)->args;
    EXPECT_EQ(arg->start, 'C');
    EXPECT_EQ(arg->end, 'C');
    EXPECT_EQ(arg->maxStops, 3);
    
    ++it;
    EXPECT_EQ((*it)->type, CMD_COUNT_TRIPS_EXACT_STOPS);
    EXPECT_TRUE((*it)->id == 1);
    arg = (CountTripsArg*)(*it)->args;
    EXPECT_EQ(arg->start, 'A');
    EXPECT_EQ(arg->end, 'C');
    EXPECT_EQ(arg->stops, 4);
    
    ++it;
    EXPECT_EQ((*it)->type, CMD_COUNT_TRIPS_LIMITED_DISTANCE);
    EXPECT_TRUE((*it)->id == 2);
    arg = (CountTripsArg*)(*it)->args;
    EXPECT_EQ(arg->start, 'C');
    EXPECT_EQ(arg->end, 'D');
    EXPECT_EQ(arg->limitDistance, 29);
    
    ++it;
    EXPECT_EQ(it, cmdList.end());
}

TEST_F (TRAINS_INOUT_UT, ExtractFindShortestRouteCmd)
{
    system("echo get-shortest-distance AC > ut_input");
    system("echo get-shortest-distance df >> ut_input");

    int ret = ExtractCommand(input, edgeList, cmdList);
    EXPECT_EQ(ret, 0);

    list<Command*>::iterator it = cmdList.begin();
    EXPECT_EQ((*it)->type, CMD_SHORTEST_DISTANCE);
    EXPECT_TRUE((*it)->id == 0);
    CountTripsArg* arg = (CountTripsArg*)(*it)->args;
    EXPECT_EQ(arg->start, 'A');
    EXPECT_EQ(arg->end, 'C');
    
    ++it;
    EXPECT_EQ((*it)->type, CMD_SHORTEST_DISTANCE);
    EXPECT_TRUE((*it)->id == 1);
    arg = (CountTripsArg*)(*it)->args;
    EXPECT_EQ(arg->start, 'D');
    EXPECT_EQ(arg->end, 'F');
        
    ++it;
    EXPECT_EQ(it, cmdList.end());
}

