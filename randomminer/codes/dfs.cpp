#include "graph.h"
#include "randomMining.h"

void randommining::doDFS()
{
	//color 0 for white 1 for gray 2 for black parent=-1 for NIL	
	for(ChangeableVertexIterator it=rawGraph->myverts.begin();it!=rawGraph->myverts.end();++it)
	{
		(*it)->color=0;
		(*it)->parent=-1;
	}
	for( ChangeableVertexIterator it=rawGraph->myverts.begin();it!=rawGraph->myverts.end();++it)
	{
			
		if((*it)->color==0) {
			DFSVISIT((*it));
		}
	}
	
}

void randommining::DFSVISIT(Vertex* u)
{
	u->color=1;

	for(ChangeableVertexIterator itx=u->list.begin();itx!=u->list.end();++itx)
	{
		if(rawGraph->vertexmap.find((*itx)->label) !=rawGraph->vertexmap.end())
		{
			if((*itx)->color==0){
				(*itx)->parent=(u)->label;
				DFSVISIT((*itx));
			}
		}
	}
	u->color=2;
}

bool randommining::isGraphConnected()
{
	int vertparentminusonecount=0;	
	for(ConstVertexIterator it=rawGraph->myverts.begin();it!=rawGraph->myverts.end();++it)
	{
		if((*it)->parent==-1) vertparentminusonecount++;
		if(vertparentminusonecount>1) return false;
	}
	
	return true;
}
