#ifndef _TRAINS_INOUT_H_
#define _TRAINS_INOUT_H_

#include "command.h"
#include "graph.h"

int ExtractCommand(const char* input, EdgeList& edgeList, std::list<Command*>& cmdList);

int Output(list<Command*>& cmdList);

#endif

