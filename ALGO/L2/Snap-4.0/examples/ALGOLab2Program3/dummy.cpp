#include "stdafx.h"
#include <stdlib.h>
#define NO_OF_EDGES 10000
#define NO_OF_VERTICES 1000
#define NO_OF_TRIALS 1

int newvertexindex = 10;
int vertexcover[NO_OF_VERTICES]={-1};
int length;
int isvertexalreadythereinset(int vertex);
void addvertextoset(int nid);
void printvertexcover();
const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "graph.txt", "Edge list format");
int minvertexcoverlength = 32000;


void removeedgesandmerge(PNEGraph G,int V,int NewVertex)
{
	printf("--------------------------------------\n");
	 typedef PNEGraph PGraph;
	 for (typename PGraph::TObj::TEdgeI ei = G->BegEI(); ei < G->EndEI(); ei++) {
		  	          printf("%d\t%d\n", ei.GetSrcNId(), ei.GetDstNId());
		  	          if(ei.GetSrcNId() == V && ei.GetDstNId()!= V)
		  	          {
		  	        	G->AddEdge(V,ei.GetDstNId());
		  	        	G->DelEdge(ei.GetSrcNId(), ei.GetDstNId());
		  	          }
		  	          else if(ei.GetSrcNId() != V && ei.GetDstNId() == V)
		  	          {
		  	        	G->AddEdge(V,ei.GetDstNId());
		  	        	G->DelEdge(ei.GetSrcNId(), ei.GetDstNId());
		  	          }
		  	          else if(ei.GetSrcNId() == V && ei.GetDstNId() == V)
		  	          {
		  	        	G->DelEdge(ei.GetSrcNId(), ei.GetDstNId());
		  	          }
	 }
	 printf("--------------------------------------\n");

}

int main(int argc, char* argv[]) {
  //// what type of graph do you want to use?
  //typedef PUNGraph PGraph; // undirected graph
  //typedef PNGraph PGraph;  //   directed graph
  typedef PNEGraph PGraph;  //   directed multigraph
  //typedef TPt<TNodeNet<TInt> > PGraph;
  //typedef TPt<TNodeEdgeNet<TInt, TInt> > PGraph;

  // this code is independent of what particular graph implementation/type we use
  printf("Creating graph:\n");
  PGraph G = PGraph::TObj::New();
  G = TSnap::LoadEdgeList<PNEGraph>(InFNm);

  printf("no of edges = %d\n",G->GetEdges());
  int actualNoOfEdgesCreated = G->GetEdges();
  int loop = 0;
  G->Dump();

  while(loop < NO_OF_TRIALS)
  {
	  length=0;

	  int noofverticesremaining = G->GetNodes();
	  	  while(noofverticesremaining > 2){

	  		  	  int randomedgeid = G->GetRndEId();
							  //printf("%d\t%d\n",G->GetEdge(randomedgeid).GetSrcNId(),G->GetEdge(randomedgeid).GetDstNId());
							  printf("random edge id is %d\n",randomedgeid);

							  if(G->IsEdge(randomedgeid))
								  G->DelEdge(randomedgeid);
	  	    		  	  removeedgesandmerge(G,G->GetEdge(randomedgeid).GetSrcNId(),newvertexindex);
	  	    		  	  /*removeedgesandmerge(G,G->GetEdge(randomedgeid).GetDstNId(),newvertexindex);
	  	    		  	  newvertexindex++;*/

//	  	    		  	  /break;
							  printf("no of vertices is %d\n",G->GetNodes());
							  G->Dump();
							  noofverticesremaining = G->GetNodes();


	  	        }


	  	  /*
	  	for (typename PGraph::TObj::TEdgeI ei = G->GetRndEI(); ei < G->EndEI(); ei++) {
	  		  	          	  printf("%d\t%d\n", ei.GetSrcNId(), ei.GetDstNId());
	  		  		  	  	  	  int randomedgeid = G->GetRndEId();
	  		  	    		  	  removeedgesandmerge(G,G->GetEdge(randomedgeid).GetSrcNId(),newvertexindex);
	  		  	    		  	  removeedgesandmerge(G,G->GetEdge(randomedgeid).GetDstNId(),newvertexindex);
	  		  	    		  	  newvertexindex++;

	  		  	    		  	  break;



	  		  	        }*/


	  //printvertexcover();
	  //printf("In Iteration %d ,size of vertex cover is %d\n",loop,length);
	  if(minvertexcoverlength > length)
		  minvertexcoverlength = length;
	  //G = TSnap::LoadEdgeList<PUNGraph>(InFNm);
	  loop++;


  }

  printf("Minimum Vertex Cover length is %d \n",minvertexcoverlength);
  G->Dump();
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



