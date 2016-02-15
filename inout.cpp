/*
 * File Name        : inout.cpp
 * File Description : 1) Parse command from input
 *                    2) Write command result into output
 * Author           : liuxiaowei
 * Version          : 
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "graph.h"
#include "command.h"
#include "macro.h"
#include "errno.h"
#include "log.h"

using namespace std;

/* buf size for one command line */
#define BUF_SIZE        (4 * 1024)

/* Extract command arguments from file */
static int ExtractGraphCommand(const char* cmd, EdgeList& edgeList)
{
    const char* pos = cmd;
    CHECK_ARG_WITH_RET(cmd, RET_INVALID_PARAM);
    edgeList.clear();

    /* parse edges from command arguments */
    while (*pos != '\0' && *(pos + 1) != '\0' && *(pos + 2) != '\0')
    {
        if (isalpha(*pos) && isalpha(*(pos+1)))
        {
            EdgeWithWeight ew;
            ew.edge.start = toupper(*pos);
            ew.edge.end   = toupper(*(pos+1));
            ew.w          = atoi(pos+2);
            edgeList.push_back(ew);
            pos += 3;
        }
        else
        {
            pos++;
        }
    }

    return RET_OK;
}

static int ExtractCalcDistanceCmd(const char* cmd, list<Command*>& cmdList)
{
    CHECK_ARG_WITH_RET(cmd, RET_INVALID_PARAM);
    
    string* route    = new string;

    /* extract route from cmd */
    const char* pos = cmd;
    while (*pos != '\0')
    {
        if (isalpha(*pos))
        {
            route->append(1, *pos);
        }
        pos++;
    }
    
    return CreateNewCommand(CMD_ROUTE_DISTANCE, (void*)route, cmdList);
}

static int ExtractTripsArgs(const char* cmd, const char* strArg, int* arg)
{
    const char* pos = strstr(cmd, strArg);
    if (NULL == pos)
    {
        return RET_ERR;
    }
    
    pos += strlen(strArg);
    while (pos != NULL && *pos != '\0')
    {
        if (isblank(*pos))
        {
            pos++;
        }
        *arg = atoi(pos);
        return RET_OK;
    }

    LogError("Invalid command:%s cause argumets is null", cmd);
    return RET_INVALID_COMMAND;
}

static int ExtractCountTripsArgs(const char* cmd, CountTripsArg* trip, COMMAND_TYPE* cmdType)
{
    /* extract arg of counting routes with max stops */
    int ret = ExtractTripsArgs(cmd, STR_ARG_MAX_STOPS, &trip->maxStops);
    if (RET_OK == ret)
    {
        *cmdType = CMD_COUNT_TRIPS_MAX_STOPS;
        return RET_OK;
    }

    /* extract arg of counting routes with specified stops */
    ret = ExtractTripsArgs(cmd, STR_ARG_STOPS, &trip->stops);
    if (RET_OK == ret)
    {
        *cmdType = CMD_COUNT_TRIPS_EXACT_STOPS;
        return RET_OK;
    }

    /* extract arg of counting routes with specified stops */
    ret = ExtractTripsArgs(cmd, STR_ARG_LIMITED_DISTACNE, &trip->limitDistance); 
    if (RET_OK == ret)
    {
        *cmdType = CMD_COUNT_TRIPS_LIMITED_DISTANCE;
        return RET_OK;
    }

    LogError("Invalid arguments in cmd:%s", cmd);
    return RET_INVALID_COMMAND;
}

static int ExtractCountTripsCmd(const char* cmd, list<Command*>& cmdList)
{
    CHECK_ARG_WITH_RET(cmd, RET_INVALID_PARAM);

    CountTripsArg* arg = new CountTripsArg;

    /* parse route from cmd */
    const char* pos = cmd;
    bool isValidRoute = false;
    while (*pos != '\0' && *(pos + 1) != '\0')
    {
        if (isalpha(*pos) && isalpha(*(pos+1)))
        {
            arg->start = toupper(*pos);
            arg->end   = toupper(*(pos+1));
            isValidRoute = true;
            break;
        }
        pos++;
    }
    if (!isValidRoute)
    {
        delete arg;
        LogError("Invalid command:%s", cmd);
        return RET_INVALID_COMMAND;
    }

    /* extract cmd type and arg by argument string */
    COMMAND_TYPE type;
    int ret = ExtractCountTripsArgs(cmd, arg, &type);
    if (ret != RET_OK)
    {
        delete arg;
        LogError("Invalid command:%s", cmd);
        return RET_INVALID_COMMAND;
    }
    
    return CreateNewCommand(type, (void*)arg, cmdList);
}

static int ExtractFindShortestRouteCmd(const char* cmd, list<Command*>& cmdList)
{
    CHECK_ARG_WITH_RET(cmd, RET_INVALID_PARAM);

    CountTripsArg* arg = new CountTripsArg;

    /* parse route form cmd */
    const char* pos = cmd;  
    bool isValidRoute = false;
    while (*pos != '\0' && *(pos + 1) != '\0')
    {
        if (isalpha(*pos) && isalpha(*(pos+1)))
        {
            arg->start = toupper(*pos);
            arg->end   = toupper(*(pos+1));
            isValidRoute = true;
            break;
        }
        pos++;
    }

    if (!isValidRoute)
    {
        delete arg;
        LogError("Invalid command:%s because route is invalid.", cmd);
        return RET_INVALID_COMMAND;
    }

    return CreateNewCommand(CMD_SHORTEST_DISTANCE, (void*)arg, cmdList);
}

int ExtractCommand(const char* input, EdgeList& edgeList, list<Command*>& cmdList)
{
    CHECK_ARG_WITH_RET(input, RET_INVALID_PARAM);

    FILE* fp = fopen(input, "r");
    if (NULL == fp)
    {
        LogError("Open file:%s failed return:%d.", input, errno);
        return RET_OPEN_FILE_FAIL;
    }

    char cmd[BUF_SIZE+1];
    while (!feof(fp))
    {
        char* pos = NULL;
        memset(cmd, 0, BUF_SIZE+1);
        fgets(cmd, BUF_SIZE, fp);
        
        /* skip empty lines or comment lines */
        if(('\0' == *cmd) || ('\n' == *cmd) || ('#' == *cmd))
        {
            continue;
        }

        /* skip space or tab */
        pos = cmd;
        while (isblank(*pos))
        {
            pos++;
            continue;
        }

        /* extract arguments from each command */
        if (strstr(pos, STR_CMD_GRAPH) != NULL)
        {
            ExtractGraphCommand(pos + strlen(STR_CMD_GRAPH), edgeList);
        }
        else if (strstr(pos, STR_CMD_DISTANCE) != NULL)
        {
            ExtractCalcDistanceCmd(pos + strlen(STR_CMD_DISTANCE), cmdList);
        }
        else if (strstr(pos, STR_CMD_TRIPS) != NULL)
        {
            ExtractCountTripsCmd(pos + strlen(STR_CMD_TRIPS), cmdList);
        }
        else if (strstr(pos, STR_CMD_SHORTEST_DISTANCE) != NULL)
        {
            ExtractFindShortestRouteCmd(pos + strlen(STR_CMD_SHORTEST_DISTANCE), cmdList);
        }
        else
        {
            LogError("Not supported command:%s", pos);
        }
    }

    fclose(fp);
    return RET_OK;
}

