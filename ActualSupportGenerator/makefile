Research.out :  StringTokenizer.o Research.cpp  randomMining.h graph.o random.o 
	g++ -O3 -g -o Research.out graph.o StringTokenizer.o  random.o   Research.cpp randomMining.cpp dfs.cpp canonicalcode.cpp
graph.o	:   graph.h graph.cpp
	g++ -O3 -g -c graph.cpp

StringTokenizer.o : StringTokenizer.cpp StringTokenizer.h
	g++ -O3 -c -g StringTokenizer.cpp StringTokenizer.h
	
random.o: random.h  random.cpp
	g++ -O3 -g -c random.cpp



clean:
	rm -f *.o
	rm -f *.out
	rm -f *.gch
	rm -f *.dSYM

