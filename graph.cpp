#include "graph.h"
#include "macro.h"
#include "log.h"

#include <iterator>
#include <string>
#include <iostream>

using namespace std;

/*Description : Init trains graph
 *Input       : edgeList     -- dege list
 *Output      : graph        -- trains graph
 *Return      : 
 */
int InitGraph(EdgeList &edgeList, Graph &graph)
{
    EdgeList::iterator it = edgeList.begin();
    EdgeList::iterator end = edgeList.end();
    for (; it != end; it++)
    {
        Edge edge = it->edge;
        Node node = {edge.end - 'A', it->w};
        graph.neighbors[edge.start - 'A'].push_back(node);
    }
    graph.isInitialized = true;
    return RET_OK;
}

/*Description : check whether trains graph is initialized
 *Input       : graph        -- trains graph
 *Output      : 
 *Return      : 
 */
bool GraphIsInitialized(Graph &graph)
{
    return graph.isInitialized;
}

/*Description : find the edge weight in graph
 *Input       : graph        -- trains graph
 *            : start        -- start node
 *            : end          -- end node
 *Output      : 
 *Return      : the weight of this edge
 */
static int GRAPH_FindEdge(Graph &graph, int start, int end)
{
    list<Node>::iterator it = graph.neighbors[start].begin();
    for (; it != graph.neighbors[start].end(); it++)
    {
        if (end == it->v)
        {
            return it->w;
        }
    }

    return RET_NO_ROUTE;
}

/*Description : calculate this distance of one route
 *Input       : graph        -- trains graph
 *            : route        -- route
 *Output      : 
 *Return      : RET_OK/FAIL
 */
int GRAPH_CalcRouteDistance(Graph &graph, string& route)
{
    if (!GraphIsInitialized(graph))
    {
        return RET_GRAPH_NOT_INIT;
    }

    int distance = 0;
    int size = route.size();
    for (int i = 0; i + 1 < size; i++)
    {
        int weight = GRAPH_FindEdge(graph, route[i] - 'A', route[i+1] - 'A');
        if (weight != RET_NO_ROUTE)
        {
            distance += weight;
        }
        else
        {
            return RET_NO_ROUTE;
        }
    }

    return distance;
}

/*Description : find the shortest route for specifed start and end
 *Input       : graph        -- trains graph
 *            : path         -- current path
 *            : end          -- end node
 *            : len          -- the distance of path
 *Output      : bestPath     -- the shortest route
 *            : bestPathLen  -- the shortest distance
 *Return      : RET_OK/FAIL
 */
void GRAHP_FindShortestRoute(Graph& graph, string path, char end, 
    int len, string& bestPath, int& bestPathLen) 
{
    /* if we have found a better path, cancel search  */
    if ((path[path.size()-1] == end) && len < bestPathLen && len > 0) 
    {
        bestPath   = path;
        bestPathLen = len;
        return;
    }
    
    char lastChar = path[path.size()-1];
    int lastNodeIndex = lastChar - 'A';

    list<Node>::iterator it  = graph.neighbors[lastNodeIndex].begin();
    for (; it != graph.neighbors[lastNodeIndex].end(); it++) 
    {
        char newChar = (char)(it->v + 'A');
        int  newCost = it->w;
        unsigned int pos = path.find(newChar);
        /* new char has not been visited or new char is the end node */
        if (pos == path.npos || 0 == pos)
        {
            GRAHP_FindShortestRoute(graph, path.append(1, newChar), end, 
            len + newCost, bestPath, bestPathLen);
        }
    }
}

/*Description : count number of routes for maxminum stops
 *Input       : graph        -- trains graph
 *            : path         -- current path
 *            : end          -- end node
 *            : maxStops     -- the maxminum stops
 *Output      : routes       -- the routes
 *Return      : RET_OK/FAIL
 */
void GRAHP_CountRoutesMaxStops(Graph& graph, string path, 
        char end, int maxStops, list<string>& routes)
{
    /* if the path reach the maximum stops, then cancel search */
    if (path.size() - 1 > (unsigned int)maxStops)
    {
        return;
    }

    /* check if we have reach the end node */
    if (path.size() > 1 && path[path.size()-1] == end)
    {
        routes.push_back(path);
        //cout << path << endl;
    }

    /* get the last node in path, and go on searching */
    char lastNode      = path[path.size()-1];
    int  lastNodeIndex = lastNode - 'A';

    list<Node>::iterator it  = graph.neighbors[lastNodeIndex].begin();
    for (; it != graph.neighbors[lastNodeIndex].end(); it++) 
    {
        char newChar = (char)(it->v + 'A');
        GRAHP_CountRoutesMaxStops(graph, path + newChar, end, maxStops, routes);
    }
}

/*Description : count number of routes for specifed stops
 *Input       : graph        -- trains graph
 *            : path         -- current path
 *            : end          -- end node
 *            : stops        -- the stops
 *Output      : routes       -- the routes
 *Return      : RET_OK/FAIL
 */
void GRAHP_CountRoutesSpecifedStops(Graph& graph, string path,
        char end, int stops, list<string>& routes)
{
    /* if the path reach the stops, then cancel search */ 
    if (path.size() - 1 > (unsigned int)stops)
    {
        return;
    }

    /* check if we have reach the end node */ 
    if ((path.size() - 1 == (unsigned int)stops) && (path[path.size()-1] == end))
    {
        routes.push_back(path);
        //cout << path << endl;
    }

    /* get the last node in path, and go on searching */ 
    char lastNode      = path[path.size()-1];
    int  lastNodeIndex = lastNode - 'A';

    list<Node>::iterator it  = graph.neighbors[lastNodeIndex].begin();
    for (; it != graph.neighbors[lastNodeIndex].end(); it++) 
    {
        char newChar = (char)(it->v + 'A');
        GRAHP_CountRoutesSpecifedStops(graph, path + newChar, end, stops, routes);
    }
}

/*Description : count number of routes with limited distance
 *Input       : graph        -- trains graph
 *            : path         -- current path
 *            : end          -- end node
 *            : maxDistance  -- limited distance
 *            : curDistance  -- distance of path
 *Output      : routes       -- the routes
 *Return      : RET_OK/FAIL
 */
void GRAPH_CountRoutesInLimitedDistance(Graph& graph, string path, 
    char end, int maxDistance, int curDistance, list<string>& routes)
{
    /* if the path'distance exceeds the limit, then cancel search */ 
    if (curDistance >= maxDistance)
    {
        return;
    }

    /* check if we have reach the end node */ 
    if (curDistance > 0 && path[path.size()-1] == end)
    {
        routes.push_back(path);
        //cout << path << endl;
    }

    char lastNode     = path[path.size()-1];
    int lastNodeIndex = lastNode - 'A';

    list<Node>::iterator it  = graph.neighbors[lastNodeIndex].begin();
    for (; it != graph.neighbors[lastNodeIndex].end(); it++) 
    {
        char newChar = (char)(it->v + 'A');
        GRAPH_CountRoutesInLimitedDistance(graph, path + newChar, end, 
            maxDistance, curDistance + it->w, routes);
    }
}

