#include "stdafx.h"
#include <stdlib.h>
#define NO_OF_EDGES 10000
#define NO_OF_VERTICES 1000
#define NO_OF_TRIALS 10000

int vertexcover[NO_OF_VERTICES]={-1};
int length;
int isvertexalreadythereinset(int vertex);
void addvertextoset(int nid);
void printvertexcover();
const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "graph.edgelist", "Edge list format");
int minvertexcoverlength = 32000;


int main(int argc, char* argv[]) {
  //// what type of graph do you want to use?
  typedef PUNGraph PGraph; // undirected graph
  //typedef PNGraph PGraph;  //   directed graph
  //typedef PNEGraph PGraph;  //   directed multigraph
  //typedef TPt<TNodeNet<TInt> > PGraph;
  //typedef TPt<TNodeEdgeNet<TInt, TInt> > PGraph;

  // this code is independent of what particular graph implementation/type we use
  printf("Creating graph:\n");
  PGraph G = PGraph::TObj::New();

  for (int n = 0; n < NO_OF_VERTICES; n++) {
    G->AddNode(); // if no parameter is given, node ids are 0,1,...,9
  }
  G->AddEdge(0, 1);
  for (int e = 0; e < NO_OF_EDGES; e++) {
    const int NId1 = G->GetRndNId();
    const int NId2 = G->GetRndNId();
    G->AddEdge(NId1, NId2);
    /*
    if (G->AddEdge(NId1, NId2) != -2) {
      //printf("  Edge %d -- %d added\n", NId1,  NId2); }
    else {
      //printf("  Edge %d -- %d already exists\n", NId1, NId2); }*/
  }
  IAssert(G->IsOk());
  TSnap::SaveEdgeList(G, "graph.edgelist", "Edge list format");
  printf("no of edges = %d\n",G->GetEdges());
  int actualNoOfEdgesCreated = G->GetEdges();
  int loop = 0;
  while(loop < NO_OF_TRIALS)
  {
	  length=0;

	  for(int i=0;i<actualNoOfEdgesCreated;i++)
	    {
	  	  int randomedge = rand() % (actualNoOfEdgesCreated - i);
	  	  //printf("random is %d\n",randomedge);
	  	  int index=0;
	  	  for (typename PGraph::TObj::TEdgeI ei = G->BegEI(); ei < G->EndEI(); ei++) {
	  	          //printf("%d\t%d\n", ei.GetSrcNId(), ei.GetDstNId());
	  	    	  if (randomedge == index && !isvertexalreadythereinset(ei.GetSrcNId()) && !isvertexalreadythereinset(ei.GetDstNId()))
	  	    		  {
	  	    		  	  addvertextoset(ei.GetSrcNId());
	  	    		  	  addvertextoset(ei.GetDstNId());
	  	    		  	  G->DelEdge(ei.GetSrcNId(), ei.GetDstNId());
	  	    		  	  break;
	  	    		  }
	  	    	  else if(randomedge == index)
	  	    	  {
	  	    		  G->DelEdge(ei.GetSrcNId(), ei.GetDstNId());
	  	    		  break;
	  	    	  }
	  	    	  index++;

	  	        }
	    }
	  //printvertexcover();
	  printf("In Iteration %d ,size of vertex cover is %d\n",loop,length);
	  if(minvertexcoverlength > length)
		  minvertexcoverlength = length;
	  G = TSnap::LoadEdgeList<PUNGraph>(InFNm);
	  loop++;
  }



/*
  for (typename PGraph::TObj::TEdgeI ei = G->BegEI(); ei < G->EndEI(); ei++) {
      //printf("%d\t%d\n", ei.GetSrcNId(), ei.GetDstNId());
	  if (!isvertexalreadythereinset(ei.GetSrcNId()) && !isvertexalreadythereinset(ei.GetDstNId()))
		  {
		  	  addvertextoset(ei.GetSrcNId());
		  	  addvertextoset(ei.GetDstNId());
		  }

    }*/

  //G->Dump();
  /*delete
  PGraph::TObj::TNodeI NI = G->GetNI(0);
  printf("Delete edge %d -- %d\n", NI.GetId(), NI.GetOutNId(0));
  G->DelEdge(NI.GetId(), NI.GetOutNId(0));
  const int RndNId = G->GetRndNId();
  printf("Delete node %d\n", RndNId);
  G->DelNode(RndNId);
  IAssert(G->IsOk());*/
  printf("Minimum Vertex Cover length is %d \n",minvertexcoverlength);
   return 0;
}


int isvertexalreadythereinset(int vertex)
{

	for(int i=0;i<length;i++)
	{
		//printf("vertex cover is %d and vertex is %d\n",vertexcover[i],vertex);
		if(vertexcover[i] == vertex)
			return 1;
	}
	return 0;
}

void addvertextoset(int nid)
{
	if(!isvertexalreadythereinset(nid))
	{
		vertexcover[length] = nid;
		length++;
	}

}


void printvertexcover()
{
	printf("vertex cover is {");
	for(int i=0;i<length;i++)
	{
		printf("%d,",vertexcover[i]);
	}
	printf("}\n");

}



