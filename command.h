/*
 * File Name        : command.h
 * File Description : Define of command type, command request and response for responding type
 * Author           : liuxiaowei
 * Version          : 
 */
#ifndef _TRAINS_COMMAND_H_
#define _TRAINS_COMMAND_H_

#include <vector>
#include <string>
#include <list>

#include "graph.h"

using std::string;
using std::list;
using std::vector;

#define STR_CMD_GRAPH               "Graph"
#define STR_CMD_DISTANCE            "get-distance"
#define STR_CMD_TRIPS               "count-routes"
#define STR_CMD_SHORTEST_DISTANCE   "get-shortest-distance"

#define STR_ARG_MAX_STOPS           "-maxstops"
#define STR_ARG_STOPS               "-stops"
#define STR_ARG_LIMITED_DISTACNE    "-limited-distance"

enum COMMAND_TYPE{
    //CMD_GRAPH           = 0,  /* init train graph */
    CMD_ROUTE_DISTANCE    = 0,  /* compute distance of specified route */
    //CMD_COUNT_TRIPS,            /* compute the number of routes with maximum/exactly stops or limited distance */
    CMD_COUNT_TRIPS_EXACT_STOPS,      /* compute the number of routes from start to end with maximum stops */
    CMD_COUNT_TRIPS_MAX_STOPS,        /* compute the number of routes from start to end with exactly stops */
    CMD_COUNT_TRIPS_LIMITED_DISTANCE,   /* compute the number of different routes from start to end with a distance of less than specified distance */
    CMD_SHORTEST_DISTANCE,      /* compute the length of the shortest route from start to end */

    GRAPH_CMD_NR,
};

typedef struct tagCommandResult
{
    int          distance;  /* < 0 means no route to end */
    list<string> routes;
}CommandResult;

typedef struct tagCommand
{
    COMMAND_TYPE  type;
    unsigned int  id;        /* command id for print */
    void *        args;      /* command arguments */
    CommandResult result;    /* command result */
}Command;

typedef struct tagCountTripsArg
{
    char start;
    char end;
    int  stops;            /* CMD_COUNT_TRIPS_EXACT_STOPS */
    int  maxStops;         /* CMD_COUNT_TRIPS_MAX_STOPS */
    int  limitDistance;    /* CMD_COUNT_TRIPS_LIMITED_DISTANCE */
}CountTripsArg;

/* result of CMD_ROUTE_DISTANCE */
typedef struct tagCmdRouteDistanceResult
{
    int distance;
}CmdRouteDistanceResult;

/* result of CMD_COUNT_TRIPS_EXACT_STOPS,CMD_COUNT_TRIPS_MAX_STOPS,CMD_TRIPS_MAX_DISTANCE */
typedef struct tagCmdCountTripsResult
{
    std::vector<Route> routes;
}CmdCountTripsResult;

/* result of CMD_SHORTEST_DISTANCE */
typedef struct tagCmdShortestRouteResult
{
    //int   distance;
    Route route;
}CmdShortestRouteResult;

typedef int (*CommandHandleFunc)(Graph &graph, Command* cmd);

typedef string (*CommandResultDumpFunc)(Command* cmd);

/************************************************************************/
/* Command API Declaration                                              */
/************************************************************************/

/* Handler of each command type */
int CalculateRouteDistance(Graph &graph, Command* cmd);
int CountTrips(Graph &graph, Command* cmd);
int FindShortestRoute(Graph &graph, Command* cmd);

/* Result dump handler of each command type */
string DumpRouteDistance(Command* cmd);
string DumpTripsExactStops(Command* cmd);
string DumpTripsMaxStops(Command* cmd);
string DumpTripsMaxDistance(Command* cmd);
string DumpShortestRoute(Command* cmd);

int ExecuteCommandList(Graph &graph, std::list<Command*> &commandList);

void DumpCommanResult(list<Command*>& cmdList);

void DestroyCommandList(std::list<Command*> &commandList);

int RegisterCommand(COMMAND_TYPE cmd, CommandHandleFunc handle, 
                    CommandResultDumpFunc dump);

int CreateNewCommand(COMMAND_TYPE type, void* arg, list<Command*>& cmdList);

#endif

