#ifndef GRAPH_H_
#define GRAPH_H_
#include<set>
#include<vector>
#include<string>
#include<map>
#include<list>
#include<bitset>
#include <boost/interprocess/containers/flat_set.hpp>
#include <iostream>
#include <boost/unordered_map.hpp>
#include "boost/smart_ptr.hpp"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/global_fun.hpp>
#define  NODELABEL 30
#define  EDGELABEL 20

using boost::multi_index_container;
using namespace boost::multi_index;


using namespace boost;
using namespace std;


typedef boost::container::flat_set<int> BoostIntSet;
typedef	std::map<int,int> ::const_iterator ConstMapIterator;

class MySupport{
/* Limits the graph database size to 700 */
public:	
	int supportCount;
	int lastIndex;	
	bitset<700>sids;
	int lastLoop; size_t lastIter;
};

class Vertex{
public:
        int label;
	int  globallabel;
        boost::container::flat_set<Vertex*>list;
	int color;
	int parent;
	Vertex(){}
        Vertex(int lab)
        {
	  label=lab;
	  list.clear();
        }
	Vertex(int lab, int gbl)
	{
	  label=lab;
	  globallabel=gbl;
	  list.clear();
	}
        void reset()
	{
	 	list.clear();
	}
        ~Vertex()
        {
	  list.clear();
        } 
};


class Edge{
public:
        Vertex  *st;
        Vertex  *end;
	int edgelabel;
	Edge(){}
	Edge(Vertex* stx,  Vertex* edx) //edges are stored in sorted pair (bigvertex,smallvertex) 
	{
		if(stx->label > edx->label){
			st=stx;
			end=edx;
		}else{
			st=edx;
		        end=stx;
		}
	}
	Edge( Vertex* start,  Vertex* ed, int  label)
	{
		st=start;
		end=ed;
		edgelabel=label;
	}
	int reset()
	{}
        ~Edge()
        { }
	
	
};


class compareV { 
   public:
     inline bool operator()(const Vertex *x,const Vertex  *y)const{ 
	 	 return x->label<y->label;
      } 
};

class compareE { 
   public:
      inline bool operator()(const Edge   *x,const Edge *y)const{ 
	 if(x->st->label !=y->st->label) return x->st->label<y->st->label;
	 else return x->end->label<y->end->label;
      } 
 
};


typedef	boost::container::flat_set<Vertex*,compareV> VertexSet;
typedef boost::container::flat_set<Edge*,compareE> EdgeSet;


typedef EdgeSet::const_iterator ConstEdgeSetIterator;
typedef EdgeSet::iterator ChangeableEdgeSetIterator;


typedef VertexSet::const_iterator ConstVertexIterator;
typedef VertexSet::iterator ChangeableVertexIterator;

class graph{

public:
	VertexSet myverts;
	EdgeSet myedges;
	int myid;
	map<int,int> vertexmap;
public:
	int numV;
	int numE;
	graph();
	~graph();
	void reset()
	{
		myverts.clear();	
		myedges.clear();
		vertexmap.clear();
	}
 	void addVertex(Vertex* v);
	void addEdge(Edge* E);
	void updateAdj(Vertex* s,  Vertex *t);
	void printAdjacencyList();
};
typedef graph* GraphClassPtr;


class fiveTuple
{

public: int dfsst, dfsend;
        int stlabel,edgelabel,endlabel;
	fiveTuple(){}
        fiveTuple(int dfsSt,int dfsEnd,int stLabel, int edgeLabel, int endLabel)
        {
                dfsst=dfsSt;dfsend=dfsEnd;
                stlabel=stLabel;
                edgelabel=edgeLabel;
                endlabel=endLabel;
        }
};

class canonicalcodeStruc{
public:
        int maxcode;
        canonicalcodeStruc(){
                edges.clear();
                codetuples.clear();
                dfscodemap.clear();
                maxcode=0;
        }
        canonicalcodeStruc(canonicalcodeStruc* code)
        {}
	void reset()
	{
		edges.clear();	
		codetuples.clear();
		dfscodemap.clear();
		maxcode=0;
	}
	~canonicalcodeStruc(){}
        std::vector<Edge*> edges;
        std::vector<fiveTuple*>codetuples; //newly added
        map<int,int>dfscodemap;
};

             
class heapData{
public:
        heapData(){count="";}
	heapData(string codex,string _count,int id,string _totalid,string _mysup):code(codex),count(_count){
		graphids.insert(id);
		totalid.assign(_totalid);
		mysupport.assign(_mysup);
	}
        heapData (string codex)
        {
                code.assign(codex);
                graphids.clear();
        }
	void changeme(string _codex,string _count,int _id){
		code.assign(_codex);
		count=_count;
		graphids.clear();
		graphids.insert(_id);
	}
        ~heapData(){
                graphids.clear();
                code.assign("");
        }
	void reset()
	{
		graphids.clear();
		code.assign("");
		count.assign("");
		mysupport.assign("");
	}
	string mysupport;
	string totalid;
	string count;
        string code;
        std::set<int>graphids; //now graph has real ids numbered 0 through number.
        bool operator==(const heapData* other)const{
                if(code.compare(other->code)==0){
                        return true;
                }
                else return false;
        }
	friend std::ostream& operator<<(std::ostream& os,const heapData& e)
  	{
    		cout<<"#"<<e.code<<"#"<<e.graphids.size()<<"|"<<e.mysupport<<endl;
    		return os;
  	}
};

class codex{};
class countx{};
class graphidsx{};


class myobjectpool{
private:
	std::list<Vertex*> vertexResources;
	std::list<Edge*> edgeResources;
	std::list<graph*> graphResources;
	std::list<fiveTuple*> fiveTupleResources;
	std::list<canonicalcodeStruc*> canonicalcodeResources;
	std::list<heapData*> heapDataResources;
public:	myobjectpool() {}


	Vertex* getVResource()
	{
		if (vertexResources.empty())
		{
			for(int i=0;i<50;i++)
			{
				Vertex *v =new Vertex();
				vertexResources.push_back(v);
			}
		}
		
		Vertex* resource = vertexResources.front();
		vertexResources.pop_front();
		return resource;
	}
	
	void returnVResource(Vertex* v)
	{
		v->reset();
		vertexResources.push_back(v);
	}
	Edge* getEResource()
	{
		if (edgeResources.empty())
		{
			for(int i=0;i<50;i++)
			{
				Edge *E=new Edge();
				edgeResources.push_back(E);
			}
		}
		
		Edge* resource = edgeResources.front();
		edgeResources.pop_front();
		return resource;
	}
	
	void returnEResource(Edge* E)
	{
		E->reset();
		edgeResources.push_back(E);
	}
	graph* getGResource()
	{
		if(graphResources.empty())
		{
			for(int i=0;i<15;i++)
			{
				graph* g=new graph();
				graphResources.push_back(g);
			}
		}
		graph* resource=graphResources.front();
		graphResources.pop_front();
		return resource;
	}
	void returnGResource(graph* g)
	{
		g->reset();
		graphResources.push_back(g);
	}
	fiveTuple* getFiveTupleResource()
	{
		if(fiveTupleResources.empty())
		{
			for(int i=0;i<40;i++)
			{
				fiveTupleResources.push_back(new fiveTuple());
			}
	
		}
		fiveTuple* resource =fiveTupleResources.front();
		fiveTupleResources.pop_front();
		return resource;
	}
	void returnFiveTupleResource(fiveTuple* five)
	{
		fiveTupleResources.push_back(five);
	}
	canonicalcodeStruc* getcancodeResource()
	{
		if(canonicalcodeResources.empty())
		{
			for(int i=0;i<40;i++)
			{
				canonicalcodeResources.push_back(new canonicalcodeStruc());
			}
		}
		canonicalcodeStruc* resource=canonicalcodeResources.front();
		canonicalcodeResources.pop_front();
		return resource;
	}
	void returncancodeResource(canonicalcodeStruc* code)
	{
		code->reset();
		canonicalcodeResources.push_back(code);
	}
	heapData* getheapDataResources()
	{
		if(heapDataResources.empty())
		{	
			for(int i=0;i<40;i++)
			{
				heapDataResources.push_back(new heapData());
			}
		}
		heapData* resource=heapDataResources.front();
		heapDataResources.pop_front();
		return resource;
	}
	void returnheapDataResources(heapData* h)
	{
		h->reset();
		heapDataResources.push_back(h);
	}
	
};



#endif

/* GRAPH_H_ */

