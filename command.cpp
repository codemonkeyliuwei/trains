/*
 * File Name        : command.cpp
 * File Description : Execute user command
 * Author           : liuxiaowei
 * Version          : 
 */

#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <sstream>
#include <limits.h>

#include "command.h"
#include "error.h"
#include "log.h"
#include "macro.h"

using namespace std;


/* string no route for print */
#define NO_SUCH_ROUTE "NO SUCH ROUTE"

typedef struct tagCommandMap 
{
    COMMAND_TYPE            cmd;
    CommandHandleFunc       handle;
    CommandResultDumpFunc   dump;
}CommandMap;

static vector<CommandMap> g_CmdMap(GRAPH_CMD_NR);

/*Description : register command handler for a command
 *Input       : cmd     -- command type
 *            : handle  -- command handler
 *            : dump    -- command result dump handler
 *Output      : 
 */
int RegisterCommand(COMMAND_TYPE cmd, CommandHandleFunc handle, 
                    CommandResultDumpFunc dump)
{
    if (cmd >= GRAPH_CMD_NR)
    {
        LogError("Invalid command:%d.", cmd);
        return RET_INVALID_PARAM;
    }

    CommandMap cmdMap = {cmd, handle, dump};
    g_CmdMap[cmd] = cmdMap;
    return RET_OK;
}

/*Description : create a new command and insert into command list
 *Input       : type    -- command type
 *            : arg     -- command arguments
 *Output      : cmdList -- command list
 */
int CreateNewCommand(COMMAND_TYPE type, void* arg, list<Command*>& cmdList)
{
    CHECK_ARG_WITH_RET(arg, RET_INVALID_PARAM);
    
    Command*   command = new Command;
    command->id   = cmdList.size();
    command->type = type;
    command->args = arg;    
    cmdList.push_back(command);

    return RET_OK;
}

/*Description : handler for command CMD_ROUTE_DISTANCE
 *Input       : graph   -- command type
 *            : cmd     -- command
 *Output      : 
 *Return      : RET_OK/FAIL
 */
int CalculateRouteDistance(Graph &graph, Command* cmd)
{
    CHECK_ARG_WITH_RET(cmd, RET_INVALID_PARAM);
    cmd->result.distance = GRAPH_CalcRouteDistance(graph, *(string*)(cmd->args));
    return RET_OK;
}

/*Description : handler for command CMD_COUNT_TRIPS
 *Input       : graph   -- command type
 *            : cmd     -- command
 *Output      : 
 *Return      : RET_OK/FAIL
 */
int CountTrips(Graph& graph, Command* cmd)
{
    CountTripsArg* arg = (CountTripsArg*)cmd->args;
    string path(1, arg->start);
    //list<string> routes;

    if (CMD_COUNT_TRIPS_MAX_STOPS == cmd->type && arg->maxStops > 0)
    {
        /* count trips with max stops */
        GRAHP_CountRoutesMaxStops(graph, path, arg->end, arg->maxStops, cmd->result.routes);
    }
    else if (CMD_COUNT_TRIPS_EXACT_STOPS == cmd->type && arg->stops > 0)
    {
        /* count trips with specifed stops */
        GRAHP_CountRoutesSpecifedStops(graph, path, arg->end, arg->stops, cmd->result.routes);
    }
    else if (CMD_COUNT_TRIPS_LIMITED_DISTANCE == cmd->type 
        && arg->limitDistance > 0)
    {
        /* count trips with limited distance */
        GRAPH_CountRoutesInLimitedDistance(graph, path, arg->end, 
            arg->limitDistance, 0, cmd->result.routes);
    }
    else
    {
        LogError("Invalid arguments for command:%d.", cmd->type);
        return RET_INVALID_PARAM;
    }
    
    return RET_OK;
}

/*Description : handler for command CMD_SHORTEST_DISTANCE
 *Input       : graph   -- command type
 *            : cmd     -- command
 *Output      : 
 *Return      : RET_OK/FAIL
 */
int FindShortestRoute(Graph &graph, Command* cmd)
{
    CountTripsArg* arg = (CountTripsArg*)cmd->args;
    string path(1, arg->start);
    string bestPath(path);
    int maxDistance = INT_MAX;
    int curDistance = 0;
    
    GRAHP_FindShortestRoute(graph, path, arg->end, curDistance, bestPath, maxDistance);

    cmd->result.distance = (bestPath.size() > 1) ? maxDistance : RET_NO_ROUTE;
    return RET_OK;
}

/*Description : handler for dumping result of command CMD_ROUTE_DISTANCE
 *Input       : cmd     -- command
 *Output      : 
 *Return      : result
 */
string DumpRouteDistance(Command* cmd)
{
    string output("Output #");
    stringstream ss;
    if (cmd->result.distance < 0)
    {
        ss << output << cmd->id + 1 << ": " << NO_SUCH_ROUTE;
    }
    else
    {
        ss << output << cmd->id + 1 << ": " << cmd->result.distance;
    }
    cout << ss.str() << endl;
    return ss.str();
}

/*Description : handler for dumping result of command CMD_COUNT_TRIPS
 *Input       : cmd     -- command
 *Output      : 
 *Return      : result
 */
string DumpTripsExactStops(Command* cmd)
{
    string output("Output #");
    stringstream ss;
    ss << output << cmd->id + 1 << ": " << cmd->result.routes.size();
    cout << ss.str() << endl;
    return ss.str();
}

string DumpTripsMaxStops(Command* cmd)
{
    string output("Output #");
    stringstream ss;
    ss << output << cmd->id + 1 << ": " << cmd->result.routes.size();
    cout << ss.str() << endl;
    return ss.str();
}

string DumpTripsMaxDistance(Command* cmd)
{
    string output("Output #");
    stringstream ss;
    ss << output << cmd->id + 1 << ": " << cmd->result.routes.size();
    cout << ss.str() << endl;
    return ss.str();
}

/*Description : handler for dumping result of command CMD_SHORTEST_DISTANCE
 *Input       : cmd     -- command
 *Output      : 
 *Return      : shortest route distance
 */
string DumpShortestRoute(Command* cmd)
{
    string output("Output #");
    stringstream ss;
    ss << output << cmd->id + 1 << ": " << cmd->result.distance;
    cout << ss.str() << endl;
    return ss.str();
}

/*Description : handler for dumping result of all commands
 *Input       : cmdList    -- command list
 *Output      : 
 *Return      : 
 */
void DumpCommanResult(list<Command*>& cmdList)
{    
    list<Command*>::iterator end = cmdList.end();    
    for (list<Command*>::iterator it  = cmdList.begin(); it != end; it++)
    {
        if (g_CmdMap[(*it)->type].dump)
        {
            g_CmdMap[(*it)->type].dump(*it);
        }
    }
}

/*Description : execute all commands in command list
 *Input       : graph       -- trains graph
 *            : cmdList     -- command list
 *Output      : 
 *Return      : 
 */
int ExecuteCommandList(Graph &graph, list<Command*> &commandList)
{
    int ret = RET_OK;
    list<Command*>::iterator it  = commandList.begin();
    list<Command*>::iterator end = commandList.end();
    for (; it != end; it++)
    {
        if (g_CmdMap[(*it)->type].handle)
        {
            ret = g_CmdMap[(*it)->type].handle(graph, *it);
            if (ret != RET_OK)
            {
                LogError("Execute command:%d fail return:%d", (*it)->type, ret);
            }
        }
    }

    //LogInfo("Execute all command successfully!");
    return RET_OK;
}

/*Description : destroy all commands in command list
 *Input       : cmdList     -- command list
 *Output      : 
 *Return      : 
 */
void DestroyCommandList(list<Command*> &commandList)
{
    list<Command*>::iterator it  = commandList.begin();
    list<Command*>::iterator end = commandList.end();
    for (; it != end; it++)
    {
        switch ((*it)->type)
        {
            case CMD_ROUTE_DISTANCE:
                delete (string*)(*it)->args;
                break;
            case CMD_COUNT_TRIPS_EXACT_STOPS:
            case CMD_COUNT_TRIPS_MAX_STOPS:
            case CMD_COUNT_TRIPS_LIMITED_DISTANCE:
            case CMD_SHORTEST_DISTANCE:
                delete (CountTripsArg*)(*it)->args;
                break;
            default:
                LogError("Invalid command type:%d.", (*it)->type);
        }
        delete (*it);
    }
}


