#include "randomMining.h"
#include "codeset.h"

void randommining::ProcessInputFile()
{
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
			g1->reset();
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
			    else{
				//need to add some code:delete verts and edges.
				
				for(boost::container::flat_set<Vertex*,compareV>::iterator itv=g1->myverts.begin();itv!=g1->myverts.end();++itv)
				{
					pool->returnVResource(*itv);
				}
				for(boost::container::flat_set<Edge*,compareE>::iterator itE=g1->myedges.begin();itE!=g1->myedges.end();++itE)
				{
					pool->returnEResource(*itE);
				}
				g1->reset();
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
			   k=i;i=j;j=k;
		      }
		      if (g1->myverts.size()<sizeOfSubgrah) continue;
		      if(mysupportList[i][j][elab].lastIndex!=g1->myid)
		      {
				mysupportList[i][j][elab].lastIndex=g1->myid;
				mysupportList[i][j][elab].supportCount++;
				mysupportList[i][j][elab].sids[g1->myid]=1;
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
			   pool->returnEResource(E);
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
/*	printSupportList();*/
	infile.close();
}
string randommining:: makeFillerForString(string mystring)
{

        for (int len=mystring.length();len<=fillerlength;len++)
        {
               mystring=mystring+" ";
        }
        return mystring;

}

void randommining:: printSupportList()
{
        int d;
        for(i=0;i<NODELABEL;i++)
        {
                for(j=0;j<NODELABEL;j++)
                {
                        for( k=0;k<EDGELABEL;k++)
                        {
                                d=mysupportList[i][j][k].supportCount;
                                if(mysupportList[i][j][k].supportCount>0) 
                                        std::cout<<i<<"  "<<j<<"  "<<k<<"  "<<d<<endl;
                        }
                }
        }
}

void randommining::parseArgs(int argc, char* argv[]) {
      if(argc<7) {
	      printUsage(argv[0]);
	      exit(0);
      }
      if(((string)(argv[1]))!="-d" || ((string)(argv[3]))!="-i" || ((string) (argv[5]))!="-s"){
	      printUsage(argv[0]);
	      exit(0);
      }
      for ( i=1; i < argc; i++){
		      if ((string)(argv[i])== "-d"){
				      datafile=argv[++i];
				      if(ifprint)  cout<<makeFillerForString("Data File:")<<datafile<<endl;
		      }
		      else if((string)(argv[i])== "-i"){
				      noofIteration=atof(argv[++i]);
				      if(ifprint) cout<<makeFillerForString("noofIteration:")<<noofIteration<<endl;
		      }
		      else if((string)(argv[i])== "-s"){
				      sizeOfSubgrah=atof(argv[++i]);
				      if(ifprint) cout<<makeFillerForString("sizeOfSubgrah:")<<sizeOfSubgrah<<endl;
		      }
		      else if( ((string)(argv[i])).compare("-q")==0){
				      Qsize=boost::lexical_cast<int>(argv[++i]);
		      }
	}
}
void randommining::populateNeighbor(const graph* g,  int forwhom)
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

}
bool randommining::getNeighborGraph(const graph* g, int forwhom)
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
        labelMustnottodelete=-1;

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

        for(ConstVertexIterator   l=g->myverts.begin();l!=g->myverts.end();l++)
        {
                   if((*l)->label==labelToDelete){}
                   else rawGraph->addVertex((*l));
        }
	pool->returnVResource(v);
	populateEdges();
	
        doDFS();
        if(!isGraphConnected()){
		return false;
	}
	return true;
}  

void randommining::insertIntoHeap()
{

	typedef code_set::index<codex>::type code_set_by_code;
	code_set_by_code& code_index=mycodeset.get<codex>();

	code_set_by_code::iterator it=code_index.find(Lastestcancode);
	if(it!=code_index.end())
	{
		heapData hd=*it;
		temp[0]=hd.graphids.size();
		hd.graphids.insert(graphIndex-1);
		if(temp[0]==hd.graphids.size()) {}
		else {
		        hd.totalid.assign(boost::lexical_cast<string>(hd.graphids.size()));
			for(temp[9]=hd.totalid.size();temp[9]<gTot;temp[9]++)
			{
				hd.totalid='0'+hd.totalid;
			}				
			hd.count=LastIteration;
		}
		code_index.replace(it,hd);
	}
	else {
		if(mycodeset.size()<Qsize){
			totid="1";
			for(temp[9]=1;temp[9]<gTot;temp[9]++)
                        {
                               totid='0'+totid;
                        }
			mycodeset.insert(heapData(Lastestcancode,LastIteration,graphIndex-1,totid,_avgSupport));


		}else{
			typedef code_set::index<graphidsx>::type code_set_by_graphid;
			code_set_by_graphid&  graphid_index=mycodeset.get<graphidsx>();
			code_set_by_graphid::iterator itg=graphid_index.begin();
			heapData hd=*itg;
			hd.mysupport.assign("");
			hd.mysupport.assign(_avgSupport);
			hd.code.assign(Lastestcancode);
			hd.graphids.clear();
			hd.count=LastIteration;
			hd.graphids.insert(graphIndex-1);
			hd.totalid.assign(boost::lexical_cast<string>(hd.graphids.size()));
                        for(temp[9]=hd.totalid.size();temp[9]<gTot;temp[9]++)
                        {
                                hd.totalid='0'+hd.totalid;
                        } 
			graphid_index.replace(itg,hd);
		}
	}

}

void randommining::printHeap()
{
	
    	print_out_by<graphidsx>(mycodeset);   	

}

int randommining:: randomMiner()
{
	int rnd,cnt;
	noofGraphs=myGraphSet.size();
	bool count[noofGraphs+1],toContinue;
	double sampleTime=0.0,codeGenTime=0.0, insertHeapTime=0.0;
	int low=1; int  high=noofGraphs;
	cout<<noofGraphs<<endl;

	for(i=1;i<=noofGraphs;i++)
	{
		count[i]=false;
	}
	if(noofGraphs >=1)
	{
		vector<GraphClassPtr>recentGraphs(noofGraphs+1);
		struct timeval start_time;
		struct timeval stop_time;
		double  microsec= 1000000.0;double st;
		size_t i=0;
		string noIter=boost::lexical_cast<string>(noofIteration);
		nIter=noIter.size();
		string totalG=boost::lexical_cast<string>(myGraphSet.size());
		gTot=totalG.size();
		while(i++<noofIteration)
		{
			graphIndex =boost_get_a_random_number(low,high+1);
			//cout<<graphIndex<<endl;	
			gettimeofday(&start_time, (struct timezone *)0);
			st = start_time.tv_sec + (start_time.tv_usec/microsec);	
			if(count[graphIndex]==false) {	
				LastLoop=0;
				getRandomInitialSubgraph();
				recentGraphs[graphIndex]=rawGraph;
				count[graphIndex]=true;
				//totsup=(int) getAVgSupport(recentGraphs[graphIndex]);
				totsup=(int) getSetIntersection(recentGraphs[graphIndex]);

			}else{
				if(myGraphSet[graphIndex-1]->myverts.size()==sizeOfSubgrah)
				{
					continue;
				}
				popNeiAndGetOne(recentGraphs[graphIndex]);
				pool->returnGResource(recentGraphs[graphIndex]);
				recentGraphs[graphIndex]=rawGraph;
				if(rawGraph->myedges.size()<sizeOfSubgrah-1)
				{
					cout<<"Subgraph has not been sampled Properly"<<endl;	
					recentGraphs[graphIndex]->printAdjacencyList();
				}
				/*recentGraphs[graphIndex]->printAdjacencyList();*/
			}


			gettimeofday(&stop_time, (struct timezone *)0);
			sampleTime+=stop_time.tv_sec + (stop_time.tv_usec/microsec) -st;

			gettimeofday(&start_time, (struct timezone *)0);
			st = start_time.tv_sec + (start_time.tv_usec/microsec);
			
			/* check to code gen  */
			LastIteration.assign(boost::lexical_cast<string>(i));
			LastIter=i;
			for(cnt=(boost::lexical_cast<string>(i)).size();cnt<nIter;cnt++)
			{
				LastIteration='0'+LastIteration;
			}
		
			_avgSupport.assign(boost::lexical_cast<string>(totsup));
			
			for(cnt=_avgSupport.size();cnt<gTot; cnt++)
			{
				_avgSupport='0'+_avgSupport;
			}
			if(mycodeset.size()==Qsize) {
				typedef code_set::index<graphidsx>::type code_set_by_graphid;
				code_set_by_graphid&  graphid_index=mycodeset.get<graphidsx>();
				code_set_by_graphid::iterator itg=graphid_index.begin();
				if(_avgSupport.compare( (*itg).mysupport)<0) continue;
			}
			getcanonicalCode();
			gettimeofday(&stop_time, (struct timezone *)0);
			codeGenTime+=stop_time.tv_sec + (stop_time.tv_usec/microsec) -st;
			
			/*check to insert*/
			gettimeofday(&start_time, (struct timezone *)0);
			st = start_time.tv_sec + (start_time.tv_usec/microsec);
			insertIntoHeap();
			gettimeofday(&stop_time, (struct timezone *)0);
			insertHeapTime+=stop_time.tv_sec + (stop_time.tv_usec/microsec) -st;

		}
	}else{
		cout<<"no graph to Mine";
	}
	printHeap();
	cout<<"Total Sample Time"<<sampleTime<<"  Total code Gen Time:"<<codeGenTime<<"  Total insert Time:"<<insertHeapTime<<endl;
	cout<<"|"<<sampleTime<<"|"<<codeGenTime<<"|"<<insertHeapTime<<"|"<<endl;	
	return 1;
}
   
