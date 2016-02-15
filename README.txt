1 Compiling and Running Platform
	OS: Fedora 18 Linux 3.6.10
	Compile Tool: g++ 4.7.2
	
2 How to compile?
	# cd trains
	# make clean && make
		
3 How to run the routine ?
	Input your command into "input" file, and then execute the process.
	# ./trains

4 Command description as following:
	graph			--  Initialize trains graph with edges
						sample: graph AB5 BC4 CD8 DC8 DE6 AD5 CE2 EB3 AE7
	get-distance	--  Get route's distance
						sample: get-distance ABC
	count-routes	--  Count the number of routes
						sample1: count-routes CC -maxstops 3
								Count the number of route from C to C and have maxmium 3 stops.
						sample2: count-routes AC -stops 4
								Count the number of route from A to C and have exactly 4 stops.
						sample3: count-routes CC -limited-distance 29
								Count the number of route from C to C and the distance is less then 29.
	get-shortest-distance	--  Get the shortest route distance
						sample: get-shortest-distance AC
								Get the shortest route distance from A to C
5 Source file description
	inout.cpp		--	extract command arguments from input file
	command.cpp 	--  execute command and dump command result
	graph.cpp		--  store trains graph and privodes some function to search routes on this graph