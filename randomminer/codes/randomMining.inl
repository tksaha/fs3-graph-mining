inline randommining::randommining()
{
      sizeOfSubgrah=0;
      fillerlength=15;
      noofGraphs=1;
      ifprint=true;
      noofIteration=0;
      Qsize=totsup=0;
      for( i=0;i<NODELABEL;i++)
	{
		for( j=0;j<NODELABEL;j++)
		{

			for( k=0;k<EDGELABEL;k++)
			{
				MySupport mysup;
				mysup.lastIndex=-1;
				mysup.supportCount=0;
				mysup.lastLoop=-1;
				mysup.lastIter=-1;
				mysupportList[i][j][k]=mysup;
			}
		}
	}
	for( i=0;i<10;i++)
	{
		temp[i]=0; 
		tempbool[i]=false;
		tempdouble[i]=0.0;
	}
	pool=new myobjectpool();
	myGraphSet.clear();
	globalset.clear();
	LastIter=0; LastLoop=0;
	iCodeStage=cindex=0;
}
inline randommining:: ~randommining()
{
	myGraphSet.clear();
	globalset.clear();

}

inline double randommining:: getSetIntersection( const graph *g)
{
	//supportx=0.0;
	temp[0]=0;temp[1]=0;temp[2]=0;temp[3]=0;
	bitset<700>  allids;
	int index=0;
	int cnt=0;
	for(ConstEdgeSetIterator it=g->myedges.begin();it!=g->myedges.end();++it)
	{
		   temp[0]=(*it)->st->globallabel;
		   temp[1]=(*it)->end->globallabel;
		   temp[3]=(*it)->edgelabel; 
		   if(temp[0]>temp[1]) { 
				temp[2]=temp[0]; temp[0]=temp[1];temp[1]=temp[2];
		   }
		   temp[2]=mysupportList[temp[0]][temp[1]][temp[3]].supportCount;
		   if(temp[2]==0) cout<<"Error in avg. support"<<endl;
		   else {
			
			if(mysupportList[temp[0]][temp[1]][temp[3]].lastLoop==LastLoop && 
					mysupportList[temp[0]][temp[1]][temp[3]].lastIter==LastIter) continue	; 
			else{
				mysupportList[temp[0]][temp[1]][temp[3]].lastLoop=LastLoop;
				mysupportList[temp[0]][temp[1]][temp[3]].lastIter=LastIter;
			}
			if(index==0) {
				allids=allids | mysupportList[temp[0]][temp[1]][temp[3]].sids;
				index++;
			}
			else {
				cnt++;
			
				allids=allids &  mysupportList[temp[0]][temp[1]][temp[3]].sids ;
			}
		  }
	}
	//cout<<" Set Intersection Size"<<minEd.size()<<endl;
	for(ConstEdgeSetIterator it=g->myedges.begin();it!=g->myedges.end();++it)
	{
		   temp[0]=(*it)->st->globallabel;
		   temp[1]=(*it)->end->globallabel;
		   temp[3]=(*it)->edgelabel; 
		   if(temp[0]>temp[1]) { 
				temp[2]=temp[0]; temp[0]=temp[1];temp[1]=temp[2];
		   }
		   temp[2]=mysupportList[temp[0]][temp[1]][temp[3]].supportCount;
		   if(temp[2]==0) cout<<"Error in avg. support"<<endl;
		   else{
			 mysupportList[temp[0]][temp[1]][temp[3]].lastLoop=-1;
			 mysupportList[temp[0]][temp[1]][temp[3]].lastIter=-1;	
		   }	

	}
//	cout<<"set operation done: "<<cnt<<endl;
	return (double) allids.count();

}
inline void randommining::gettwoVertexGraph()
{
		
	temp[0]=1; temp[1]=myGraphSet[graphIndex-1]->myedges.size();
	int rnd=boost_get_a_random_number(temp[0], temp[1]+1);
	ChangeableEdgeSetIterator  itx=myGraphSet[graphIndex-1]->myedges.begin()+rnd-1;
	rawGraph->addVertex((*itx)->st) ;
	rawGraph->addVertex((*itx)->end);
}
inline void randommining::populateEdges()
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
inline void randommining::getRandomInitialSubgraph()
{
	if(sizeOfSubgrah<2) { cout<<"Size must be at least 2"<<endl;}
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
inline void randommining:: getcanonicalCode(){
	iCodeStage=0;
	newlevelcode.clear();
	code.clear();
	getfirstlabelCanonicalCode();
	if(rawGraph->myedges.size()>1) populateNextLevels();
	printCanonicalCode();
}
//canonincal code stuff ends;
inline int randommining::absmine(int a,int b)
{
		if((a-b)<0) return -(a-b);
		else return (a-b);
} 
inline void randommining::popNeiAndGetOne(const graph *g)
{
	int dgx,dgy;
	double  zSupx,zSupy,acc_prob,accvalue,rnd;
	populateNeighbor(g, 0);
	dgx=uniqgxpopcount;
	if(dgx==0){ cout<<"What?"<<endl;}
	zSupx=getSetIntersection(g);
	LastLoop=0;
	while(true){
	     rawGraph =pool->getGResource();
	     while(!getNeighborGraph(g, 0))
	     {
		       pool->returnGResource(rawGraph);
		       rawGraph=pool->getGResource();
	     }
	     populateNeighbor(rawGraph, 1);
	     dgy=uniqgypopcount; //dgy
	     zSupy=getSetIntersection(rawGraph);
	     accvalue=(zSupy*dgx)/(zSupx*dgy);
	     if(accvalue>1.0) acc_prob=1.0;//acceptance probability
	     else acc_prob=accvalue;
	     rnd=random_uni01();//rnd;
	     totsup=(int)(zSupy);
	     if(rnd<=acc_prob) break;
	     pool->returnGResource(rawGraph);
	     LastLoop++;
       }
}
