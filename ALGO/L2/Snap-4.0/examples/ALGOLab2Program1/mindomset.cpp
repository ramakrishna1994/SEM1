#include "stdafx.h"
#include <stdlib.h>
#define NO_OF_EDGES 7
#define NO_OF_VERTICES 6
#define NO_OF_TRIALS 5



const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "graph.edgelist", "Edge list format");
int besttrial,bestmindomsetlength = 32000;
int mindomset[NO_OF_VERTICES]={-1};
int completednodes[NO_OF_VERTICES]={-1};
int lengthofcompletednodes=0;
int lengthofmindomset=0;

int isnodealreadyselected(int node)
{
	for(int i=0;i<lengthofcompletednodes;i++)
	{
		if(node == completednodes[i])
		{
			return 1;
		}
	}
	return 0;
}

int allnodescompleted()
{
	return lengthofcompletednodes;
}

int selectrandomnodefromgraph(PUNGraph G)
{
	if(lengthofcompletednodes == NO_OF_VERTICES)
		return -1;
	while(1)
	{

		srand (time(NULL));
		int randomnode = rand() % G->GetNodes();
		if(!isnodealreadyselected(randomnode))
		{
			completednodes[lengthofcompletednodes]=randomnode;
			lengthofcompletednodes++;
			return randomnode;
		}

	}

}

int checkfornodeexistsinmindomset(int node)
{
	for(int i=0;i<lengthofmindomset;i++)
	{
		if(mindomset[i] == node)
		{
			return 1;
		}

	}
	return 0;
}

void insertintomindomset(int node)
{
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
  int loop = 1;
  G->Dump();
  while(loop <= NO_OF_TRIALS)
  {
	  printf("+++++++++++++++++++++++++++++ Trial %d +++++++++++++++++++++++\n",loop);
	  	  lengthofcompletednodes=0;
	  	  lengthofmindomset=0;
	  	  while(lengthofcompletednodes < NO_OF_VERTICES)
	  	  {
				int randomnode = selectrandomnodefromgraph(G);
				printf("Randomly selected node is %d\n",randomnode);
				for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++)
				{
					if(NI.GetId() == randomnode)
					{
						int isthere = checkfornodeexistsinmindomset(randomnode);
						if(!isthere)
						{
							int count = 0;
							for (int e = 0; e < NI.GetOutDeg(); e++)
							{
								isthere = checkfornodeexistsinmindomset(NI.GetInNId(e));
								if(!isthere)
									count++;
							  // get the node ID and its neighbour
							  //printf("node id %d has neighbor %d\n", NI.GetId(), NI.GetInNId(e));
							}
							if(count == NI.GetOutDeg())
								insertintomindomset(randomnode);
						}

					}

				}

	  	  }

	  printmindomset();
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




