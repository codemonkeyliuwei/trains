/*
 * File Name        : trains.cpp
 * File Description : Main routine to run trains system
 * Author           : liuxiaowei
 * Version          : 
 */

#include "macro.h"
#include "error.h"
#include "inout.h"
#include "log.h"

#define INPUT_FILE      "input"
#define OUTPUT_FILE     "output"

int main(int argc, char* argv[])
{
    Graph               graph;
    EdgeList            edgeList;
    std::list<Command*> commandList;
    int                 ret = -1;

    /* register handler for each command */
    RegisterCommand(CMD_ROUTE_DISTANCE, CalculateRouteDistance, DumpRouteDistance);
    RegisterCommand(CMD_COUNT_TRIPS_EXACT_STOPS, CountTrips, DumpTripsExactStops);
    RegisterCommand(CMD_COUNT_TRIPS_MAX_STOPS, CountTrips, DumpTripsMaxStops);
    RegisterCommand(CMD_COUNT_TRIPS_LIMITED_DISTANCE, CountTrips, DumpTripsMaxDistance);
    RegisterCommand(CMD_SHORTEST_DISTANCE, FindShortestRoute, DumpShortestRoute);

    /* extract command from input */
    ret = ExtractCommand(INPUT_FILE, edgeList, commandList);
    if (ret != RET_OK)
    {
        LogError("Parse command from file:%s failed return:%d.", INPUT_FILE, ret);
        return ret;
    }

    /* initialize graph */
    ret = InitGraph(edgeList, graph);
    if (ret != RET_OK)
    {
        DestroyCommandList(commandList);
        LogError("Parse command from file:%s failed return:%d.", INPUT_FILE, ret);
        return ret;
    }

    /* execute command */
    ret = ExecuteCommandList(graph, commandList);
    if (ret != RET_OK)
    {
        DestroyCommandList(commandList);
        LogError("Execute command list failed return:%d.", ret);
        return ret;
    }

    /* print the command result */
    DumpCommanResult(commandList);

    /* release command and result */
    DestroyCommandList(commandList);

    return RET_OK;
}
