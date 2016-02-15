/*
 * File Name        : graph.h
 * File Description : Define of train graph with adjacency table
 * Author           : liuxiaowei
 * Version          : 
 */
#ifndef _TRAINS_GRAPH_H_
#define _TRAINS_GRAPH_H_

#include <list>
#include <string>

#include "error.h"

using std::list;
using std::string;


#define MAX_VERTEX_NR   26

typedef struct tagEdge
{
    char start;
    char end;
}Edge;

typedef struct tagEdgeWithWeight
{
    Edge edge;
    int  w;
}EdgeWithWeight;

typedef struct tagNode
{
    int  v;     // transform char to int
    int  w;
}Node;

/* graph command arguments */
typedef std::list<EdgeWithWeight> EdgeList;

/* compute distance command arguments */
typedef std::string Route;

typedef struct tagGraph
{
    bool isInitialized;
    list<Node> neighbors[MAX_VERTEX_NR];
}Graph;


int InitGraph(EdgeList &edgeList, Graph &graph);

int GRAPH_CalcRouteDistance(Graph &graph, string& route);

void GRAHP_CountRoutesMaxStops(Graph& graph, string path, char end, 
        int maxStops, list<string>& routes);

void GRAHP_CountRoutesSpecifedStops(Graph& graph, string path, char end, 
    int stops, list<string>& routes);

void GRAPH_CountRoutesInLimitedDistance(Graph& graph, string path, 
    char end, int maxDistance, int curDistance, list<string>& routes);

void GRAHP_FindShortestRoute(Graph& graph, string path, char end, 
    int len, string& bestPath, int& bestPathLen);

#endif
