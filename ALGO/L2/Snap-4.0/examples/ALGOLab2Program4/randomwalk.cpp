#include "stdafx.h"
#include <stdlib.h>
#define NO_OF_EDGES 100
#define NO_OF_VERTICES 20
#define NO_OF_TRIALS 10
#define SOURCE 1
#define DESTINATION 10


const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "graph.edgelist", "Edge list format");
int besttrial = -1;
int noofruns = 32000;

int getrandomneighbourofgivennode(PUNGraph G,int node)
{
	for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++)
	{
		if(NI.GetId() == node)
		{
			//srand (time(NULL));
			int randomneighbour = rand() % NI.GetOutDeg();
			//printf("random index is %d\n",randomneighbour);
			//printf("outdegree is %d\n",NI.GetOutDeg());
			for (int e = 0; e < NI.GetOutDeg(); e++)
			{
				if(e == randomneighbour)
					return NI.GetInNId(e);
			}
		}

	}
	return -1;
}

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
  }
  IAssert(G->IsOk());
  TSnap::SaveEdgeList(G, "graph.edgelist", "Edge list format");
  int loop = 1;
  G->Dump();
  while(loop <= NO_OF_TRIALS)
  {
	  printf("+++++++++++++++++++++++++++++ Trial %d +++++++++++++++++++++++\n",loop);
	  	  int nooftimestorun = 2*NO_OF_VERTICES*NO_OF_VERTICES*NO_OF_VERTICES;
	  	  int currentrun = 1;
	  	  int neighbourselected = getrandomneighbourofgivennode(G,SOURCE);
	  	  //printf("Neighbour selected for node %d is %d\n",SOURCE,neighbourselected);
	  	  printf("%d-->",SOURCE);
	  	  while(currentrun <= nooftimestorun)
	  	  {
	  		  	if(neighbourselected == DESTINATION)
				{
	  		  		printf("%d",neighbourselected);
					printf("\nDestination reached successfully in %d run(s)!!\n",currentrun);
					if(noofruns > currentrun)
					{
						noofruns = currentrun;
						besttrial = loop;
					}
					break;
				}
	  		  	printf("%d-->",neighbourselected);
	  		  	int node = neighbourselected;
	  		  	neighbourselected = getrandomneighbourofgivennode(G,neighbourselected);
				//printf("Neighbour selected for node %d is %d\n",node,neighbourselected);

				currentrun++;
	  	  }
	  	  loop++;
	  	  printf("\n");
  }

  printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  if(besttrial != -1)
  {
	  printf("Best Trial is %d and found destination in %d run(s)\n",besttrial,noofruns);
  }
  else
	  printf("Best Trial not found even after %d trials\n",NO_OF_TRIALS);
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
return 0;
}




