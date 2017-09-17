#include "stdafx.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

#define NO_OF_VERTICES 5242
#define NO_OF_TRIALS 10000



const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "graph.txt", "Edge list format");
int besttrial,bestmindomsetlength = 32000;
int mindomset[NO_OF_VERTICES]={-1};
int completednodes[NO_OF_VERTICES]={-1};
int lengthofcompletednodes=0;
int lengthofmindomset=0;



int getrandomnodefromgraph(PUNGraph G)
{
	return G->GetRndNId();

}

void removeselfloopsingraph(PUNGraph G)
{
	for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++)
		{
			if(EI.GetSrcNId() == EI.GetDstNId())
				G->DelEdge(EI.GetSrcNId(),EI.GetDstNId());
		}

}

void insertintomindomset(int node)
{
	//printf("Node %d is inserted into mindomset \n",node);
	mindomset[lengthofmindomset] = node;
	lengthofmindomset++;
}

void printmindomset()
{
	printf("mindomset is {");
	for(int i=0;i<lengthofmindomset;i++)
		printf("%d,",mindomset[i]);
	printf("}\n");
}

int main(int argc, char* argv[]) {
  //// what type of graph do you want to use?
  typedef PUNGraph PGraph; // undirected graph
  //typedef PNGraph PGraph;  //   directed graph
  //typedef PNEGraph PGraph;  //   directed multigraph
  //typedef TPt<TNodeNet<TInt> > PGraph;
  //typedef TPt<TNodeEdgeNet<TInt, TInt> > PGraph;


  printf("Creating graph:\n");
  PGraph G = PGraph::TObj::New();

 /* for(int i=1;i<=NO_OF_VERTICES;i++)
	  G->AddNode(i);*/

  int loop = 1;

  while(loop <= NO_OF_TRIALS)
  {
	  printf("+++++++++++++++++++++++++++++ Trial %d +++++++++++++++++++++++\n",loop);
	  	  G = TSnap::LoadEdgeList<PUNGraph>(InFNm);
	  	  removeselfloopsingraph(G);
	  	  lengthofcompletednodes=0;
	  	  lengthofmindomset=0;
	 	  while(1)
	 	  {
	  			//G->Dump();
	  			int node = getrandomnodefromgraph(G);
				//printf("Node with max degree is %d\n",node);
				for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++)
				{
					if(NI.GetId() == node)
					{
						insertintomindomset(NI.GetId());
						//printf("Node %d has degree %d\n",NI.GetId(),NI.GetDeg());
						int size = NI.GetDeg();
						while(size!=0 )
						{
							G->DelNode(NI.GetInNId(0));
							//printf("node id %d has neighbor %d is deleted\n", NI.GetId(), NI.GetOutNId(0));
							size = NI.GetDeg();

						}
						G->DelNode(node);
						//printf("node %d added to mindomset and all its neighbours and itself are deleted\n--------------\n",node);
						break;
					}
				}
				if(G->Empty())
					break;
	 	  }

	  //printmindomset();
	 	  printf("Length of Dom set is %d\n",lengthofmindomset);
	  if(bestmindomsetlength > lengthofmindomset)
	  {
		  bestmindomsetlength = lengthofmindomset;
		  besttrial = loop;
	  }

	  loop++;
  }

  printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  printf("Best Minimum Dominating Set Length is %d\n",bestmindomsetlength);
  printf("Best Trial is %d \n",besttrial);
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
return 0;
}




