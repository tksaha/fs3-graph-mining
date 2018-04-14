#include "graph.h"


void dfs::doDFS()
{
	//color 0 for white 1 for gray 2 for black parent=-1 for NIL	
	for(ChangeableVertexIterator it=gx->myverts.begin();it!=gx->myverts.end();++it)
	{
		(*it)->color=0;
		(*it)->parent=-1;
	}
	for( ChangeableVertexIterator it=gx->myverts.begin();it!=gx->myverts.end();++it)
	{
			
		if((*it)->color==0) {
			DFSVISIT((*it));
		}
	}
	
}

void dfs::DFSVISIT(Vertex* u)
{
	u->color=1;

	for(ChangeableVertexIterator itx=u->list.begin();itx!=u->list.end();++itx)
	{
		if(gx->vertexmap.find((*itx)->label) !=gx->vertexmap.end())
		{
			if((*itx)->color==0){
				(*itx)->parent=(u)->label;
				DFSVISIT((*itx));
			}
		}
	}
	u->color=2;
}

bool dfs::isGraphConnected()
{
	int vertparentminusonecount=0;	
	for(ConstVertexIterator it=gx->myverts.begin();it!=gx->myverts.end();++it)
	{
		if((*it)->parent==-1) vertparentminusonecount++;
		if(vertparentminusonecount>1) return false;
	}
	
	return true;
}
