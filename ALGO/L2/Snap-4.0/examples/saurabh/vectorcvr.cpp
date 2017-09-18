#include "stdafx.h"
#include<stdlib.h>
#include<unistd.h>


bool checkalldone()
{

}
int main(int argc, char* argv[])
 {
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Graph generators. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  TExeTm ExeTm;
  PUNGraph G;
 // int ran_ver;
  Try
{
	  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "output.txt", "Output graph filename");
	  const TStr Plot = Env.GetIfArgPrefixStr("-g:", "e", "Which generator to use:"
	    "\n\tf: Complete graph. Required parameters: n (number of nodes)"
	    "\n\ts: Star graph. Required parameters: n (number of nodes)"
	   "\n\t2: 2D Grid. Required parameters: n (number of rows), m (number of columns)"
	    "\n\tk: Random k-regular graph. Required parameters: n (number of nodes), k (degree of every node)");
	  const int N = Env.GetIfArgPrefixInt("-n:", 1000, "Number of nodes");
	  const int M = Env.GetIfArgPrefixInt("-m:", 5000, "Number of edges");
	  //const double P = Env.GetIfArgPrefixFlt("-p:", 0.1, "Probability/Degree-exponent");
	  const int K = Env.GetIfArgPrefixInt("-k:", 3, "Degree");
	  const int trial = Env.GetIfArgPrefixInt("-t:", 1, "Number of trials");
	  if (Env.IsEndOfRun()){ return 0; }
	  TExeTm ExeTm;
	  TInt::Rnd.PutSeed(0); // initialize random seed
	  printf("Generating...\n");
	 
	  TStr DescStr;
	  if (Plot == "f") {
	    G = TSnap::GenFull<PUNGraph>(N);
	    DescStr = TStr::Fmt("Undirected complete graph.");
	  } else
	  if (Plot == "s") {
	    G = TSnap::GenStar<PUNGraph>(N, false);
	    DescStr = TStr::Fmt("Undirected star graph (1 center node connected to all other nodes).");
	  } else
	  if (Plot == "2") {
	    G = TSnap::GenGrid<PUNGraph>(N, M, false);
	    DescStr = TStr::Fmt("Undirected 2D grid of %d rows and %d columns.", N, M);
	  } 
	  if (Plot == "k") {
	    G = TSnap::GenRndDegK(N, K);
	    DescStr = TStr::Fmt("Undirected k-regular random graph (every node has degree K).");
	  } 
  
  printf("done.\n");
  // save graph
  TSnap::SaveEdgeList(G, OutFNm, DescStr);
  //TFOut FOut("out1.graph");G->Save(FOut);

// load the graph
  
    PUNGraph G= TSnap ::LoadEdgeList<PUNGraph>(OutFNm,0,1);

 // traverse all the edges directly, count them
  int NCount = 0;
  /*for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++) {
  printf("edge between node %d and node %d\n",EI.GetSrcNId(), EI.GetDstNId());
      NCount++;
  }*/
  NCount = G->GetEdges();
  printf("Number of edges %d", NCount);
 int v_coversize[32000];
 int edgeset[32000][32000];
// -1 --> not present any edge ,1 -->present but not travered ,2--> present and travered

for(int i =0 ;i < NCount ; i++)
{	for(int j =0 ;j < NCount ; j++)
	{
		if(G->IsEdge(i,j))
			edgeset[i][j]=1;
		else
			edgeset[i][j]=10000;
	}
}
for(int i =0 ;i < NCount ; i++)
{
	v_coversize[i]=-999;
}
//visit all edges 

  	for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++) 
	{
	printf("edge between node %d and node %d\n",EI.GetSrcNId(), EI.GetDstNId());
	edgeset[EI.GetSrcNId()][EI.GetDstNId()]=1;
	edgeset[EI.GetDstNId()][EI.GetSrcNId()]=1;
	}


// Consider all vertices by selecting it randomly for all trails
    
for(int t=0 ;t<trial ;t++)
{  	
    int p=0 , z=0;
    
    bool flag =true ;
   	
    while(flag) //check if there is any unvisited edge
    {

    	flag = false;

    	/*TUNGraph::TEdgeI EI = G->EndEI(); //Returns an edge iterator referring to the past-the-end edge in the graph
	int s =	rand()%(EI.GetId());
	int d =	rand()%(EI.GetId()); */
    	int s,d;
    	while(1)
    	{
    		s = G->GetRndNId();
			d = G->GetRndNId();
			if(G->IsEdge(s,d))
				break;
    	}
    printf(" chkpoint1") ;
	if(edgeset[s][d]==1 ||edgeset[d][s]==1 )
	{
		edgeset[s][d]=2;	
		edgeset[d][s]=2;
		p++;
	}

	 for(int i =0 ;i < NCount ; i++)
	{	for(int j =0 ;j < NCount ; j++)
		{
		if(edgeset[i][j]==1)
		flag= true;
		break;
		}


	}

    }
	v_coversize[z++]=p;
			
}
int min = 9999 ;
for(int j=0 ;j<trial ;j++)
{
	if(v_coversize[j]<min)
	{
	min=v_coversize[j];
	}
}
printf("Size of minimum vertex cover is :%d",min);
}
  Catch
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
}



