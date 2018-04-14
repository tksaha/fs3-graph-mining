#include "randomMining.h"
#include<bitset>

void randomMining::ProcessInputFile()
{
  	//time_tracker tt;
	//tt.start();
	std::string line;
	ifstream infile;
	infile.open(datafile, ios::in);
	int count=0;
	GraphClassPtr g1;

	while(1)								
	{
	    std::getline(infile,line);
	   
	    if (line.length() < 1){
		g1->numV=g1->myverts.size();
		g1->numE=g1->myedges.size();
		if(g1->numV>=sizeOfSubgrah){
                    //g1->printAdjacencyList();
                    myGraphSet.push_back(g1);
                }
		else {
			for(boost::container::flat_set<Vertex*,compareV>::iterator itv=g1->myverts.begin();itv!=g1->myverts.end();++itv)
			{
				pool->returnVResource(*itv);
			}
			for(boost::container::flat_set<Edge*,compareE>::iterator itE=g1->myedges.begin();itE!=g1->myedges.end();++itE)
			{
				pool->returnEResource(*itE);
			}
			pool->returnGResource(g1);
		}
		break;
	    }
	   StringTokenizer strtok = StringTokenizer(line,"\t");
	   std::string str=strtok.nextToken(" ");
	   if(str=="t") {
	         
		  if(count>0){
		    g1->numV=g1->myverts.size();
		    g1->numE=g1->myedges.size();
		    if(g1->numV>=sizeOfSubgrah){
                        //g1->printAdjacencyList();
                        myGraphSet.push_back(g1);
                    }
		    else {
			for(boost::container::flat_set<Vertex*,compareV>::iterator itv=g1->myverts.begin();itv!=g1->myverts.end();++itv)
                        {
                                pool->returnVResource(*itv);
                        }
                        for(boost::container::flat_set<Edge*,compareE>::iterator itE=g1->myedges.begin();itE!=g1->myedges.end();++itE)
                        {
                                pool->returnEResource(*itE);
                        }

			pool->returnGResource(g1);
		    }
		    
		  }
		  count++;
		  g1=pool->getGResource();
		  g1->myid=count-1;
	    }
	    else{
		 
		
		 if(!str.compare("v"))
		  {
		      str=strtok.remainingString();
		      StringTokenizer strtok = StringTokenizer(str," ");
		      int  v=strtok.nextIntToken();
		      int vlabel=strtok.nextIntToken();
		      Vertex *vx=pool->getVResource();
		      vx->label=v;
		      vx->globallabel=vlabel;
		      if(g1->vertexmap.find(v)==g1->vertexmap.end())
		      {
		      		g1->addVertex(vx);
		      }
		  }else{
		      str=strtok.remainingString();
		      StringTokenizer strtok = StringTokenizer(str," ");
		      int st=strtok.nextIntToken();
		      int end=strtok.nextIntToken();
		      int elab=strtok.nextIntToken();
		      int i=g1->vertexmap[st];
		      int j=g1->vertexmap[end];
		      int k;
		      if(st==end) continue;//no self loop
		      //for support list
		      if(i>j)  {
			   k=i;
			   i=j;
			   j=i;
		      }
		      if(mysupportList[i][j][elab].lastIndex!=g1->myid)
		      {
				mysupportList[i][j][elab].lastIndex=g1->myid;
				mysupportList[i][j][elab].supportCount++;
		      }
		      //edges 
		      Edge *E=pool->getEResource();
		      if(st<end) {
			  int t=end;
			  end=st;st=t;
		      }
		      Vertex *v1,*v2;
		      v1=pool->getVResource();
		      v2=pool->getVResource();
		      v1->label=st; v2->label=end;
                      ChangeableVertexIterator it=g1->myverts.find(v1);
		      ChangeableVertexIterator itx=g1->myverts.find(v2);		    
		      if(it==g1->myverts.end() || itx==g1->myverts.end())
		      {
			   std::cout<<"vertex not found"<<endl; 			
		      }
                      else {
			
			E->st=(*it); E->end=(*itx);E->edgelabel=elab;
		      	g1->addEdge(E);
		      	g1->updateAdj((*it),(*itx));
		      }
		      pool->returnVResource(v1);
		      pool->returnVResource(v2);
		  }
	      
	    }
	}
//	tt.stop();
//	printSupportList();
//	std::cout<<"File Reading Time: "<<tt.print()<<endl;
	infile.close();
}
string randomMining:: makeFillerForString(string mystring)
{

          for (int len=mystring.length();len<=fillerlength;len++)
          {
                mystring=mystring+" ";
          }
          return mystring;

}

void randomMining:: printSupportList()
{
        int d;
        for(int i=0;i<NODELABEL;i++)
        {
                for(int j=0;j<NODELABEL;j++)
                {
                        for(int k=0;k<EDGELABEL;k++)
                        {
                                d=mysupportList[i][j][k].supportCount;
                                if(mysupportList[i][j][k].supportCount>0) 
                                        std::cout<<i<<"  "<<j<<"  "<<k<<"  "<<d<<endl;
                        }
                }
        }
}

void randomMining::parseArgs(int argc, char* argv[]) {
              if(argc<7) {
                      printUsage(argv[0]);
                      exit(0);
              }
              if(((string)(argv[1]))!="-d" || ((string)(argv[3]))!="-p" || ((string) (argv[5]))!="-sup"){
                      printUsage(argv[0]);
                      exit(0);
              }
              for (int i=1; i < argc; i++){
              		 if ((string)(argv[i])== "-d"){
                                              datafile=argv[++i];
                                              if(ifprint)  cout<<makeFillerForString("Data File:")<<datafile<<endl;
                              }
                              else if( ((string)(argv[i])).compare("-sup")==0){
                                              supportValue=atof(argv[++i]);
                              }
                              else if( ((string)(argv[i])).compare("-p")==0){
                                              parsefile=argv[++i];
                              }
                
              }
}

void randomMining::populateNeighbor(const graph* g,  int forwhom)
{
        if(forwhom){ gypopulation.clear();gypopcount=0; uniqgypopcount=0;}
        else {gxpopulation.clear();gxpopcount=0; uniqgxpopcount=0;}
        globalset.clear();


        for(ConstVertexIterator  itx=g->myverts.begin();itx!=g->myverts.end();++itx)
        {

                  mysetTemp.clear();
                  if((*itx)->list.size()==0) {}
                  else {
                                for(ConstVertexIterator  ita=(*itx)->list.begin();ita!=(*itx)->list.end();++ita)
                                {
                                    if(g->vertexmap.find((*ita)->label)==g->vertexmap.end())
                                    {
                                        mysetTemp.push_back((*ita)->label);
                                        globalset.insert((*ita)->label);
                                    }
                                }

                  }
                  if(forwhom){
                         gypopulation.push_back(mysetTemp);
		         gypopcount=gypopcount+mysetTemp.size();

                  }else{
                         gxpopulation.push_back(mysetTemp);
			 gxpopcount=gxpopcount+mysetTemp.size();
                  }
	}
	if(forwhom==0) uniqgxpopcount=globalset.size();
        else uniqgypopcount=globalset.size();
/*	if(forwhom==0) cout<<"printing gx pop"<<endl;
	else cout<<"printing gy pop"<<endl;
	for(boost::container::flat_set<int>::iterator itg=globalset.begin();itg!=globalset.end();++itg)
	{
		cout<<(*itg)<<endl;
	}
*/
}
bool randomMining::getNeighborGraph(const graph* g, int forwhom)
{
        
	int low=1; int high,rnd,index,labelToAdd,labelToDelete,labelMustnottodelete;
        if(forwhom) high=gypopcount;
        else high=gxpopcount;
	if(high+1 >low){
        	rnd=boost_get_a_random_number(low,high+1);
	}else{
		cout<<"problem in rnd getneighbor"<<endl;
	}
        index=0;
        labelToAdd=-1;
        labelToDelete=-1;
        labelMustnottodelete;

        vector<vector<int> >::iterator itl;
        if(forwhom) itl=gypopulation.begin();
        else  itl=gxpopulation.begin();
        int count=0;
        bool flag=false;
        for(;  ;++itl)
        {
            if(flag) break;
            if(forwhom){
                if(itl==gypopulation.end()) break;
            }
            else{
                if(itl==gxpopulation.end()) break;
            }
            index++;
            if(count+(*itl).size()>=rnd)
            {
                for(vector<int>::iterator itp=(*itl).begin();itp!=(*itl).end();++itp)
                {
                        count++;
                        if(count==rnd) {
                                labelToAdd=(*itp);
                                flag=true;
                                break;
                        }

                }
            }
            else count =count+(*itl).size();

        }
	ConstVertexIterator  x=g->myverts.begin()+index-1;
        if(x!=myGraphSet[graphIndex-1]->myverts.end()) labelMustnottodelete=(*x)->label;
        else {cout<<"Error finding label must not to delete"<<endl;}
        std::vector<int>candidateToDelete;
        candidateToDelete.clear();
        for(ConstVertexIterator  l=g->myverts.begin();l!=g->myverts.end();l++)
        {
                   if((*l)->label==labelMustnottodelete){}
                   else{
                         candidateToDelete.push_back((*l)->label);
                   }

        }
        if(candidateToDelete.size()==1) labelToDelete=candidateToDelete[0];
        else if(candidateToDelete.size()>1){

                labelToDelete=candidateToDelete[boost_get_a_random_number(0,candidateToDelete.size())];
        }else{
		cout<<"Error finding label to delete"<<endl;
	}

        if(labelToAdd==-1 or labelToDelete==-1) cout<<"Error"<<endl;

	Vertex* v=pool->getVResource();
	v->label=labelToAdd;
	ChangeableVertexIterator itvn=myGraphSet[graphIndex-1]->myverts.find(v);
	rawGraph->addVertex(*itvn);    
	//cout<<labelToAdd<<"  "<<labelToDelete<<" "<<labelMustnottodelete<<"label"<<endl; 

        for(ConstVertexIterator   l=g->myverts.begin();l!=g->myverts.end();l++)
        {
                   if((*l)->label==labelToDelete){}
                   else rawGraph->addVertex((*l));
        }
	pool->returnVResource(v);
	populateEdges();
	
	dfs* mydfs=pool->getDFSResource();
	mydfs->gx=rawGraph;
        mydfs->doDFS();

        if(!mydfs->isGraphConnected()){
		//cout<<"returning false"<<endl;
		return false;
	}
	pool->returnDfsResource(mydfs);
	return true;
}     
