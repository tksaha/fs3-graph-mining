/*
 * File:   randomMining.h
 * Author: Tanay Kumar Saha
 *
 * Created on October 15, 2012, 7:33 PM
 */

#ifndef RANDOMMINING_H
#define	RANDOMMINING_H

#include <string>
#include<fstream>
#include "graph.h"
#include<iostream>
#include<vector>
#include<string>
#include<bitset>
#include "StringTokenizer.h"
#include "random.h"
#include <sys/time.h>
#include <unistd.h>
#include <boost/lexical_cast.hpp>
#include<algorithm>
using namespace std;
using namespace boost;

//one assumption on node label and edge label exists.

class randommining
{

private:
	std::vector<GraphClassPtr> myGraphSet;
	char *datafile;
	int sizeOfSubgrah,supportValue,temp[10],Qsize,fillerlength,graphIndex,gxpopcount,gypopcount,uniqgxpopcount,uniqgypopcount;
	bool tempbool[10],ifprint;
	double tempdouble[10],supportx;
	unsigned int noofIteration,noofGraphs;

/* Variables Common to all */
	GraphClassPtr rawGraph;
	MySupport mysupportList[NODELABEL][NODELABEL][EDGELABEL];
	myobjectpool *pool;

/* Variable related to random Graph Sampler */
	boost::container::flat_set<int>globalset;
	vector<vector<int> > gxpopulation;
	vector<vector<int> > gypopulation;
	vector<int>mysetTemp;
	string  LastIteration; 
	int gTot,nIter,totsup;
	string totid,_avgSupport;
	int LastLoop; size_t LastIter;
/* Variables related to  canonical code stuff */
	std::vector<canonicalcodeStruc*> code;
	std::vector<canonicalcodeStruc*>newlevelcode;
	string Lastestcancode;
	int i,j,k,x,y, i2,j2,i1,j1,status, _lij, rhs_lij;
	bool is_fwd,rhs_is_fwd;
	int iCodeStage,cindex;    
	   
public:
    	randommining();
    	~randommining();

/*Methods related to Parser */
   	string  makeFillerForString(string mystring);  
    	void printSupportList();
    	void printUsage(char *progname) {
		std::cout<<"Usage: "<<progname<<" -d data-file -i iteration -s sizeOfSubgrah -q (queueSize,0 by default)"<<endl;
    	}
    	void ProcessInputFile();
    	void parseArgs(int argc, char* argv[]);

/*Methods related to Random Graph Sampler */
    	double getSetIntersection( const graph *g);
	void gettwoVertexGraph();
   	void populateEdges();
    	void getRandomInitialSubgraph();
    	void populateNeighbor(const graph* g, int forwhom);
    	bool  getNeighborGraph(const graph* g, int forwhom); 
	void popNeiAndGetOne(const graph *g);
    	void insertIntoHeap();
    	void printHeap();
    	int randomMiner();
    	void doDFS();
/*Methods corresponding to Canonical Code*/
	int absmine(int a,int b);	
    	void freeCodex(canonicalcodeStruc* codex);
    	bool generateAnEdge(Vertex* endvertex, canonicalcodeStruc *it);
    	bool comparecodesAndInsert (canonicalcodeStruc *newcode);
    	void getfirstlabelCanonicalCode();
    	void printCanonicalCode() ;
    	void populateNextLevels();
    	void insert(bool how, Vertex* st, Vertex* end, int  edgelabel);
    	void  getcanonicalCode();

/*Methods related to DFS algorithm */

	bool isGraphConnected();
    	void DFSVISIT(Vertex* u);
};
#include "randomMining.inl"
		     
#endif	/* RANDOMMINING_H */

