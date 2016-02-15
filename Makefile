###########################################
#Makefile for trains
###########################################

UT_INC = -I. -I./ut/include
LINK = -L./ut/lib
LIB += -lgtest -lpthread
SRC = $(wildcard *.cpp) 
UT_SRC = $(shell find ut/src -name *.cc)

CC = g++
CC_FLAG = -g -Wall -O0

BIN = trains
UT_BIN = trains_ut
OBJ = $(patsubst %.cpp,%.o,$(SRC)) 
OBJ_UT = $(patsubst %.cc,%.o,$(UT_SRC))

all:$(BIN)

ut:$(UT_BIN)
	@./$(UT_BIN)

$(UT_BIN):$(OBJ_UT) graph.o inout.o command.o
	$(CC) $(LINK) -o $@ $^ $(LIB) 
	
$(BIN):$(OBJ)
	$(CC) $(INC) -o $@ $(OBJ)
	
memcheck:$(BIN)
	valgrind --leak-check=full --show-reachable=yes ./trains
	
.SUFFIXES: .c .o .cpp .cc

.cpp.o:
	$(CC) $(CC_FLAG) $(INC) -c $*.cpp -o $*.o
	
.cc.o:
	$(CC) $(CC_FLAG) $(UT_INC) -c $*.cc -o $*.o
	
test:
	@echo "TODO"

.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	rm -f $(OBJ) $(OBJ_UT) $(BIN) $(UT_BIN)
