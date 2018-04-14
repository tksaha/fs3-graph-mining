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
#include "random.h"
#include<iostream>
#include<vector>
#include<string>
#include "StringTokenizer.h"
#include <sys/time.h>
#include <unistd.h>
#include <boost/lexical_cast.hpp>
#include<algorithm>

using namespace std;
using namespace boost;



class randomMining
{
private:
    std::vector<GraphClassPtr> myGraphSet;
    char *datafile;
    char *parsefile;
    int sizeOfSubgrah;
    int supportValue;
    int temp[10];
    bool tempbool[10];
    double tempdouble[10];
    int Qsize;
    unsigned int noofIteration;
    unsigned int noofGraphs;
    bool ifprint;//for debugging purpose only
    int fillerlength;//print filler length
    GraphClassPtr rawGraph;
    MySupport mysupportList[NODELABEL][NODELABEL][EDGELABEL];
    myobjectpool *pool;
    double supportx;
    int graphIndex;
    boost::container::flat_set<int>globalset;
    int gxpopcount,gypopcount;
    int uniqgxpopcount;
    int uniqgypopcount;
    vector<vector<int> > gxpopulation;
    vector<vector<int> > gypopulation;
    vector<int>mysetTemp;
    std::vector<canonicalcodeStruc*> code;
    std::vector<canonicalcodeStruc*>newlevelcode;
    string Lastestcancode;
    std::set<heapData*,compareheapData> MinedGraph;
    std::vector<heapData*> MyQ; 
    size_t LastIteration;
    int ifheaped, isFirstTime;
    int threshold;
    set<int>myinitset;
    int i,j,x,y, i2,j2,i1,j1,status, _lij, rhs_lij;
    bool is_fwd,rhs_is_fwd;
public:
    randomMining()
    {
      sizeOfSubgrah=0;
      fillerlength=15;
      noofGraphs=1;
      ifprint=true;
      noofIteration=0;
      Qsize=0;
      for(int i=0;i<NODELABEL;i++)
	{
		for(int j=0;j<NODELABEL;j++)
		{

			for(int k=0;k<EDGELABEL;k++)
			{
				MySupport mysup;
				mysup.lastIndex=-1;
				mysup.supportCount=0;
				mysupportList[i][j][k]=mysup;
			}
		}
	}
	for(int i=0;i<10;i++)
	{
		temp[i]=0; 
		tempbool[i]=false;
		tempdouble[i]=0.0;
	}
	pool=new myobjectpool();
	myGraphSet.clear();
	globalset.clear();
	MinedGraph.clear();
	MyQ.clear();
	ifheaped=false;
	isFirstTime=false;
	threshold=1;
	myinitset.clear();
    }
    ~randomMining()
    {
	myGraphSet.clear();
	globalset.clear();

    }
    string  makeFillerForString(string mystring);  
    void printSupportList();
   
    void printUsage(char *progname) {
    	 std::cout<<"Usage: "<<progname<<" -d data-file -p patternFile -sup supportvalue(int)"<<endl;
    }
    void ProcessInputFile();
    void parseArgs(int argc, char* argv[]);
    double getAVgSupport(  const graph *g)
    {
                supportx=0.0;
		temp[0]=0;temp[1]=0;temp[2]=0;temp[3]=0;
                //time_tracker ttme;
                //ttme.start();
                for(ConstEdgeSetIterator it=g->myedges.begin();it!=g->myedges.end();++it)
                {
			   temp[0]=(*it)->st->globallabel;
			   temp[1]=(*it)->end->globallabel;
			   temp[3]=(*it)->edgelabel;
			   if(temp[0]>temp[1]) { 
					temp[2]=temp[0]; temp[0]=temp[1];temp[1]=temp[2];
			   }
			   
		   	   temp[2]=mysupportList[temp[0]][temp[1]][temp[3]].supportCount;
			   if(temp[2]==0) cout<<"Error in avg. support"<<temp[0]<<" "<<temp[1]<<" "<<temp[3]<<endl;
			   else supportx=supportx+temp[2];
                }
               // ttme.stop();
                return supportx/(g->myedges.size());
    }
    void gettwoVertexGraph()
    {
		
		temp[0]=1; temp[1]=myGraphSet[graphIndex-1]->myedges.size();
		int rnd=boost_get_a_random_number(temp[0], temp[1]+1);
		ChangeableEdgeSetIterator  itx=myGraphSet[graphIndex-1]->myedges.begin()+rnd-1;
		rawGraph->addVertex((*itx)->st) ;
		rawGraph->addVertex((*itx)->end);
		
   }
   void populateEdges()
    {
	    for(ConstVertexIterator itx=rawGraph->myverts.begin();itx!=rawGraph->myverts.end();++itx)
            {
                for(ConstVertexIterator ity=itx+1; ity!=rawGraph->myverts.end();++ity)
		{
			Edge *E=pool->getEResource();
			if((*itx)->label > (*ity)->label) {E->st=(*itx) ; E->end=(*ity);}
			else {E->st=(*ity) ; E->end=(*itx); }
			ConstEdgeSetIterator itE=myGraphSet[graphIndex-1]->myedges.find(E);	
			if( itE!=myGraphSet[graphIndex-1]->myedges.end())
			{
				rawGraph->addEdge((*itE));
			}
			pool->returnEResource(E);
		}
                 
            }
    }
    void getRandomInitialSubgraph()
    {
		if(sizeOfSubgrah<2) { cout<<"Size must be 2"<<endl;}
		else {	
			rawGraph =pool->getGResource();
                	gettwoVertexGraph();
			tempbool[0]=false;
                        
			while(rawGraph->myverts.size()!=sizeOfSubgrah)
			{
				temp[0]=rawGraph->myverts.size();
				for(ConstVertexIterator itx=rawGraph->myverts.begin();itx!=rawGraph->myverts.end();++itx)
				{
					for(ChangeableVertexIterator  itz=(*itx)->list.begin();itz!=(*itx)->list.end();++itz)
					{
						if(rawGraph->vertexmap.find((*itz)->label)==rawGraph->vertexmap.end()) {
								rawGraph->addVertex((*itz));
								itx=rawGraph->myverts.begin();
								itz=(*itx)->list.begin();
						}

						if(rawGraph->myverts.size()==sizeOfSubgrah) {
							tempbool[0]=true;
							break;
						}
					}
					if(tempbool[0]==true) break;
				}
				if(tempbool[0]==true) break;
				if(rawGraph->myverts.size()==temp[0]){
					 rawGraph->reset();
					 gettwoVertexGraph();
				}
                        }
			populateEdges(); 
		 }
		
		
    }
    void populateNeighbor(const graph* g, int forwhom);
    bool  getNeighborGraph(const graph* g, int forwhom); 
//canonical code stuff
    void freeCodex(canonicalcodeStruc* codex);
    bool generateAnEdge(Vertex* endvertex, canonicalcodeStruc *it);
    bool comparecodesAndInsert (canonicalcodeStruc *newcode);
    void getfirstlabelCanonicalCode();
    void printCanonicalCode() ;
    void populateNextLevels();
    void insert(bool how, Vertex* st, Vertex* end, int  edgelabel);
    void  getcanonicalCode(){
		newlevelcode.clear();
		code.clear();
                getfirstlabelCanonicalCode();
		if(rawGraph->myedges.size()>1) populateNextLevels();
		printCanonicalCode();
    }

    int absmine(int a,int b)
    {
	if((a-b)<0) return -(a-b);
	else return (a-b);
    } 

    void popNeiAndGetOne(graph *g)
    {
                populateNeighbor(g, 0);
		int dgx=uniqgxpopcount;//dgx
		tempdouble[1]=getAVgSupport(g);//zSupx
		while(true){
		     rawGraph =pool->getGResource();
		     //cout<<"printing graph to mod"<<endl;
		     //g->printAdjacencyList();
		     while(!getNeighborGraph(g, 0))
                     {
			       pool->returnGResource(rawGraph);
			       rawGraph=pool->getGResource();
                     }
		     //cout<<"---------------------------------"<<endl;
                     populateNeighbor(rawGraph, 1);
                     int dgy=uniqgypopcount; //dgy
                     tempdouble[2]=getAVgSupport(rawGraph);//zsupy
                     tempdouble[3]=(tempdouble[2]*dgx)/(tempdouble[1]*dgy);
                     if(tempdouble[3]>1.0) tempdouble[0]=1.0;//acceptance probability
                     else tempdouble[0]=tempdouble[3];
                     tempdouble[4]=random_uni01();//rnd;
                     if(tempdouble[4]<=tempdouble[0]) break;
                     pool->returnGResource(rawGraph);
               }
    }
    
    void insertIntoHeap()
    {


         heapData *d=pool->getheapDataResources();
         Lastestcancode.resize(Lastestcancode.size());
	 d->code=Lastestcancode;
         std::set<heapData*,compareheapData>::iterator itM=MinedGraph.find(d);
         
         if(itM!=MinedGraph.end()) {
		 (*itM)->graphids.insert(graphIndex-1);
		 
                 pool->returnheapDataResources(d);
         }else{
		 d->graphids.insert(graphIndex-1);
		 double avg_=getAVgSupport (rawGraph);
		 d->avg_support=avg_;
                 MinedGraph.insert(d);
	 }
         


    }

    void printHeap()
    {
	
        
        for(std::set<heapData*,compareE>::iterator itMine=MinedGraph.begin();itMine!=MinedGraph.end();++itMine)
        {
            //  cout<<endl<<"Printing Canonical code: "<<supportValue<<endl;             
              if((*itMine)->graphids.size()>=supportValue)
                {
                        cout<<"#";
                        cout<<(*itMine)->code;
                        cout<<"#"<<(*itMine)->graphids.size()<<"|"<<(*itMine)->avg_support<<endl;
/*                      cout<<"Number of graph this subgraph exists:  "<<(*itMine)->graphids.size()<<endl;
                        cout<<"Number of times this subgraph sampled: "<<(*itMine)->count<<endl;
                        cout<<"This graph has been found in:  ";
                        for(std::set<int>::iterator itg=(*itMine)->graphids.begin();itg!=(*itMine)->graphids.end();++itg)
                        {
                                cout<<(*itg)<<"  ";
                        }
                        cout<<endl;
*/
                }
        }
	cout<<"Total Heap Instance"<<MinedGraph.size()<<endl;

    }

    int  randomMiner()
    {
		int rnd;
		size_t totalsq=0;

		noofGraphs=myGraphSet.size();
		bool count[noofGraphs+1];
		double sampleTime=0.0,codeGenTime=0.0, insertHeapTime=0.0;
		int low=1;int  high=noofGraphs;
		cout<<noofGraphs<<endl;

		for(i=1;i<=noofGraphs;i++)
		{
			count[i]=false;
		}

		int itr=0;
		map<int,int>mymap; mymap.clear();
		for(std::vector<graph*>::iterator itgr=myGraphSet.begin();itgr!=myGraphSet.end();++itgr)
                {
                        mymap[(*itgr)->myid]=itr;
                        itr++;
                }

		if(noofGraphs >=1)
		{
			struct timeval start_time;
  			struct timeval stop_time;
			double  microsec= 1000000.0;double st;
			std::string line;
                        ifstream infile;
                        infile.open(parsefile);
                        int rnd,n,vs,nv,v;
                        Vertex *vy;
                        bool flag=false;
			while(true){
				std::getline(infile,line); 
                                rawGraph=pool->getGResource();

                                if (line.length() < 1){
                               
                                        break;
                                }
                                StringTokenizer strtok = StringTokenizer(line," ");
                                std::string str=strtok.nextToken();
                                if(str=="#")
                                {
                                                
                                        rnd=strtok.nextIntToken();
                                        map<int,int>::const_iterator itmap=mymap.find(rnd);
                                        graphIndex=(*itmap).second+1;
					cout<<"    "<<graphIndex<<endl;
                                
                                }else{
					totalsq++;
                                	gettimeofday(&start_time, (struct timezone *)0);
                                        st = start_time.tv_sec + (start_time.tv_usec/microsec);

                                        n=strtok.countTokens();
                                        vs=boost::lexical_cast< int >(str )-1;
                                        vy =pool->getVResource();
					vy->label=vs;
                                        boost::container::flat_set<Vertex*,compareV>::const_iterator itv=(myGraphSet[graphIndex-1])->myverts.find(vy);
                                        rawGraph->addVertex(*itv);

                                        for(nv=0;nv<n;nv++)
                                        {
                                                v=strtok.nextIntToken()-1;
                                                vy->label=v;
                                                boost::container::flat_set<Vertex*,compareV>::const_iterator itv=(myGraphSet[graphIndex-1])->myverts.find(vy);
                                                rawGraph->addVertex(*itv);
                                        }
					pool->returnVResource(vy);
					if(rawGraph->myverts.size()>0) { 
						populateEdges();
						gettimeofday(&stop_time, (struct timezone *)0);
						sampleTime+=stop_time.tv_sec + (stop_time.tv_usec/microsec) -st;
						gettimeofday(&start_time, (struct timezone *)0);
                                                st = start_time.tv_sec + (start_time.tv_usec/microsec);

				                getcanonicalCode();
					        gettimeofday(&stop_time, (struct timezone *)0);
 			                        codeGenTime+=stop_time.tv_sec + (stop_time.tv_usec/microsec) -st;


						gettimeofday(&start_time, (struct timezone *)0);
                                                st = start_time.tv_sec + (start_time.tv_usec/microsec);

						insertIntoHeap();	
						gettimeofday(&stop_time, (struct timezone *)0);
                        			insertHeapTime+=stop_time.tv_sec + (stop_time.tv_usec/microsec) -st;

					}
					else cout<<"Error populating vertices"<<endl;
				}
				pool->returnGResource(rawGraph);
                        }
			
			
                }else{
                        cout<<"no graph to Mine";
                }
  		printHeap();
		cout<<"Total Seq"<<totalsq<<endl;
                cout<<"Total graph gen time"<<sampleTime<<"canonical code gen Time"<<codeGenTime<<"insert time"<<insertHeapTime<<endl;
		
                return 1;
        }

};

		     
#endif	/* RANDOMMINING_H */

