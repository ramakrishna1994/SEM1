#include "stdafx.h"
#include <stdlib.h>

#define NO_OF_VERTICES 8
#define NO_OF_TRIALS 100

int vertexcover[NO_OF_VERTICES]={-1};
int length;
int isvertexalreadythereinset(int vertex);
void addvertextoset(int nid);
void printvertexcover();
const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "graph.edgelist", "Edge list format");
int minvertexcoverlength = 32000,besttrial;

int getrandomnodefromgraph(PUNGraph G)
{
	return G->GetRndNId();

}

int checkforneighboursexists(PUNGraph G,int node)
{
	for (typename PUNGraph::TObj::TNodeI ni = G->BegNI(); ni < G->EndNI(); ni++)
	{
		if(ni.GetId() == node)
			if(ni.GetDeg() > 0)
				return 1;
	}
	return 0;
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
  G = TSnap::LoadEdgeList<PUNGraph>(InFNm);
  G->Dump();
  int actualNoOfEdgesCreated = G->GetEdges();
  int loop = 1;
  while(loop <= NO_OF_TRIALS)
  {
	  printf("+++++++++++++++++++++++++++++ Trial %d +++++++++++++++++++++++\n",loop);
	  length=0;
	  while(G->GetEdges() > 0)
	  {
		int node = getrandomnodefromgraph(G);
		if(checkforneighboursexists(G,node))
			addvertextoset(node);
		G->DelNode(node);
	  }
	  printf("size of vertex cover is %d\n",length);
	  printvertexcover();
	  if(minvertexcoverlength > length)
	  {
		  minvertexcoverlength = length;
		  besttrial = loop;
	  }

	  G = TSnap::LoadEdgeList<PUNGraph>(InFNm);
	  loop++;
  }
  printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  printf("Minimum Vertex Cover length is %d\n",minvertexcoverlength,besttrial);
  printf("Best Trial is %d\n",besttrial);
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
		vertexcover[length] = nid;
		length++;

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



